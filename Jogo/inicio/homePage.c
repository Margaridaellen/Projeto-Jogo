#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#define AZUL_ESCURO (Color){10, 25, 38, 255}
#define larguraTela 1920
#define alturaTela 1080
#define movimento 10.0f

int main(void)
{

    InitWindow(larguraTela, alturaTela, "inicio");
    SetTargetFPS(60);

    // Posições iniciais
    float pos1 = 400.0f;
    float pos2 = 300.0f;
    float r = 25.0f;

    // Velocida em pixels
    float v_inicial = 500.0f;

    while (!WindowShouldClose())
    {
        // Calcular o deltatime(dt)
        float dt = GetFrameTime();
        float v = v_inicial * dt;

        if (IsKeyDown(KEY_RIGHT))
        {
            pos1 = pos1 + v; // direita
        }
        if (IsKeyDown(KEY_LEFT))
        {
            pos1 = pos1 - v; // esquerda
        }
        if (IsKeyDown(KEY_DOWN))
        {
            pos2 = pos2 + v; // para baixo
        }
        if (IsKeyDown(KEY_UP))
        {
            pos2 = pos2 - v; // para cima
        }

        BeginDrawing(); // O que deve aparecer na tela
        ClearBackground(AZUL_ESCURO);

        // Desenhar circulo (pos1 e pos2 são parâmetos que definem o centro do circulo e o r controla o tamanho)
        DrawCircle((int)pos1, (int)pos2, r, WHITE);

        // Escreve texto na tela, 10-distância da borda esquerda até o inicio do texto, 10-Topo da janela até o topo do texto, 20-Tamanho da fonte
        DrawText("Utilize as setas do teclado para se movimentar", 10, 10, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
