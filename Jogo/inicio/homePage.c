#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#define AZUL_ESCURO (Color){10, 25, 38, 255}
#define larguraTela 1920
#define alturaTela 1080
#define movimento 10.0f

int Personagem(void)
{
    InitWindow(larguraTela, alturaTela, "inicio");
    SetTargetFPS(60);

    float pos1 = 400.0f;
    float pos2 = 300.0f;
    float r = 25.0f;
    float pos3 = 800.0f;
    float pos4 = 300.0f;
    float r2 = 25.0f;

    float v_inicial = 500.0f;
    float v_inicialVilao = 500.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float v = v_inicial * dt;
        float v2 = v_inicialVilao * dt;

        if (IsKeyDown(KEY_RIGHT))
            pos1 += v;
        if (IsKeyDown(KEY_LEFT))
            pos1 -= v;
        if (IsKeyDown(KEY_DOWN))
            pos2 += v;
        if (IsKeyDown(KEY_UP))
            pos2 -= v;

        if (IsKeyDown(KEY_D))
            pos3 += v2;
        if (IsKeyDown(KEY_A))
            pos3 -= v2;
        if (IsKeyDown(KEY_S))
            pos4 += v2;
        if (IsKeyDown(KEY_W))
            pos4 -= v2;

        BeginDrawing();
        ClearBackground(AZUL_ESCURO);

        DrawCircle((int)pos1, (int)pos2, r, WHITE);
        DrawCircle((int)pos3, (int)pos4, r2, WHITE);

        DrawText("Utilize as setas do teclado para se movimentar", 10, 10, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int main(void)
{
    return Personagem();
}

