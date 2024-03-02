#ifndef MENU
#define MENU

#include "menu.h"
#include "raylib.h"

int abrirMenu();
void abrirGameOver(int **morreu, Sound **Tema);
void menuVitoria(int time, int* venceu, Sound* Tema);

#endif