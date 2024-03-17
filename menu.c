#include "menu.h"
#include "raylib.h"

int abrirMenu(){
    
    const int screen_width = 1640;
    const int screen_height = 924;
    int bloqueador = 0;
    int fechar = 0;

    InitWindow(screen_width, screen_height , "exes invaders");

    Texture2D menu = LoadTexture("./assets/Menu.png");        // Carrega as texturas a partir de imagens da galeria
    Texture2D tutorial = LoadTexture("./assets/Tutorial.png");
    SetTargetFPS(60);
    
    
    while(IsKeyUp(KEY_ENTER)==1 && IsKeyUp(KEY_ESCAPE)==1){

        BeginDrawing();
        
        if(bloqueador == 0)
            DrawTexture(menu, screen_width/2 - menu.width/2, screen_height/2 - menu.height/2, WHITE); //desenha o mapa no centro da tela);

        if(IsKeyDown(KEY_H)){
            bloqueador = 1;
            DrawTexture(tutorial, screen_width/2 - tutorial.width/2, screen_height/2 - tutorial.height/2, WHITE); //desenha o mapa no centro da tela);
        }

        EndDrawing();
        
        if(IsKeyDown(KEY_ESCAPE)){
            fechar = 1;
        }

    }

    return fechar;
}


void gameOver(){

    // const int screen_weight = 1600;
    // const int screen_height = 800;
    Texture2D GameOver = LoadTexture("./assets/Gameover.png");


    while(IsKeyUp(KEY_ESCAPE) && IsKeyUp(KEY_ENTER)){
        BeginDrawing();
        DrawTexture(GameOver, 0, 0, WHITE);
        EndDrawing();
    }
}

void gameWin(){
    // const int screen_weight = 1600;
    // const int screen_height = 800;
    Texture2D Vitoria = LoadTexture("./assets/Win.png");
    SetTargetFPS(60);

    while(IsKeyUp(KEY_ESCAPE) && IsKeyUp(KEY_ENTER)){
        BeginDrawing();
        DrawTexture(Vitoria, 0,0, WHITE);
        EndDrawing();
    }
}