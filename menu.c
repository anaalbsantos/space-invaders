#include "menu.h"
#include "raylib.h"
#include "colisoes.h"
#include "driver.h"
#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */
#include "ioctl_cmds.h"
#include <stdio.h>

int abrirMenu(int fd){
    
    const int screen_width = 1024;
    const int screen_height = 768;
    int bloqueador = 0;
    int nivel = 0;
    unsigned int data_switches=0;
    

    InitWindow(screen_width, screen_height , "exes invaders");

    Texture2D menu = LoadTexture("./assets/Menu.png");        // Carrega as texturas a partir de imagens da galeria
    Texture2D tutorial = LoadTexture("./assets/Tutorial.png");
    SetTargetFPS(30);
    
    int *vetor_switch = NULL;
    int flag = 0;
    ioctl(fd, RD_SWITCHES);
	read(fd, &data_switches,1);
	vetor_switch = binary_transform(data_switches, 18);
		
    while(flag == 0){
		
		ioctl(fd, RD_SWITCHES);
		read(fd, &data_switches,1);
		vetor_switch = binary_transform(data_switches, 18);
		
        BeginDrawing();
        
        if(bloqueador == 0)
            DrawTexture(menu, screen_width/2 - menu.width/2, screen_height/2 - menu.height/2, WHITE); //desenha o mapa no centro da tela);

        if(IsKeyDown(KEY_H)){
            bloqueador = 1;
            DrawTexture(tutorial, screen_width/2 - tutorial.width/2, screen_height/2 - tutorial.height/2, WHITE); //desenha o mapa no centro da tela);
        }
		
		EndDrawing();
        
        if(vetor_switch[17]){
            nivel = 1;
            flag = 1;
            }
        

        if(vetor_switch[16]){
            nivel = 2;
            flag = 1;
            }
        

        if(vetor_switch[15]){
            nivel = 3;
            flag = 1;
            }
        
        
        if(vetor_switch[13]){
        	nivel = 4;
        	flag = 1;
        	}
        	
        if(IsKeyDown(KEY_ESCAPE))
        	CloseWindow();

    }
    return nivel;
}




int gameOver(int fd){

    Texture2D GameOver = LoadTexture("./assets/Gameover.png");
    int resetar = 0;
    unsigned int data_switches=0;
	
	int *vetor_switch = NULL;
    ioctl(fd, RD_SWITCHES);
	read(fd, &data_switches,1);
	vetor_switch = binary_transform(data_switches, 18);
	int flag = 0;
	
    while(flag == 0){
    
    	ioctl(fd, RD_SWITCHES);
		read(fd, &data_switches,1);
		vetor_switch = binary_transform(data_switches, 18);
	
        BeginDrawing();
        DrawTexture(GameOver, 0, 0, WHITE);
        EndDrawing();

    if(vetor_switch[14]){
        resetar = 2;
        flag = 1;
    }
    
    if(vetor_switch[13]){
        resetar = 1;
        flag = 1;
       }
	
    }
    return resetar;
}

int gameWin(int fd){
        
        Texture2D Vitoria = LoadTexture("./assets/Win.png");
        int resetar = 0;
        unsigned int data_switches=0;
        
        int *vetor_switch = NULL;
    	ioctl(fd, RD_SWITCHES);
		read(fd, &data_switches,1);
		vetor_switch = binary_transform(data_switches, 18);
		int flag = 0;

        while(flag == 0){
        
        	ioctl(fd, RD_SWITCHES);
			read(fd, &data_switches,1);
			vetor_switch = binary_transform(data_switches, 18);
	
            BeginDrawing();
            DrawTexture(Vitoria, 0, 0, WHITE);
            EndDrawing();

        if(vetor_switch[14]){
            resetar = 2;
            flag = 1;
        }

        if(vetor_switch[13]){
            resetar = 1;
            flag = 1;
        }
	}
	
    return resetar;
}
