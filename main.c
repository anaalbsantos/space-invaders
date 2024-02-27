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

Projectile_List Generate_Ex_Projectile(Ex ex, Projectile_List projectile_list, Texture2D Musical_Notes){

    Projectile new_projectile = {{ex.position.x+71, ex.position.y+160, 50, 70},{0,1},Musical_Notes}; //Orientação (0,1) indica que ele não anda na direção x, só na +y (pra baixo). 71 é metade da largura da foto e 160 é um pouco maior que a altura do ex

    projectile_list.projectiles = (Projectile *) realloc(projectile_list.projectiles, (projectile_list.qtd_projectile+1)*sizeof(Projectile));
    projectile_list.projectiles[projectile_list.qtd_projectile] = new_projectile;
    projectile_list.qtd_projectile++;
    return projectile_list;
}

Projectile_List Generate_Taylor_Projectile(Rocket rocket, Projectile_List projectile_list, Texture2D Musical_Notes){ //Sempre que apertar espaço essa função gera um projétil que vai para cima (0,-1) e adiciona no vetor de projéteis

    Projectile new_projectile = {{rocket.position.x+10, rocket.position.y-60, 50, 70},{0,-1},Musical_Notes}; //Orientação (0,-1) indica que ele não anda na direção x, só na -y (pra cima)

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
    Ex *exes=NULL;
    if(nivel==1){ //O vetor estático serve só pra ficar mais fácil de declarar a posição e a textura.
        *qtd_exes = 2;
        Ex static_exes[2] = {{{100.0,100.0, 143, 150}, textures_exes[0]}, //A posição (x, y, largura e altura) e a textura dos exes
                             {{300.0,100.0, 143, 150}, textures_exes[0]}}; 
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

void Unload_All_Textures(Rocket rocket, Texture2D texture_projectiles, Texture2D texture_exes[], int qtd_texture_exes){ //Dá unload em todas as texturas
    UnloadTexture(rocket.texture);
    UnloadTexture(texture_projectiles);
    for(int i=0;i<qtd_texture_exes;i++) UnloadTexture(texture_exes[i]);
}

int main() {
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    int nivel = 1; //Indica o nível atual do jogo

    InitWindow(screenWidth, screenHeight, "exes invaders");

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
    Vector2 rocket_pos = { (float)2*screenWidth/3, (float)2*screenHeight/3 };
    rocket.position.x = rocket_pos.x;
    rocket.position.y = rocket_pos.y;
    rocket.texture = LoadTexture("assets/rocket-pixel.png");


    float intervalo = 5, tempo = 5; //Tempo pra uma nave atirar
    int invaders_direction = 1;
    SetTargetFPS(80);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    //Rectangle teste = {0, 0, 50, 50}; //Testando
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // -------------------- Testes --------------------
        // if (IsKeyDown(KEY_RIGHT)) teste.x += 1.0f;
        // if (IsKeyDown(KEY_LEFT)) teste.x -= 1.0f;
        // if (IsKeyDown(KEY_UP)) teste.y -= 1.0f;
        // if (IsKeyDown(KEY_DOWN)) teste.y += 1.0f;
        // printf("x = %.2f || y = %.2f\n", teste.x, teste.y);
        // ------------------------------------------------


        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) rocket.position.x += 4.0f;
        if (IsKeyDown(KEY_LEFT)) rocket.position.x -= 4.0f;
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


        //ATENÇão!!!!! Aninha eu mudei essa parte aqui pra tirar essa segunda avaliação, mas não sei se isso pode ter alterado alguma coisa
            //Versão Anterior
        // if(invaders_direction && joePosition.x < screenWidth-300) joePosition.x += 2.0f;
        // else if(!invaders_direction && joePosition.x > 100) joePosition.x -= 2.0f;
            //Versão Posterior
        for(int i=0;i<qtd_exes;i++){
            if(invaders_direction) exes[i].position.x += 2.0f;
            else if(!invaders_direction) exes[i].position.x -= 2.0f;
        }
        //----------------------------------------------------------------------------------

        projectile_list = Projectile_Movement(projectile_list);
        

        //Essa variável collision não faz nada mas eu imaginei que as check_collision podiam exigir que um valor inteiro fosse retornado em alguma outra parta do código (sim, isso é uma gambiarra)
        int collision = Check_Collision_Projectiles_Exes(&exes, &qtd_exes, &projectile_list);
        collision = Check_Projectiles_Boundaries(&projectile_list, screenWidth, screenHeight);

        printf("%d\n", projectile_list.qtd_projectile);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            //DrawRectangleRec(teste, BLUE);
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
