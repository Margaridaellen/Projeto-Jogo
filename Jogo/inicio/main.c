#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include "jogo.h"
//Define a cor de fundo antes da imagem como "Padrão"
#define CINZA (Color){211, 211, 211, 255}
// O formato da tela em pixels.
#define larguraTela 1480
#define alturaTela 720

static char caminhoArquivo[1024];

//O "const" significa que quem recebe esse ponteiro não deve modificar o texto por meio dele.
//ele literalmente serve para indicar que um valor não deve ser modificado por aquele acesso,
//nesse caso um ponteiro.

//O "resolverCaminhoDoAsset" é somente o nome da função que é um ponteiro, para montar o caminho 
//que o programa vai usar para localizar um asset, como uma imagem.

// O parâmetro caminhoRelativo recebe o caminho do asset(A imagem) como texto.
// Esse caminho é relativo ao diretório de trabalho atual do jogo.
// "const" indica que a função não deve modificar o texto recebido.
// A função usará esse caminho para montar o caminho completo do arquivo.
//Resumindo: O *caminhoRelativo vai receber o mainho da imagem dentroda função.

//GetWorkingDirectory(); diz apartir de qua pasta procurar.

//"const char *base = GetWorkingDirectory();" Pede à Raylib o diretório atual de execução e guarda o resultado em base.
//Ou seja, vai ver o que tem dentro das pastas e guarda dentro da variavel "base".

//"if (base == NULL || base[0] == '\0') base = ".";"
//Verifica se a Raylib não retornou um diretório válido: NULL ou texto vazio.
// Nesse caso, usa ".", que significa “diretório atual”.
//Para evitar o famoso "Vazamento de memoria", substituindo o valor nulo ou zerado em um ".".

//snprintf(...)
//Monta o caminho final e o escreve no vetor global 

//O sizeof(caminhoArquivo) informa ao snprintf quantos bytes cabem no vetor caminhoArquivo (neste caso, 1024)
//. Assim, ele não escreve além do espaço reservado.

//%s/%s significa: juntar o conteúdo de base, dorma o caminho completo incluindo as "/"

//return caminhoArquivo; No final fala para o jogo: "è esse arquivo aqui que você precisa".


