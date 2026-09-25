#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#define CINZA (Color){211, 211, 211, 255}
#define larguraTela 1920
#define alturaTela 1080

typedef struct
{
    float posX;
    float posY;
    float raio;
    float velocidadeY;
} GameObject;

void inicializarRecursos(void);
void atualizarPersonagem(float dt, float v);
void desenharPersonagem(void);
void atualizarInimigo(float dt, float v);
void desenharInimigo(void);
void liberarRecursos(void);

GameObject personagem = {200.0f, 200.0f, 30.0f, 0.0f};
GameObject inimigo = {700.0f, 500.0f, 30.0f, 0.0f};

Texture2D TexFundo;
Texture2D personagemParado[2];
Texture2D animCorrendoProta[5];
Texture2D personagemPulando[4];
Texture2D inimigoParado[1];
Texture2D animCorrendoRobo[3];
Texture2D inimigoPulando[2];

float escalaPersonagem = 0.30f;
float escalaInimigo = 0.40f;
float larguraPadraoPersonagem;
float alturaPadraoPersonagem;
float larguraPadraoInimigo;
float alturaPadraoInimigo;

int framePersonagem = 0;
int frameInimigo = 0;
float tempoAnimacaoPersonagem = 0.0f;
float tempoAnimacaoInimigo = 0.0f;
float tempoAterrissagem = 0.0f;
float tempoAterrissagemInimigo = 0.0f;
float tempoPuloInicioPersonagem = 0.0f;
float jumpBufferPersonagem = 0.0f;
float coyoteTimerPersonagem = 0.0f;

bool olhandoParaEsquerda = false;
bool olhandoParaEsquerdaRobo = false;
bool protagonistaCorrendo = false;
bool roboCorrendo = false;
bool isGroundedPersonagem = false;
bool isGroundedInimigo = false;
bool estavaNoAr = false;
bool estavaNoArInimigo = false;

Rectangle piso = {-1000.0f, 770.0f, 25000.0f, 600.0f};

int main(void)
{
    InitWindow(larguraTela, alturaTela, "inicio");
    SetTargetFPS(60);
    inicializarRecursos();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float v = 500.0f * dt;

        if (tempoPuloInicioPersonagem > 0.0f) tempoPuloInicioPersonagem -= dt;

        if (IsKeyPressed(KEY_SPACE)) jumpBufferPersonagem = 0.12f;
        if (jumpBufferPersonagem > 0.0f)
        {
            jumpBufferPersonagem -= dt;
            if (jumpBufferPersonagem < 0.0f) jumpBufferPersonagem = 0.0f;
        }

        if (isGroundedPersonagem) coyoteTimerPersonagem = 0.10f;
        else
        {
            coyoteTimerPersonagem -= dt;
            if (coyoteTimerPersonagem < 0.0f) coyoteTimerPersonagem = 0.0f;
        }

        atualizarPersonagem(dt, v);
        atualizarInimigo(dt, v);

        BeginDrawing();
        ClearBackground(CINZA);
        DrawTexture(TexFundo, 0, 0, WHITE);
        desenharPersonagem();
        desenharInimigo();
        DrawText("Utilize as setas para mover e ESPACO para pular", 10, 10, 20, WHITE);
        EndDrawing();
    }

    liberarRecursos();
    CloseWindow();
    return 0;
}

void inicializarRecursos(void)
{
    Image img = LoadImage("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Fundo/FundoLab1.png");
    Color corChaoEscuro = (Color){57, 64, 73, 255};
    Color corChaoAmarelo = (Color){218, 181, 70, 255};
    ImageColorReplace(&img, corChaoEscuro, (Color){0, 0, 0, 0});
    ImageColorReplace(&img, corChaoAmarelo, (Color){0, 0, 0, 0});
    TexFundo = LoadTextureFromImage(img);
    UnloadImage(img);

    personagemParado[0] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/ParadoProta/AsDuasMaos.png");
    personagemParado[1] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/ParadoProta/UmaMao.png");

    animCorrendoProta[0] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/CorrendoProta/Run1.png");
    animCorrendoProta[1] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/CorrendoProta/Run2.png");
    animCorrendoProta[2] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/CorrendoProta/Run3.png");
    animCorrendoProta[3] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/CorrendoProta/Run4.png");
    animCorrendoProta[4] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/CorrendoProta/Run5.png");

    personagemPulando[0] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/ProtaPulando/Pulo1.png");
    personagemPulando[1] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/ProtaPulando/Pulo2.png");
    personagemPulando[2] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/ProtaPulando/Pulo3.png");
    personagemPulando[3] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/imagens Prota/ProtaPulando/PUlo4.png");

    inimigoParado[0] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Robo/RoboAndando/parado0.png");

    animCorrendoRobo[0] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Robo/RoboAndando/Run1.png");
    animCorrendoRobo[1] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Robo/RoboAndando/Run2.png");
    animCorrendoRobo[2] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Robo/RoboAndando/Run3.png");

    inimigoPulando[0] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Robo/RoboPulando/WhatsApp_Image_2026-09-24_at_22.15.34__5_-removebg-preview.png");
    inimigoPulando[1] = LoadTexture("/home/samuel/Área de trabalho/Projeto_Jogo/Jogo/Imagem Robo/RoboPulando/WhatsApp_Image_2026-09-24_at_22.15.34__6_-removebg-preview.png");

    larguraPadraoPersonagem = personagemParado[0].width * escalaPersonagem;
    alturaPadraoPersonagem = personagemParado[0].height * escalaPersonagem;
    larguraPadraoInimigo = inimigoParado[0].width * escalaInimigo;
    alturaPadraoInimigo = inimigoParado[0].height * escalaInimigo;
    inimigo.posY = piso.y - alturaPadraoInimigo + 89.0f;
}

void liberarRecursos(void)
{
    UnloadTexture(TexFundo);
    for (int i = 0; i < 2; i++) UnloadTexture(personagemParado[i]);
    for (int i = 0; i < 5; i++) UnloadTexture(animCorrendoProta[i]);
    for (int i = 0; i < 4; i++) UnloadTexture(personagemPulando[i]);
    for (int i = 0; i < 1; i++) UnloadTexture(inimigoParado[i]);
    for (int i = 0; i < 3; i++) UnloadTexture(animCorrendoRobo[i]);
    for (int i = 0; i < 2; i++) UnloadTexture(inimigoPulando[i]);
}
