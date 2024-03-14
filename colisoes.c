#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "colisoes.h"

Projectile_List Generate_Ex_Projectile(Ex ex, Projectile_List projectile_list, Texture2D Musical_Notes){

    Projectile new_projectile = {{ex.position.x+71, ex.position.y+ex.texture.height, 50, 70},{0,1},Musical_Notes}; //Orientação (0,1) indica que ele não anda na direção x, só na +y (pra baixo). 71 é metade da largura da foto e 160 é um pouco maior que a altura do ex

    projectile_list.projectiles = (Projectile *) realloc(projectile_list.projectiles, (projectile_list.qtd_projectile+1)*sizeof(Projectile));
    projectile_list.projectiles[projectile_list.qtd_projectile] = new_projectile;
    projectile_list.qtd_projectile++;
    return projectile_list;
}

Projectile_List Generate_Taylor_Projectile(Rocket rocket, Projectile_List projectile_list, Texture2D Musical_Notes){ //Sempre que apertar espaço essa função gera um projétil que vai para cima (0,-1) e adiciona no vetor de projéteis

    Projectile new_projectile = {{rocket.position.x+10, rocket.position.y-rocket.texture.height, 50, 70},{0,-1},Musical_Notes}; //Orientação (0,-1) indica que ele não anda na direção x, só na -y (pra cima)

    projectile_list.projectiles = (Projectile *) realloc(projectile_list.projectiles, (projectile_list.qtd_projectile+1)*sizeof(Projectile));
    projectile_list.projectiles[projectile_list.qtd_projectile] = new_projectile;
    projectile_list.qtd_projectile++;
    return projectile_list;
}



Projectile_List Projectile_Movement(Projectile_List projectile_list){ //Coordena os movimentos dos projéteis baseado na "orientation" que foi declarada nas funções Generate Projectile
    for(int i=0; i<projectile_list.qtd_projectile; i++){
        projectile_list.projectiles[i].position.x += 4*projectile_list.projectiles[i].orientation.x;
        projectile_list.projectiles[i].position.y += 4*projectile_list.projectiles[i].orientation.y;
    }
    return projectile_list;
}

void Draw_Projectiles(Projectile_List projectile_list){ //Escreve os projéteis na tela
    for(int i=0; i<projectile_list.qtd_projectile; i++){
        DrawTextureV(projectile_list.projectiles[i].texture, (Vector2){projectile_list.projectiles[i].position.x, projectile_list.projectiles[i].position.y}, WHITE);
    }
}

Ex *Generate_Exes(int nivel, int *qtd_exes, Texture2D textures_exes[]){ //Essa função inicializa os exes a depender da fase (a lógica disso na main ainda não tá feita :(
    Ex *exes = NULL;
    if(nivel==1){ //O vetor estático serve só pra ficar mais fácil de declarar a posição e a textura.
        *qtd_exes = 5;
        int a = *(qtd_exes);
        Ex static_exes[] = {{{100.0,100.0, 143, 150}, textures_exes[0]}, //A posição (x, y, largura e altura) e a textura dos exes
                             {{300.0,100.0, 143, 150}, textures_exes[0]},
                             {{500.0,100.0, 143, 150}, textures_exes[0]},
                             {{700.0,100.0, 143, 150}, textures_exes[0]},
                             {{900.0,100.0, 143, 150}, textures_exes[0]}}; 
        exes = (Ex*) malloc ((*qtd_exes)*sizeof(Ex));
        for(int i=0;i<(*qtd_exes);i++) exes[i] = static_exes[i]; //No final eu igual o vetor estático ao dinâmico (é dinâmico pq os exes vão ser mortos durante o jogo)
    }
    return exes;
}

void Draw_Exes(Ex* exes, int qtd_exes){ // Escreve os exes na tela
    for(int i=0;i<qtd_exes;i++){
        DrawTextureV(exes[i].texture, (Vector2){exes[i].position.x, exes[i].position.y}, WHITE);
    }
}


void Remove_Ex(int idx_delete, Ex **exes, int *qtd_exes){ //Remove um ex do vetor dinâmico de exes e passa pra uma posição anterior
    for(int i=idx_delete;i<(*qtd_exes)-1;i++){
        (*exes)[i] = (*exes)[i+1];
    }
    (*qtd_exes)--;
    (*exes) = (Ex *) realloc(*exes, (*qtd_exes)*sizeof(Ex));
}


void Remove_Projectile(int idx_delete, Projectile_List *projectile_list){ //Remove um projétil do vetor dinâmico de projéteis e passa pra uma posição anterior
    for(int i=idx_delete;i<(*projectile_list).qtd_projectile-1;i++){
        (*projectile_list).projectiles[i] = (*projectile_list).projectiles[i+1];
    }
    (*projectile_list).qtd_projectile--;
    (*projectile_list).projectiles = (Projectile *) realloc((*projectile_list).projectiles, ((*projectile_list).qtd_projectile)*sizeof(Projectile));
}

int Check_Projectiles_Boundaries(Projectile_List *projectile_list, int screenWidth, int screenHeight){ //Vai checar de um projétil passou dos limites da tela 
    for(int i=0;i<(*projectile_list).qtd_projectile;i++){
        if(((*projectile_list).projectiles[i].position.y<-70)||((*projectile_list).projectiles[i].position.y>1080)){
            Remove_Projectile(i, projectile_list);
            return 1;
        }
    }
    return 0;
}

int Check_Collision_Projectiles_Exes(Ex **exes, int *qtd_exes, Projectile_List *projectile_list){ //Checa a colisão entre um projétil e um ex
    for(int i=0;i<*qtd_exes;i++){
        for(int j=0;j<(*projectile_list).qtd_projectile;j++){
            if(CheckCollisionRecs((*exes)[i].position, (*projectile_list).projectiles[j].position)){
                Remove_Ex(i, exes, qtd_exes);
                Remove_Projectile(j, projectile_list);
                return 1;
            }
        }
    }
    return 0;
}

int Check_Collision_Projectiles_Rocket(Rocket *rocket, Projectile_List *projectile_list){ //Checa a colisão entre um projétil e a nave
    for(int j=0; j<(*projectile_list).qtd_projectile; j++){
        if(CheckCollisionRecs((*rocket).position, (*projectile_list).projectiles[j].position)){
            (*rocket).vida--;
            Remove_Projectile(j, projectile_list);
            
            return 1;
        }
    }
    return 0;
}

int Check_Game_Status(Rocket *rocket, Projectile_List *projectile_list, Ex **exes, int *qtd_exes){
    if((*rocket).vida == 0 || *qtd_exes == 0){
        rocket->position = (Rectangle){-1000,-1000,0,0}; //Se a vida for 0, a nave some da tela
        for(int i=0;i<(*projectile_list).qtd_projectile;i++) Remove_Projectile(i, projectile_list); //E todos os projéteis somem também (pra não ter um projétil que acertou a nave e ela sumiu mas o projétil continua lá
        for(int i=0;i<*qtd_exes;i++) Remove_Ex(i, exes, qtd_exes); //E todos os exes somem também
    }

    if(*qtd_exes == 0) return 1;
    else return 0;
}

void Unload_All_Textures(Rocket rocket, Texture2D texture_projectiles, Texture2D texture_exes[], int qtd_texture_exes){ //Dá unload em todas as texturas
    UnloadTexture(rocket.texture);
    UnloadTexture(texture_projectiles);
    for(int i=0;i<qtd_texture_exes;i++) UnloadTexture(texture_exes[i]);
}
