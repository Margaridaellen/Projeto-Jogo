#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#define larguraTela 1920
#define alturaTela 1080

int main(){
 InitWindow(larguraTela,alturaTela,"inicio");
 SetTargetFPS(60);

 while(!WindowShouldClose()){



    BeginDrawing();
    ClearBackground(BLUE);

        EndDrawing();
 }

CloseWindow();

    return 0;
}