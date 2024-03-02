#ifndef COLISOES
#define COLISOES

#include "raylib.h"

typedef struct {
    Rectangle position;
    Texture2D texture;
    int vida;
} Rocket;

typedef struct {
    Rectangle position;
    Texture2D texture;
} Ex;

typedef struct {
    Rectangle position;
    Vector2 orientation;
    Texture2D texture;
} Projectile;

typedef struct {
    Projectile *projectiles;
    int qtd_projectile;
} Projectile_List;

Projectile_List Generate_Ex_Projectile(Ex ex, Projectile_List projectile_list, Texture2D Musical_Notes);
Projectile_List Generate_Taylor_Projectile(Rocket rocket, Projectile_List projectile_list, Texture2D Musical_Notes);
Projectile_List Projectile_Movement(Projectile_List projectile_list); //Coordena os movimentos dos projéteis baseado na "orientation" que foi declarada nas funções Generate Projectile


void Draw_Projectiles(Projectile_List projectile_list); //Escreve os projéteis na tela
Ex *Generate_Exes(int nivel, int *qtd_exes, Texture2D textures_exes[]); //Essa função inicializa os exes a depender da fase (a lógica disso na main ainda não tá feita :(
void Draw_Exes(Ex* exes, int qtd_exes); // Escreve os exes na tela
void Remove_Ex(int idx_delete, Ex **exes, int *qtd_exes); //Remove um ex do vetor dinâmico de exes e passa pra uma posição anterior
void Remove_Projectile(int idx_delete, Projectile_List *projectile_list); //Remove um projétil do vetor dinâmico de projéteis e passa pra uma posição anterior
int Check_Projectiles_Boundaries(Projectile_List *projectile_list, int screenWidth, int screenHeight); //Vai checar de um projétil passou dos limites da tela 
int Check_Collision_Projectiles_Exes(Ex **exes, int *qtd_exes, Projectile_List *projectile_list); //Checa a colisão entre um projétil e um ex


void Unload_All_Textures(Rocket rocket, Texture2D texture_projectiles, Texture2D texture_exes[], int qtd_texture_exes); //Dá unload em todas as texturas

#endif