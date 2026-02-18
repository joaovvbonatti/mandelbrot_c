#version 400

out vec4 finalColor;
uniform vec2 resolution;
uniform int maxIter;
uniform vec2 center;
uniform double scale;

vec3 palette(float t) {
    return vec3(
    0.5 + 0.5*cos(6.28318*(t + 0.3)),
    0.5 + 0.5*cos(6.28318*(t + 0.6)),
    0.5 + 0.5*cos(6.28318*(t + 0.9))
    );
}

void main() {
    //gl_FragCoord gives us a relative coordinate between 0 and width/height, starting from bottom-left
    vec2 coord = gl_FragCoord.xy / resolution; //now we have normalized coordinates between 0 and 1
    vec2 uv = coord * 2.0 - 1.0; //in this step, we end up with the plane centered in [0,0], negative values left and down, positive values right and up
    uv.x *= resolution.x / resolution.y; //un-stretch the plane
    uv *= scale; //apply scale
    vec2 complex = center + uv; //apply centering

    vec2 z = vec2(0.0);
    int i;

    for (i = 0; i < maxIter; i++) { //main mandelbrot loop
        z = vec2(
        z.x*z.x - z.y*z.y + complex.x,
        2.0*z.x*z.y + complex.y
        );

        if (dot(z, z) > 4.0) break;
    }

    if (i == maxIter) { //color
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float mu = float(i) - log2(log(length(z)));
        float t = mu / float(maxIter);
        vec3 color = palette(t);
        finalColor = vec4(color, 1.0);
    }
}