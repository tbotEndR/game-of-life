#include "raylib.h"

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

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera split screen");

    Rectangle player1 = { 200, 200, CELL_SIZE, CELL_SIZE };

    Camera2D camera1 = { 0 };
    camera1.target = (Vector2){ 200, 200 };
    camera1.offset = (Vector2){ 200.0f, 200.0f };
    camera1.rotation = 0.0f;
    camera1.zoom = 1.0f;

    RenderTexture gameboard = LoadRenderTexture(screenWidth, screenHeight);

    // Build a flipped rectangle the size of the split view to use for drawing later
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)gameboard.texture.width, (float)-gameboard.texture.height };

    //  Draw grid into a texture once (since it never changes)
    BeginTextureMode(gameboard);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera1);
    
        for (int i = 0; i < screenWidth/CELL_SIZE + 1; i++)
        {
            DrawLineV((Vector2){(float)CELL_SIZE*i, 0}, (Vector2){ (float)CELL_SIZE*i, (float)screenHeight}, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight/CELL_SIZE + 1; i++)
        {
            DrawLineV((Vector2){0, (float)CELL_SIZE*i}, (Vector2){ (float)screenWidth, (float)CELL_SIZE*i}, LIGHTGRAY);
        }

        /*
        for (int i = 0; i < screenWidth/CELL_SIZE; i++)
        {
            for (int j = 0; j < screenHeight/CELL_SIZE; j++)
            {
                DrawText(TextFormat("[%i,%i]", i, j), 10 + CELL_SIZE*i, 15 + CELL_SIZE*j, 5, LIGHTGRAY);
            }
        }
        */

        EndMode2D();
    EndTextureMode();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        BeginTextureMode(gameboard);
        BeginMode2D(camera1);
            //  draw active cells into texture
        EndMode2D();
        EndTextureMode();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(gameboard.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(gameboard);    // Unload render texture

    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}