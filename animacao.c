#include "animacao.h"
// #include "colisoes.h"

void exes_animation(int *frame, Ex *exes, int qtd_exes){
    int frame_speed = 3;

    if(*frame >= 30/frame_speed){
        *frame = 0;

        for(int i=0; i<qtd_exes; i++){
            if(exes[i].animation.x == 0) exes[i].animation.x = exes[i].animation.width;
            else exes[i].animation.x = 0;
        }
    }
}
