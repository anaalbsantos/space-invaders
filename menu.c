#include "menu.h"
#include "raylib.h"

int abrirMenu(){
    
    const int screen_width = 1640;
    const int screen_height = 924;
    int bloqueador = 0;
    int nivel = 0;
    

    InitWindow(screen_width, screen_height , "exes invaders");

    Texture2D menu = LoadTexture("./assets/Menu.png");        // Carrega as texturas a partir de imagens da galeria
    Texture2D tutorial = LoadTexture("./assets/Tutorial.png");
    SetTargetFPS(60);
    
    //PlaySound(Menu);
    
    while(IsKeyUp(KEY_ESCAPE) && IsKeyUp(KEY_ONE) && IsKeyUp(KEY_TWO) && IsKeyUp(KEY_THREE)){

        BeginDrawing();
        
        if(bloqueador == 0)
            DrawTexture(menu, screen_width/2 - menu.width/2, screen_height/2 - menu.height/2, WHITE); //desenha o mapa no centro da tela);

        if(IsKeyDown(KEY_H)){
            bloqueador = 1;
            DrawTexture(tutorial, screen_width/2 - tutorial.width/2, screen_height/2 - tutorial.height/2, WHITE); //desenha o mapa no centro da tela);
        }

        EndDrawing();
        
        if(IsKeyDown(KEY_ONE)){
            nivel = 1;
        }

        if(IsKeyDown(KEY_TWO)){
            nivel = 2;
        }

        if(IsKeyDown(KEY_THREE)){
            nivel = 3;
        }

    }

    return nivel;
}


void gameOver(){

    Texture2D GameOver = LoadTexture("./assets/Gameover.png");

    while(IsKeyUp(KEY_ESCAPE) && IsKeyUp(KEY_ENTER)){
        BeginDrawing();
        DrawTexture(GameOver, 0, 0, WHITE);
        EndDrawing();
    }

    if(IsKeyDown(KEY_ESCAPE)){
        CloseWindow();
    }
    
    if(IsKeyDown(KEY_ENTER)){

        }
}

void gameWin(){
        
        Texture2D Vitoria = LoadTexture("./Assets/Win.png");

        while(IsKeyUp(KEY_ESCAPE) && IsKeyUp(KEY_ENTER)){
            BeginDrawing();
            DrawTexture(Vitoria, 0, 0, WHITE);
            EndDrawing();
        }

        if(IsKeyDown(KEY_ESCAPE)){
            CloseWindow();
        }

        if(IsKeyDown(KEY_ENTER)){

        }
}