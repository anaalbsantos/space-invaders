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
	
    //placa--------------------------------------------------------------------------------------------
    // int fd, retval;
    // unsigned int data_button=0;
    // unsigned int data_switches=0;
    // fd = open("/dev/mydev", O_RDWR);

	// unsigned int rled = 0x00000000;
	// ioctl(fd, WR_RED_LEDS);
	// retval = write(fd, &rled, sizeof(rled));
	// printf("wrote %d bytes\n", retval);
    //------------------------------------------------------------------------------------------------
	
    
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1640;
    const int screenHeight = 924;

    int nivel = 1; //Indica o nível atual do jogo

    abrirMenu();
    float aux = GetTime();
    SetRandomSeed(time(NULL));

    //Setando os exes
    Ex *exes=NULL;
    int qtd_exes=0;
    int qtd_texture_exes = 11; //Quando tiver mais exes fora o joe, a gente aumenta isso daqui
    Texture2D texture_exes[qtd_texture_exes];
    texture_exes[0] = LoadTexture("assets/joe-sprite.png");
    for(int i=1; i<qtd_texture_exes; i++){
        texture_exes[i] = LoadTexture("assets/bixinho.png");
    } //Carrega as texturas dos exes

    exes = Generate_Exes(nivel, &qtd_exes, texture_exes);

    //Setando os Projéteis
    Projectile_List projectile_list = {NULL, 0}; //vetor de projéteis começa como NULL e tem 0 projéteis no mapa
    Texture texture_projectiles = LoadTexture("assets/nota-musical.png");

    //Setando o foguete/Taylor
    Rocket rocket;
    rocket.vida = 3;
    rocket.texture = LoadTexture("assets/rocket-pixel.png");
    rocket.position = (Rectangle){ (float)2*screenWidth/3, (float)2*screenHeight/3, rocket.texture.width, rocket.texture.height };
    
    //fundo do mapa
    Texture2D backgroung = LoadTexture("assets/background.png");
    
    //animação do alien
    int frame = 0;

    //placa--------------------------------------------------------------------------------------------
    // unsigned int data_qtdexes_vida;
	// unsigned int data_vida = decimal_to_display(rocket.vida, 4);
	// ioctl(fd, WR_R_DISPLAY);
	// retval = write(fd, &data_vida, sizeof(data_vida));
	//printf("wrote %d bytes\n", retval);
    //--------------------------------------------------------------------------------------------------

    SetTargetFPS(80);

    float intervalo = 5, tempo = 5; //Tempo pra uma nave atirar
    int invaders_direction = 1;
   	int total_exes = qtd_exes;
    int outro = 0;
    unsigned int score = 0, data_score = 0;
    int var = 0, pontuacao = 5;
    Color cor = {123,232,122,255};
    
    //variaveis de controle do personagem
    int *vetor, shot = 0;
    vetor = calloc(4, sizeof(int));


    // game loop
    while (!WindowShouldClose()){
        
        //placa--------------------------------------------------------------------------------------------
        // if(rocket.vida == 3) data = 0x40404030;
		// else if(rocket.vida == 2) data = 0x40404024;
		// else if(rocket.vida == 1) data = 0x40404079;
		// else data = 0x40404040;
    	
        // data_score = decimal_to_display(score,4);
		// ioctl(fd, WR_R_DISPLAY);
		// retval = write(fd, &data_score, sizeof(data_score));
		// ////printf("wrote %d bytes\n", retval);
		
		// ioctl(fd, RD_PBUTTONS);
		// read(fd, &data_button,1);
		// //printf("%d\n", data_button);
		// vetor = binary_transform(data_button);
		
		// ioctl(fd, RD_SWITCHES);
		// read(fd, &data_switches,1);
		// printf("%X\n", data_switches);
		
        // data_qtdexes_vida = decimal_to_display(rocket.vida,2);
        // data_qtdexes_vida = data_qtdexes_vida*pow(16, 4);
        // data_qtdexes_vida += decimal_to_display(qtd_exes, 2);  
		// ioctl(fd, WR_L_DISPLAY);
		// retval = write(fd, &data_qtdexes_vida, sizeof(data_qtdexes_vida));
		// //printf("wrote %d bytes\n", retval);

		// 	ioctl(fd, WR_RED_LEDS);
		// 	retval = write(fd, &rled, sizeof(rled));
		// 	//printf("wrote %d bytes\n", retval);
		// }
		//-------------------------------------------------------------------------------------------------
		
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
        //else if(game_status) gameWin();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(exes);
    free(projectile_list.projectiles);
    Unload_All_Textures(rocket, texture_projectiles, texture_exes, qtd_texture_exes);
    UnloadTexture(backgroung);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
