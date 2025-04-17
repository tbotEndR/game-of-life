#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stack>

#define CELL_SIZE 20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "game of life");

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 200, 200 };
    camera.offset = (Vector2){ 200.0f, 200.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    RenderTexture gameboard = LoadRenderTexture(screenWidth, screenHeight);

    // Build a flipped rectangle the size of the split view to use for drawing later
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)gameboard.texture.width, (float)-gameboard.texture.height };

    //  Draw grid into a texture once (since it never changes)
    BeginTextureMode(gameboard);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    
        for (int i = 0; i < screenWidth/CELL_SIZE + 1; i++)
        {
            DrawLineV((Vector2){(float)CELL_SIZE*i, 0}, (Vector2){ (float)CELL_SIZE*i, (float)screenHeight}, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight/CELL_SIZE + 1; i++)
        {
            DrawLineV((Vector2){0, (float)CELL_SIZE*i}, (Vector2){ (float)screenWidth, (float)CELL_SIZE*i}, LIGHTGRAY);
        }        

        EndMode2D();
    EndTextureMode();

    std::stack<Vector2> newActiveCells;

    //--------------------------------------------------------------------------------------

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //  calculate cell's top left corner
            Vector2 cellCoords = GetMousePosition();
            cellCoords.x = cellCoords.x - ((int)cellCoords.x % CELL_SIZE);
            cellCoords.y = cellCoords.y - ((int)cellCoords.y % CELL_SIZE);
            newActiveCells.push(cellCoords);
        }
        BeginTextureMode(gameboard);
        BeginMode2D(camera);
        //  draw active cells into texture
        while (!newActiveCells.empty()) {
            Vector2 currentCell = newActiveCells.top();
            DrawRectangle(currentCell.x, currentCell.y, CELL_SIZE, CELL_SIZE, RED);
            newActiveCells.pop();
        }
        EndMode2D();
        EndTextureMode();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTextureRec(gameboard.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
            DrawTextEx(GetFontDefault(), TextFormat("[%i|%i]", GetMouseX(), GetMouseY()), Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(gameboard);     // Unload render texture
    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}