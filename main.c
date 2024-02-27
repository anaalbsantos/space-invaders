#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 

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


Projectile_List Generate_Taylor_Projectile(Rocket rocket, Projectile_List projectile_list, Texture2D Musical_Notes){

    Projectile new_projectile = {{rocket.position.x+10, rocket.position.y-60, 10, 70},{0,-1},Musical_Notes};

    projectile_list.projectiles = (Projectile *) realloc(projectile_list.projectiles, (projectile_list.qtd_projectile+1)*sizeof(Projectile));
    projectile_list.projectiles[projectile_list.qtd_projectile] = new_projectile;
    projectile_list.qtd_projectile++;
    return projectile_list;
}

Projectile_List Projectile_Movement(Projectile_List projectile_list){
    for(int i=0; i<projectile_list.qtd_projectile; i++){
        projectile_list.projectiles[i].position.x += 4*projectile_list.projectiles[i].orientation.x;
        projectile_list.projectiles[i].position.y += 4*projectile_list.projectiles[i].orientation.y;
    }
    return projectile_list;
}

void Draw_Projectiles(Projectile_List projectile_list){
    for(int i=0; i<projectile_list.qtd_projectile; i++){
        DrawTextureV(projectile_list.projectiles[i].texture, (Vector2){projectile_list.projectiles[i].position.x, projectile_list.projectiles[i].position.y}, WHITE);
    }
}

Ex *Generate_Exes(int nivel, int *qtd_exes, Texture2D textures_exes[]){
    Ex *exes=NULL;
    if(nivel==1){ //O vetor estático serve só pra ficar mais fácil de declarar a posição e a textura.
        Ex static_exes[2] = {{{100.0,100.0, 50, 50}, textures_exes[0]}, //A posição (x, y, altura e largura) e a textura dos exes
                             {{300.0,100.0, 50, 50}, textures_exes[0]}}; 
        exes = (Ex*) malloc (2*sizeof(Ex));
        for(int i=0;i<2;i++) exes[i] = static_exes[i]; //No final eu igual o vetor estático ao dinâmico (é dinâmico pq os exes vão ser mortos durante o jogo)
        *qtd_exes = 2;
    }
    return exes;
}

void Draw_Exes(Ex* exes, int qtd_exes){
    for(int i=0;i<qtd_exes;i++){
        DrawTextureV(exes[i].texture, (Vector2){exes[i].position.x, exes[i].position.y}, WHITE);
    }
}

void Unload_All_Textures(Rocket rocket, Texture2D texture_projectiles, Texture2D texture_exes[], int qtd_texture_exes){
    UnloadTexture(rocket.texture);
    UnloadTexture(texture_projectiles);
    for(int i=0;i<qtd_texture_exes;i++) UnloadTexture(texture_exes[i]);
}

void Remove_Ex(int idx_delete, Ex **exes, int *qtd_exes){
    for(int i=idx_delete;i<(*qtd_exes)-1;i++){
        (*exes)[i] = (*exes)[i+1];
    }
    (*qtd_exes)--;
    (*exes) = (Ex *) realloc(*exes, (*qtd_exes)*sizeof(Ex));
}

void Remove_Projectile(int idx_delete, Projectile_List *projectile_list){
    for(int i=idx_delete;i<(*projectile_list).qtd_projectile-1;i++){
        (*projectile_list).projectiles[i] = (*projectile_list).projectiles[i+1];
    }
    (*projectile_list).qtd_projectile--;
    (*projectile_list).projectiles = (Projectile *) realloc((*projectile_list).projectiles, ((*projectile_list).qtd_projectile)*sizeof(Projectile));
}

int Check_Collision(Ex **exes, int *qtd_exes, Projectile_List *projectile_list){
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

int main() {
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    int nivel = 1; //Indica o nível atual do jogo

    printf("%d %d\n", screenHeight, screenHeight);

    InitWindow(screenWidth, screenHeight, "exes invaders");

    Ex *exes=NULL;
    int qtd_exes=0;
    Projectile_List projectile_list = {NULL, 0}; //vetor de projéteis começa como NULL e tem 0 projéteis no mapa

    Texture texture_projectiles = LoadTexture("assets/Musical_Note.png");

    Rocket rocket;
    rocket.vida = 3;
    Vector2 rocket_pos = { (float)2*screenWidth/3, (float)2*screenHeight/3 };
    rocket.position.x = rocket_pos.x;
    rocket.position.y = rocket_pos.y;
    rocket.texture = LoadTexture("assets/rocket-pixel.png");

    int qtd_texture_exes = 1; //Quando tiver mais exes fora o joe, a gente aumenta isso daqui
    Texture2D texture_exes[qtd_texture_exes]; 
    texture_exes[0] = LoadTexture("assets/joe-pixel.png");

    exes = Generate_Exes(nivel, &qtd_exes, texture_exes);
    int invaders_direction = 1;

    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) rocket.position.x += 4.0f;
        if (IsKeyDown(KEY_LEFT)) rocket.position.x -= 4.0f;
        if (IsKeyPressed(KEY_SPACE)) {
            projectile_list = Generate_Taylor_Projectile(rocket, projectile_list, texture_projectiles);
            printf("%d\n", projectile_list.qtd_projectile);
        }

        for(int i=0;i<qtd_exes;i++){
            if(exes[i].position.x == screenWidth-300) invaders_direction = 0;
            else if(exes[i].position.x == 100) invaders_direction = 1;
        }

        // if(invaders_direction && joePosition.x < screenWidth-300) joePosition.x += 2.0f;
        // else if(!invaders_direction && joePosition.x > 100) joePosition.x -= 2.0f;
        for(int i=0;i<qtd_exes;i++){
            if(invaders_direction) exes[i].position.x += 2.0f;
            else if(!invaders_direction) exes[i].position.x -= 2.0f;
        }
        //----------------------------------------------------------------------------------

        projectile_list = Projectile_Movement(projectile_list);
        int collision = Check_Collision(&exes, &qtd_exes, &projectile_list);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(rocket.texture, (Vector2){rocket.position.x, rocket.position.y}, WHITE); //Casting pq rocket agora é um Rectangle e drawTexture só aceita Vector2D
            Draw_Exes(exes, qtd_exes);
            Draw_Projectiles(projectile_list);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    Unload_All_Textures(rocket, texture_projectiles, texture_exes, qtd_texture_exes);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
