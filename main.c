#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "colisoes.h"
#include "menu.h"


int main() {
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1640;
    const int screenHeight = 924;

    int nivel = 1; //Indica o nível atual do jogo

    abrirMenu();
    SetRandomSeed(time(NULL));

    //Setando os exes
    Ex *exes=NULL;
    int qtd_exes=0;
    int qtd_texture_exes = 1; //Quando tiver mais exes fora o joe, a gente aumenta isso daqui
    Texture2D texture_exes[qtd_texture_exes]; 
    texture_exes[0] = LoadTexture("assets/joe-pixel.png");
    exes = Generate_Exes(nivel, &qtd_exes, texture_exes);

    //Setando os Projéteis
    Projectile_List projectile_list = {NULL, 0}; //vetor de projéteis começa como NULL e tem 0 projéteis no mapa
    Texture texture_projectiles = LoadTexture("assets/Musical_Note.png");

    //Setando o foguete/Taylor
    Rocket rocket;
    rocket.vida = 3;
    rocket.texture = LoadTexture("assets/rocket-pixel.png");
    rocket.position = (Rectangle){ (float)2*screenWidth/3, (float)2*screenHeight/3, rocket.texture.width, rocket.texture.height };


    float intervalo = 5, tempo = 5; //Tempo pra uma nave atirar
    int invaders_direction = 1;
    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {


        

        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT) && rocket.position.x < screenWidth - rocket.texture.width) rocket.position.x += 4.0f;
        if (IsKeyDown(KEY_LEFT) && rocket.position.x > 0) rocket.position.x -= 4.0f;
        if (IsKeyPressed(KEY_SPACE)) {
            projectile_list = Generate_Taylor_Projectile(rocket, projectile_list, texture_projectiles);
            printf("%d\n", projectile_list.qtd_projectile);
        }

        if((qtd_exes>0) && (GetTime()>tempo)){ //Cada vez que se passam "intervalo" segundos uma das naves atira 
            int random_ex = GetRandomValue(0, qtd_exes-1);
            projectile_list = Generate_Ex_Projectile(exes[random_ex], projectile_list, texture_projectiles);
            tempo+=intervalo;
        }


        for(int i=0;i<qtd_exes;i++){
            if(exes[i].position.x == screenWidth-300) invaders_direction = 0;
            else if(exes[i].position.x == 100) invaders_direction = 1;
        }


        for(int i=0;i<qtd_exes;i++){
            if(invaders_direction) exes[i].position.x += 2.0f;
            else if(!invaders_direction) exes[i].position.x -= 2.0f;
        }
        //----------------------------------------------------------------------------------

        projectile_list = Projectile_Movement(projectile_list);
        

        Check_Collision_Projectiles_Exes(&exes, &qtd_exes, &projectile_list);
        Check_Projectiles_Boundaries(&projectile_list, screenWidth, screenHeight);
        Check_Collision_Projectiles_Rocket(&rocket, &projectile_list);
        int game_status = Check_Game_Status(&rocket, &projectile_list, &exes, &qtd_exes);

        printf("%d\n", projectile_list.qtd_projectile);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            //DrawRectangleRec(teste, BLUE);
            DrawTextureV(rocket.texture, (Vector2){rocket.position.x, rocket.position.y}, WHITE); //Casting pq rocket agora é um Rectangle e drawTexture só aceita Vector2D
            Draw_Exes(exes, qtd_exes);
            Draw_Projectiles(projectile_list);
            DrawText(TextFormat("Vida: %d", rocket.vida), 10, 10, 20, RED);
            // if(game_status == 1){
            //     gameOver();
            // }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(exes);
    free(projectile_list.projectiles);
    Unload_All_Textures(rocket, texture_projectiles, texture_exes, qtd_texture_exes);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
