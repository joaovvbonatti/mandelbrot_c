#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../external/raygui/raygui.h"

#define INITIAL_WIDTH 1600
#define INITIAL_HEIGHT 900

static void updateResolutionUniform(Shader shader) {
    Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    int resLoc = GetShaderLocation(shader, "resolution");
    SetShaderValue(shader, resLoc, &res, SHADER_UNIFORM_VEC2);
}

static float updateZoom(Shader shader, Vector2* center, int centerLoc, float scale) {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    float scroll = GetMouseWheelMove();
    if (scroll != 0) {
        Vector2 mousePos = GetMousePosition(); //in pixels

        float nx = (mousePos.x / screenWidth) * 2.0f - 1.0f;
        float ny = 1.0f - (mousePos.y / screenHeight) * 2.0f;
        nx *= screenWidth / screenHeight;

        Vector2 before = {
            center->x + nx * scale,
            center->y + ny * scale
        };

        if (scroll == 1) {
            scale *= 0.9f;
        } else if (scroll == -1) {
            scale *= 1.1f;
        }

        Vector2 after = {
            center->x + nx * scale,
            center->y + ny * scale
        };

        center->x += before.x - after.x;
        center->y += before.y - after.y;

        SetShaderValue(shader, centerLoc, center, SHADER_UNIFORM_VEC2);
    }
    return(scale);
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "mandelbrot_c");
    GuiLoadStyle("themes/dark.rgs");
    SetTargetFPS(60);

    Shader shader = LoadShader(0, "shaders/mandelbrot.frag"); //raylib default vertex shader and our own custom fragment shader

    static int maxIter = 100;
    static float maxIterF = 100.0f;
    int maxIterLoc = GetShaderLocation(shader, "maxIter");

    Vector2 center = {-0.5f, 0};
    int centerLoc = GetShaderLocation(shader, "center");
    SetShaderValue(shader, centerLoc, &center, SHADER_UNIFORM_VEC2);

    float scale = 1.5f;
    int scaleLoc = GetShaderLocation(shader, "scale");


    while (!WindowShouldClose()) { //main loop, executed every frame
        updateResolutionUniform(shader);
        scale = updateZoom(shader, &center, centerLoc, scale);

        float screenWidth = (float)GetScreenWidth();
        float screenHeight = (float)GetScreenHeight();


        BeginDrawing();
            ClearBackground(BLACK);

            maxIter =  (int) maxIterF;
            SetShaderValue(shader, maxIterLoc, &maxIter, SHADER_UNIFORM_INT);

            SetShaderValue(shader, scaleLoc, &scale, SHADER_UNIFORM_FLOAT);

            BeginShaderMode(shader);
                DrawRectangle(0, 0, (int)screenWidth, (int)screenHeight, WHITE);
            EndShaderMode();

            //drawing the gui
            GuiPanel((Rectangle){screenWidth - 310, 10, 300, 150}, "Controls");
            maxIter = GuiSliderBar((Rectangle){screenWidth - 215, 40, 160, 25}, "Iterations    ", TextFormat("%i", maxIter), &maxIterF, 1, 10000);
            GuiLabel((Rectangle) {screenWidth - 300, 70, 180, 25}, TextFormat("Scale: %f", scale));
            GuiLabel((Rectangle) {screenWidth - 300, 100, 360, 25}, TextFormat("Current coordinates: %f, %f", center.x, center.y));
            GuiLabel((Rectangle) {screenWidth - 300, 130, 360, 25}, TextFormat("Scroll to zoom"));
        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();

    return 0;
}