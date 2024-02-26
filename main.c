#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 


int main() {
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "exes invaders");

    Vector2 rocketPosition = { (float)2*screenWidth/3, (float)2*screenHeight/3 };
    Vector2 joePosition = { (float)100, (float)100 };
    Texture2D rocket = LoadTexture("assets/rocket-pixel.png");
    Texture2D joe = LoadTexture("assets/joe-pixel.png");
    
    int invaders_direction = 1;

    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) rocketPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) rocketPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) rocketPosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) rocketPosition.y += 2.0f;
        
        
        if(joePosition.x == screenWidth-300) invaders_direction = 0;
        else if(joePosition.x == 100) invaders_direction = 1;

        if(invaders_direction && joePosition.x < screenWidth-300) joePosition.x += 2.0f;
        else if(!invaders_direction && joePosition.x > 100) joePosition.x -= 2.0f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(rocket, rocketPosition, WHITE);
            DrawTextureV(joe, joePosition, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(rocket);    // Unload texture
    UnloadTexture(joe);    // Unload texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
