#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "colisoes.h"
#include "menu.h"


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

int digit_display(int num){
    switch(num){
        case 0: return 0x40;
        case 1: return 0x79;
        case 2: return 0x24;
        case 3: return 0x30;
        case 4: return 0x19;
        case 5: return 0x12;
        case 6: return 0x02;
        case 7: return 0x78;
        case 8: return 0x00;
        case 9: return 0x18;
        default: return 0xFF;
    }
}

unsigned int decimal_to_display(int numero, int casas){
    casas = casas-1;
    unsigned int hex_resultado = 0, pow_hex=casas, casasAdicionais;
    int digito;
    int potencia_10 = pow(10,casas);
    int primeiroSignificativo = 0;
    while(potencia_10>0){
       /*if(primeiroSignificativo==0){ //Para o caso de números menor que o núemro de casas, os displays à esquerda ficarão apagados. Em vez de mostrar no display "0003" vai ficar "   3"
            if(numero<potencia_10) digito = -1;
            else {
                primeiroSignificativo = 1;
                digito = numero/potencia_10;
            }
        }
        else */ 
        digito = numero/potencia_10;

        casasAdicionais = pow(16, pow_hex); //Pular os primerios digitos
        hex_resultado += pow(casasAdicionais,2) * digit_display(digito);
        pow_hex--;

        numero=numero%potencia_10;
        potencia_10=potencia_10/10;
    }
    return hex_resultado;
}

int *binary_transform(int numero){
	int *vetor;
	vetor = malloc(sizeof(int)*4);
	for(int i = 0; i < 4; i++){
		if(numero%2 == 1) vetor[3-i] = 1;
		else vetor[3-i] = 0;
		numero /= 2;
	}
	return vetor;
}


int main() {
	
    int fd, retval;
    unsigned int data_button=0;
    unsigned int data_switches=0;
    fd = open("/dev/mydev", O_RDWR);

	unsigned int rled = 0x00000000;
	ioctl(fd, WR_RED_LEDS);
	retval = write(fd, &rled, sizeof(rled));
	printf("wrote %d bytes\n", retval);
	
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1640;
    const int screenHeight = 924;

    int nivel = 1; //Indica o nível atual do jogo

    abrirMenu();
    float aux= GetTime();
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
    
    unsigned int data_qtdexes_vida;
	unsigned int data_vida = decimal_to_display(rocket.vida, 4);
	ioctl(fd, WR_R_DISPLAY);
	retval = write(fd, &data_vida, sizeof(data_vida));
	//printf("wrote %d bytes\n", retval);


    float intervalo = 5, tempo = 5; //Tempo pra uma nave atirar
    int invaders_direction = 1;
    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    
	
	

    //--------------------------------------------------------------------------------------
   	int total_exes = qtd_exes;
    int sla = 0;
    int outro = 0;
    unsigned int score = 0, data_score = 0;
    int var = 0, pontuacao = 5;
    Color cor = {123,232,122,255};
    
    //variaveis de controle do personagem
    int *vetor, shot = 0;
    vetor = calloc(4, sizeof(int));
    // game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		sla++;
		// if(rocket.vida == 3) data = 0x40404030;
		// else if(rocket.vida == 2) data = 0x40404024;
		// else if(rocket.vida == 1) data = 0x40404079;
		// else data = 0x40404040;
        
    	
        data_score = decimal_to_display(score,4);
		ioctl(fd, WR_R_DISPLAY);
		retval = write(fd, &data_score, sizeof(data_score));
		////printf("wrote %d bytes\n", retval);
		
		ioctl(fd, RD_PBUTTONS);
		read(fd, &data_button,1);
		//printf("%d\n", data_button);
		vetor = binary_transform(data_button);
		
		ioctl(fd, RD_SWITCHES);
		read(fd, &data_switches,1);
		printf("%X\n", data_switches);
		
        data_qtdexes_vida = decimal_to_display(rocket.vida,2);
        data_qtdexes_vida = data_qtdexes_vida*pow(16, 4);
        data_qtdexes_vida += decimal_to_display(qtd_exes, 2);  
		ioctl(fd, WR_L_DISPLAY);
		retval = write(fd, &data_qtdexes_vida, sizeof(data_qtdexes_vida));
		//printf("wrote %d bytes\n", retval);

		if(sla % 20 == 0){
			if(outro == 0){
				rled = 0x00000000;
				outro = 1;
			}
			else{
				rled = 0xFFFFFFFF;
				outro = 0;
			}
			ioctl(fd, WR_RED_LEDS);
			retval = write(fd, &rled, sizeof(rled));
			//printf("wrote %d bytes\n", retval);
		}
		
		
        // Update
        //----------------------------------------------------------------------------------
        if (!vetor[3] && rocket.position.x < screenWidth - rocket.texture.width) rocket.position.x += 4.0f;
        if (!vetor[0] && rocket.position.x > 0) rocket.position.x -= 4.0f;
        if (!vetor[1] && !shot) {
            projectile_list = Generate_Taylor_Projectile(rocket, projectile_list, texture_projectiles);
            shot = 1;

           // printf("%d\n", projectile_list.qtd_projectile);
        }
        if(vetor[1] && shot) shot = 0;

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
            if(invaders_direction) exes[i].position.x += 2.0f;
            else if(!invaders_direction) exes[i].position.x -= 2.0f;
        }
        //----------------------------------------------------------------------------------

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
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            
            if(data_switches==1)ClearBackground(BLUE);
            else if(data_switches==2)ClearBackground(GREEN);
            else if(data_switches==3)ClearBackground(RED);
            else if(data_switches==4)ClearBackground(PURPLE);
            else if(data_switches==5)ClearBackground(BLACK);
            else if(data_switches==6)ClearBackground(BROWN);
            else if(data_switches==7)ClearBackground((Color){123,232,89,255});
            else ClearBackground(RAYWHITE);
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
        if(rocket.vida==0) gameOver();
        //if(qtd_exes==0) gameWin();
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
