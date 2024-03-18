#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "colisoes.h"
#include "menu.h"
#include "animacao.h"
#include "driver.h"

#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */

// ioctl commands defined for the pci driver header
#include "ioctl_cmds.h"

#include<stdio.h>
#include<math.h>

#define tam_switch 18
#define tam_button 4

int main() {
	   
	   //placa--------------------------------------------------------------------------------------------
     int fd, retval;
     unsigned int data_button=0;
     unsigned int data_switches=0;
     fd = open("/dev/mydev", O_RDWR);

	 unsigned int rled = 0x00000000;
	 ioctl(fd, WR_RED_LEDS);
	 retval = write(fd, &rled, sizeof(rled));
    //------------------------------------------------------------------------------------------------
	
    
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 768;
    int *vetor_switch = NULL;
	
		
    int nivel; //Indica o nível atual do jogo
    nivel = abrirMenu(fd);
    
    /*while(1){
    	ioctl(fd, RD_SWITCHES);
	read(fd, &data_switches,1);
	vetor_switch = binary_transform(data_switches, 18);
	
	for(int i = 0; i < 18; i++)
		printf("%d ", vetor_switch[i]);
		
		printf("\n");
    }*/
    
    float aux = GetTime();
    SetRandomSeed(time(NULL));
	
    int resetar = 0;
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
    int *vetor_button = NULL;
    int shot;

    Texture2D texture_exes[11];
    
    //variaveis de controle do personagem
 	
 	//placa--------------------------------------------------------------------------------------------
     unsigned int data_qtdexes_vida;
	 unsigned int data_vida = decimal_to_display(rocket.vida, 4);
	 ioctl(fd, WR_R_DISPLAY);
	 retval = write(fd, &data_vida, sizeof(data_vida));
    //--------------------------------------------------------------------------------------------------
    
    Generate_Level(nivel, &rocket, &backgroung, &cor, &projectile_list, &texture_projectiles, &qtd_exes, texture_exes, &exes, &qtd_texture_exes, &intervalo, &tempo, &invaders_direction, &total_exes, &outro, &score, &data_score, &var, &pontuacao, &vetor_button, &shot, &frame);

    // game loop
    while (!WindowShouldClose()){
		
        //
        //placa--------------------------------------------------------------------------------------------
         
        data_score = decimal_to_display(score, 4);
		ioctl(fd, WR_R_DISPLAY);
		retval = write(fd, &data_score, sizeof(data_score));
		
		ioctl(fd, RD_PBUTTONS);
		read(fd, &data_button,1);
		vetor_button = binary_transform(data_button, 4);
		
		ioctl(fd, RD_SWITCHES);
		read(fd, &data_switches,1);
		
        data_qtdexes_vida = decimal_to_display(rocket.vida,2);
        data_qtdexes_vida = data_qtdexes_vida*pow(16, 4);
        data_qtdexes_vida += decimal_to_display(qtd_exes, 2);  
		ioctl(fd, WR_L_DISPLAY);
		retval = write(fd, &data_qtdexes_vida, sizeof(data_qtdexes_vida));
		ioctl(fd, WR_RED_LEDS);
		 	retval = write(fd, &rled, sizeof(rled));
	
		//------------------------------------------------------------------------------------------------- Update---------------------------------------------------------------------------------------------------
        if (vetor_button[3] == 0 && rocket.position.x < screenWidth - rocket.texture.width) rocket.position.x += 4.0f;
        if (vetor_button[2] == 0 && rocket.position.x > 0) rocket.position.x -= 4.0f;
        if (vetor_button[0] == 0 && !shot) {
            projectile_list = Generate_Taylor_Projectile(rocket, projectile_list, texture_projectiles);
            shot = 1;

           // printf("%d\n", projectile_list.qtd_projectile);
        }
        if(vetor_button[0] == 1 && shot) shot = 0;

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
             if(data_switches==1)ClearBackground(BLUE);
             else if(data_switches==2)ClearBackground(GREEN);
             else if(data_switches==3)ClearBackground(RED);
             else if(data_switches==4)ClearBackground(PURPLE);
             else if(data_switches==5)ClearBackground(BLACK);
             else if(data_switches==6)ClearBackground(BROWN);
             else if(data_switches==7)ClearBackground((Color){123,232,89,255});
             
             //else ClearBackground(RAYWHITE);
            //------------------------------------------------------------------------------------------------
            
            DrawTexture(backgroung, 0, 0, WHITE);
            DrawTextureV(rocket.texture, (Vector2){rocket.position.x, rocket.position.y}, WHITE); //Casting pq rocket agora é um Rectangle e drawTexture só aceita Vector2D
            Draw_Exes(exes, qtd_exes);
            Draw_Projectiles(projectile_list);
            DrawText(TextFormat("Vida: %d", rocket.vida), 10, 10, 20, RED);

        EndDrawing();
        //--------------------------------------------------------------------------------------
        
        if(rocket.vida == 0) resetar = gameOver(fd);
        else if(game_status) resetar = gameWin(fd);

        if(resetar){
            aux = GetTime();
            Generate_Level(nivel, &rocket, &backgroung, &cor, &projectile_list, &texture_projectiles, &qtd_exes, texture_exes, &exes, &qtd_texture_exes, &intervalo, &tempo, &invaders_direction, &total_exes, &outro, &score, &data_score, &var, &pontuacao, &vetor_button, &shot, &frame);
            resetar = 0;
        }
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