static const char *resolverCaminhoDoAsset(const char *caminhoRelativo)
{
    const char *base = GetWorkingDirectory();
    if (base == NULL || base[0] == '\0') base = ".";
    snprintf(caminhoArquivo, sizeof(caminhoArquivo), "%s/%s", base, caminhoRelativo);
    return caminhoArquivo;
}
//Cria onde os personagens irão expalnar, com as posições x e y, raio e sua velocidade.
GameObject personagem = {1300.0f, 200.0f, 30.0f, 2.0f};
GameObject inimigo = {700.0f, 500.0f, 30.0f, 0.0f};
//Adiciona as variaveis para serem visual dentro dos parametros do Raylib
Texture2D TexFundo;
Texture2D personagemParado[2];
Texture2D animCorrendoProta[5];
Texture2D personagemPulando[4];
Texture2D inimigoParado[1];
Texture2D animCorrendoRobo[3];
Texture2D inimigoPulando[2];
//Cria o tamanho dos personagens, Primeiro pelo raio, e depois a largura e altura da imagem png.
float escalaPersonagem = 0.30f;
float escalaInimigo = 0.40f;
float larguraPadraoPersonagem;
float alturaPadraoPersonagem;
float larguraPadraoInimigo;
float alturaPadraoInimigo;
//Os frames da animação por png iniciadas como 0.
int framePersonagem = 0;
int frameInimigo = 0;
float tempoAnimacaoPersonagem = 0.0f;
float tempoAnimacaoInimigo = 0.0f;
float tempoAterrissagem = 0.0f;
float tempoAterrissagemInimigo = 0.0f;
float tempoPuloInicioPersonagem = 0.0f;
float jumpBufferPersonagem = 0.0f;
float coyoteTimerPersonagem = 0.0f;
//Declara inicialmente as funções como falsas (permanecendo elas normais)
bool olhandoParaEsquerda = false;
bool olhandoParaEsquerdaRobo = false;
bool protagonistaCorrendo = false;
bool roboCorrendo = false;
bool isGroundedPersonagem = false;
bool isGroundedInimigo = false;
bool estavaNoAr = false;
bool estavaNoArInimigo = false;
//Cria o formato, coordenada x e y, largura e altura do piso.
Rectangle piso = {-1000.0f, 770.0f, 25000.0f, 600.0f};
// A função prepara os recursos gráficos antes do loop principal.
// Ela carrega as imagens, remove cores do fundo e calcula as dimensões dos personagens.
void inicializarRecursos(void)
{
    Image img = LoadImage(resolverCaminhoDoAsset("Jogo/Imagem Fundo/FundoLab1.png"));
    Color corChaoEscuro = (Color){57, 64, 73, 255};
    Color corChaoAmarelo = (Color){218, 181, 70, 255};
    ImageColorReplace(&img, corChaoEscuro, (Color){0, 0, 0, 0});
    ImageColorReplace(&img, corChaoAmarelo, (Color){0, 0, 0, 0});
    TexFundo = LoadTextureFromImage(img);
    UnloadImage(img);

    personagemParado[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/ParadoProta/AsDuasMaos.png"));
    personagemParado[1] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/ParadoProta/UmaMao.png"));

    animCorrendoProta[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/CorrendoProta/Run1.png"));
    animCorrendoProta[1] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/CorrendoProta/Run2.png"));
    animCorrendoProta[2] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/CorrendoProta/Run3.png"));
    animCorrendoProta[3] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/CorrendoProta/Run4.png"));
    animCorrendoProta[4] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/CorrendoProta/Run5.png"));

    personagemPulando[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/ProtaPulando/Pulo1.png"));
    personagemPulando[1] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/ProtaPulando/Pulo2.png"));
    personagemPulando[2] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/ProtaPulando/Pulo3.png"));
    personagemPulando[3] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens Prota/ProtaPulando/PUlo4.png"));

    inimigoParado[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/Imagem Robo/RoboAndando/parado0.png"));

    animCorrendoRobo[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/Imagem Robo/RoboAndando/Run1.png"));
    animCorrendoRobo[1] = LoadTexture(resolverCaminhoDoAsset("Jogo/Imagem Robo/RoboAndando/Run2.png"));
    animCorrendoRobo[2] = LoadTexture(resolverCaminhoDoAsset("Jogo/Imagem Robo/RoboAndando/Run3.png"));

    inimigoPulando[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/Imagem Robo/RoboPulando/WhatsApp_Image_2026-09-24_at_22.15.34__5_-removebg-preview.png"));
    inimigoPulando[1] = LoadTexture(resolverCaminhoDoAsset("Jogo/Imagem Robo/RoboPulando/WhatsApp_Image_2026-09-24_at_22.15.34__6_-removebg-preview.png"));

    larguraPadraoPersonagem = personagemParado[0].width * escalaPersonagem;
    alturaPadraoPersonagem = personagemParado[0].height * escalaPersonagem;
    larguraPadraoInimigo = inimigoParado[0].width * escalaInimigo;
    alturaPadraoInimigo = inimigoParado[0].height * escalaInimigo;
    personagem.posY = piso.y - alturaPadraoPersonagem;
    isGroundedPersonagem = true;
    inimigo.posY = piso.y - alturaPadraoInimigo + 89.0f;
}
//Cria os loops dos vetores de animação.
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
//int main(void) é o ponto de entrada do programa: é por ela que a execução começa. 
//O void indica que a função não recebe argumentos, e o int indica que ela retorna um código de resultado ao sistema operacional.
//Ou seja é o ponto de entrada do programa: inicia e controla o ciclo principal do jogo.
//Não sei por que eu coloquei uma das condições de pulo do personagem principal ai...
int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(larguraTela, alturaTela, "inicio");
    MaximizeWindow();
    SetTargetFPS(60);
    Camera2D camera = {0};
    camera.zoom = (float)larguraTela / 1920.0f;
    inicializarRecursos();

    while (!WindowShouldClose())
    {
        // dt representa o tempo desde o último quadro e deixa o movimento independente do FPS.
        float dt = GetFrameTime();
        float v = 500.0f * dt;

        // Diminui os temporizadores usados para controlar o início do pulo.
        if (tempoPuloInicioPersonagem > 0.0f) tempoPuloInicioPersonagem -= dt;

        // Guarda o comando de pulo por alguns milissegundos para facilitar o controle.
        if (IsKeyPressed(KEY_UP)) jumpBufferPersonagem = 0.12f;
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
        BeginMode2D(camera);
        DrawTexture(TexFundo, 0, 0, WHITE);
        desenharPersonagem();
        desenharInimigo();
        EndMode2D();
        DrawText("Utilize as setas para mover e W e seta para cima para pular", 10, 10, 20, WHITE);
        EndDrawing();
    }

    liberarRecursos();
    CloseWindow();
    return 0;
}
