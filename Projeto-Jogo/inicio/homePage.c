#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#define AZUL_ESCURO (Color){10, 25, 38, 255}
#define larguraTela 1920
#define alturaTela 1080

int main()
{
    InitWindow(larguraTela, alturaTela, "inicio");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(AZUL_ESCURO);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}