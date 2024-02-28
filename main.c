#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include "menu.h"

typedef struct {
    Vector2 position;
    Texture2D texture;
} Rocket;

int main() {
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1640;
    const int screenHeight = 924;

    abrirMenu();
    InitWindow(screenWidth, screenHeight, "exes invaders");

    Rocket rocket;
    Vector2 rocket_pos = { (float)2*screenWidth/3, (float)2*screenHeight/3 };
    rocket.position = rocket_pos;
    rocket.texture = LoadTexture("assets/rocket-pixel.png");

    Vector2 joePosition = { (float)100, (float)100 };
    Texture2D joe = LoadTexture("assets/joe-pixel.png");
    
    int invaders_direction = 1;

    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) rocket.position.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) rocket.position.x -= 2.0f;
        
        
        if(joePosition.x == screenWidth-300) invaders_direction = 0;
        else if(joePosition.x == 100) invaders_direction = 1;

        if(invaders_direction && joePosition.x < screenWidth-300) joePosition.x += 2.0f;
        else if(!invaders_direction && joePosition.x > 100) joePosition.x -= 2.0f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(rocket.texture, rocket.position, WHITE);
            DrawTextureV(joe, joePosition, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(rocket.texture);    // Unload texture
    UnloadTexture(joe);    // Unload texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
