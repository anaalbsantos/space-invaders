#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "colisoes.h"
#include "menu.h"
#include "animacao.h"


#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
//#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */

// ioctl commands defined for the pci driver header
//#include "ioctl_cmds.h"

#include<stdio.h>
#include<math.h>


int main() {
	   
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1640;
    const int screenHeight = 924;

    int nivel; //Indica o nível atual do jogo
    nivel = abrirMenu();
    float aux = GetTime();
    SetRandomSeed(time(NULL));

    Ex *exes=NULL;
    int qtd_exes = 0;
    int qtd_texture_exes = 0;
    int frame = 0;
    Rocket rocket;
    Texture2D backgroung;
    Color cor;
    Projectile_List projectile_list;
    Texture texture_projectiles;

    float intervalo, tempo; //Tempo pra uma nave atirar
    int invaders_direction;
    int total_exes;
    int outro;
    unsigned int score, data_score;
    int var, pontuacao;
    int *vetor;
    int shot;

    Texture2D texture_exes[11];
    texture_exes[0] = LoadTexture("assets/joe-sprite.png");
    for(int i=1; i<11; i++){
            texture_exes[i] = LoadTexture("assets/bixinho.png");
        } //Carrega as texturas dos exes
    exes = Generate_Exes(nivel, 11, texture_exes);

    //variaveis de controle do personagem
    *vetor, shot = 0;
    vetor = calloc(4, sizeof(int));

    Generate_Level(nivel, &rocket, &backgroung, &cor, &projectile_list, &texture_projectiles, &qtd_exes, exes, &qtd_texture_exes, &intervalo, &tempo, &invaders_direction, &total_exes, &outro, &score, &data_score, &var, &pontuacao, vetor, &shot, &frame);

    // game loop
    while (!WindowShouldClose()){
		
        // Update---------------------------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT) && rocket.position.x < screenWidth - rocket.texture.width) rocket.position.x += 4.0f;
        if (IsKeyDown(KEY_LEFT) && rocket.position.x > 0) rocket.position.x -= 4.0f;
        if (IsKeyPressed(KEY_SPACE) && !shot) {
            projectile_list = Generate_Taylor_Projectile(rocket, projectile_list, texture_projectiles);
            shot = 1;

           // printf("%d\n", projectile_list.qtd_projectile);
        }
        if(IsKeyDown(KEY_SPACE) && shot) shot = 0;

        if((qtd_exes>0) && (GetTime()-aux>tempo*0.2)){ //Cada vez que se passam "intervalo" segundos uma das naves atira 
            int random_ex = GetRandomValue(0, qtd_exes-1);
            projectile_list = Generate_Ex_Projectile(exes[random_ex], projectile_list, texture_projectiles);
            tempo+=intervalo*(total_exes/qtd_exes);
        }


        for(int i=0;i<qtd_exes;i++){
            if(exes[i].position.x == screenWidth-300) invaders_direction = 0;
            else if(exes[i].position.x == 100) invaders_direction = 1;
        }


        for(int i=0;i<qtd_exes;i++){
            if(invaders_direction) {
                // if(i==0) exes[i].position.x += 4.0f; //só o joe é mais rápido else 
                exes[i].position.x += 2.0f;
            }else if(!invaders_direction){
                // if(i==0) exes[i].position.x -= 4.0f; else 
                exes[i].position.x -= 2.0f;
            }
        }

        projectile_list = Projectile_Movement(projectile_list);
       	
       	var = 0;
        var = Check_Collision_Projectiles_Exes(&exes, &qtd_exes, &projectile_list);
        if(var == 1) {
        	score += pontuacao;
        	pontuacao += 5 + pontuacao*0.5;
        }
        Check_Projectiles_Boundaries(&projectile_list, screenWidth, screenHeight);
        Check_Collision_Projectiles_Rocket(&rocket, &projectile_list);
        int game_status = Check_Game_Status(&rocket, &projectile_list, &exes, &qtd_exes);

        //printf("%d\n", projectile_list.qtd_projectile);
        //---------------------------------------------------------------------------------------------------

        frame++;
        exes_animation(&frame, exes, qtd_exes);



        // Draw----------------------------------------------------------------------------------
        BeginDrawing();

            //placa--------------------------------------------------------------------------------------------
            // if(data_switches==1)ClearBackground(BLUE);
            // else if(data_switches==2)ClearBackground(GREEN);
            // else if(data_switches==3)ClearBackground(RED);
            // else if(data_switches==4)ClearBackground(PURPLE);
            // else if(data_switches==5)ClearBackground(BLACK);
            // else if(data_switches==6)ClearBackground(BROWN);
            // else if(data_switches==7)ClearBackground((Color){123,232,89,255});
            // else 
            //------------------------------------------------------------------------------------------------
            ClearBackground(RAYWHITE);
            DrawTexture(backgroung, 0, 0, WHITE);
            DrawTextureV(rocket.texture, (Vector2){rocket.position.x, rocket.position.y}, WHITE); //Casting pq rocket agora é um Rectangle e drawTexture só aceita Vector2D
            Draw_Exes(exes, qtd_exes);
            Draw_Projectiles(projectile_list);
            DrawText(TextFormat("Vida: %d", rocket.vida), 10, 10, 20, RED);

        EndDrawing();
        //--------------------------------------------------------------------------------------
        
        if(rocket.vida == 0) gameOver();
        else if(game_status) gameWin();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(exes);
    free(projectile_list.projectiles);
    //Unload_All_Textures(rocket, texture_projectiles, &texture_exes, qtd_texture_exes);
    UnloadTexture(backgroung);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
