#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 

typedef struct {
    Vector2 position;
    Texture2D texture;
} Rocket;

int main() {
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "exes invaders");

    Rocket rocket;
    Vector2 rocket_pos = { (float)2*screenWidth/3, (float)2*screenHeight/3 };
    rocket.position = rocket_pos;
    rocket.texture = LoadTexture("assets/rocket-pixel.png");

    Vector2 joePosition = { (float)100, (float)100 };
    Texture2D joe = LoadTexture("assets/joe-pixel.png");

    Texture2D bullet = LoadTexture("assets/bullet.png");
    
    int invaders_direction = 1;
    int bullet_direction = 1;

    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) rocket.position.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) rocket.position.x -= 2.0f;
        if(IsKeyDown(KEY_SPACE)) bullet_direction = 1;
        //else bullet_direction = 0;
        
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
            //DrawTexture(bullet, 10, 10, WHITE);

            if(bullet_direction) DrawTextureV(bullet, rocket.position, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(rocket.texture);    // Unload texture
    UnloadTexture(joe);    // Unload texture
    UnloadTexture(bullet);    // Unload texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
