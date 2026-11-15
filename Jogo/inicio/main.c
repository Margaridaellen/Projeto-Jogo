#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include "jogo.h"
#include "colisao.h"
//Define a cor de fundo antes da imagem como "Padrão"
#define CINZA (Color){211, 211, 211, 255}
// O formato da tela em pixels.
#define larguraTela 1920
#define alturaTela 1080
#define QUANTIDADE_CENARIOS 9
#define DURACAO_CENARIOS_SEGUNDOS 180.0f
#define LARGURA_PAREDE 3000.0f
#define LARGURA_COLUNA_CENARIO 56.0f
#define MAX_RECORDES 10

static char caminhoArquivo[1024];
static const char *const caminhosCenarios[QUANTIDADE_CENARIOS] = {
    "Jogo/Imagem Fundo/Cenarios/1794583862594.png",
    "Jogo/Imagem Fundo/Cenarios/1794583971938.png",
    "Jogo/Imagem Fundo/Cenarios/1794583999052.png",
    "Jogo/Imagem Fundo/Cenarios/1794583998981.png",
    "Jogo/Imagem Fundo/Cenarios/1794583991106.png",
    "Jogo/Imagem Fundo/Cenarios/1794583994204.png",
    "Jogo/Imagem Fundo/Cenarios/1794583975891.png",
    "Jogo/Imagem Fundo/Cenarios/1794583983348.png",
    "Jogo/Imagem Fundo/Cenarios/1794583989366.png"
};
static Texture2D cenarios[QUANTIDADE_CENARIOS];
static float posicaoCenarios[QUANTIDADE_CENARIOS];
static float larguraCenarios[QUANTIDADE_CENARIOS];
static float larguraMundo = 0.0f;
static float tempoSequenciaCenarios = 0.0f;
static bool paredeAtiva = false;
static float posicaoParede = 0.0f;

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

static int recordes[MAX_RECORDES];
static int quantidadeRecordes = 0;
static float tempoPartida = 0.0f;
static int pontuacaoPartida = 0;
static bool pontuacaoRegistrada = false;

static void ordenarRecordes(void)
{
    for (int i = 0; i < quantidadeRecordes - 1; i++)
    {
        for (int j = 0; j < quantidadeRecordes - i - 1; j++)
        {
            if (recordes[j] < recordes[j + 1])
            {
                int temporario = recordes[j];
                recordes[j] = recordes[j + 1];
                recordes[j + 1] = temporario;
            }
        }
    }
}

static void carregarRecordes(void)
{
    FILE *arquivo = fopen(resolverCaminhoDoAsset("Jogo/recordes.txt"), "r");
    if (arquivo == NULL) return;

    int pontuacao;
    while (quantidadeRecordes < MAX_RECORDES && fscanf(arquivo, "%d", &pontuacao) == 1)
    {
        if (pontuacao >= 0) recordes[quantidadeRecordes++] = pontuacao;
    }

    fclose(arquivo);
    ordenarRecordes();
}

static int buscarMenorRecorde(void)
{
    int indiceMenor = 0;
    for (int i = 1; i < quantidadeRecordes; i++)
    {
        if (recordes[i] < recordes[indiceMenor]) indiceMenor = i;
    }
    return indiceMenor;
}

static void salvarRecorde(int pontuacao)
{
    if (quantidadeRecordes < MAX_RECORDES)
    {
        recordes[quantidadeRecordes++] = pontuacao;
    }
    else
    {
        int indiceMenor = buscarMenorRecorde();
        if (pontuacao > recordes[indiceMenor]) recordes[indiceMenor] = pontuacao;
    }

    ordenarRecordes();

    FILE *arquivo = fopen(resolverCaminhoDoAsset("Jogo/recordes.txt"), "w");
    if (arquivo == NULL)
    {
        TraceLog(LOG_ERROR, "Nao foi possivel salvar Jogo/recordes.txt");
        return;
    }

    for (int i = 0; i < quantidadeRecordes; i++)
        fprintf(arquivo, "%d\n", recordes[i]);

    fclose(arquivo);
}

//Cria onde os personagens irão expalnar, com as posições x e y, raio e sua velocidade.
GameObject personagem = {1300.0f, 200.0f, 30.0f, 0.0f};
GameObject inimigo = {500.0f, 500.0f, 30.0f, 0.0f};
//Adiciona as variaveis para serem visual dentro dos parametros do Raylib
Texture2D personagemParado[2];
Texture2D animCorrendoProta[5];
Texture2D personagemPulando[4];
Texture2D animAgachandoProta[3];
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
bool gameOver = false;
//Cria o formato, coordenada x e y, largura e altura do piso.
Rectangle piso = {0.0f, 794.0f, 0.0f, 600.0f};
// A função prepara os recursos gráficos antes do loop principal.
// Ela carrega as imagens, remove cores do fundo e calcula as dimensões dos personagens.
void inicializarRecursos(void)
{
    for (int i = 0; i < QUANTIDADE_CENARIOS; i++)
    {
        cenarios[i] = LoadTexture(resolverCaminhoDoAsset(caminhosCenarios[i]));
        if (cenarios[i].id == 0)
        {
            TraceLog(LOG_ERROR, "Nao foi possivel carregar o cenario: %s", caminhosCenarios[i]);
            continue;
        }

        SetTextureFilter(cenarios[i], TEXTURE_FILTER_POINT);
        posicaoCenarios[i] = larguraMundo;
        larguraCenarios[i] = (float)cenarios[i].width * ((float)alturaTela / (float)cenarios[i].height);
        larguraMundo += larguraCenarios[i];
    }
    piso.x = 0.0f;
    piso.width = 1000000.0f;

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

    animAgachandoProta[0] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens prota agachando/po1.png"));
    animAgachandoProta[1] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens prota agachando/po2.png"));
    animAgachandoProta[2] = LoadTexture(resolverCaminhoDoAsset("Jogo/imagens prota agachando/po5.png"));

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

    personagem.largura = larguraPadraoPersonagem;
    personagem.altura = alturaPadraoPersonagem;

    inimigo.largura = larguraPadraoInimigo;
    inimigo.altura = alturaPadraoInimigo;
}
//Cria os loops dos vetores de animação.
void liberarRecursos(void)
{
    for (int i = 0; i < QUANTIDADE_CENARIOS; i++)
    {
        if (cenarios[i].id != 0) UnloadTexture(cenarios[i]);
    }
    for (int i = 0; i < 2; i++) UnloadTexture(personagemParado[i]);
    for (int i = 0; i < 5; i++) UnloadTexture(animCorrendoProta[i]);
    for (int i = 0; i < 4; i++) UnloadTexture(personagemPulando[i]);
    for (int i = 0; i < 3; i++) UnloadTexture(animAgachandoProta[i]);
    for (int i = 0; i < 1; i++) UnloadTexture(inimigoParado[i]);
    for (int i = 0; i < 3; i++) UnloadTexture(animCorrendoRobo[i]);
    for (int i = 0; i < 2; i++) UnloadTexture(inimigoPulando[i]);
}

static void desenharColunaCenario(float centroX)
{
    int largura = (int)LARGURA_COLUNA_CENARIO;
    int x = (int)roundf(centroX - LARGURA_COLUNA_CENARIO / 2.0f);
    DrawRectangle(x, 0, largura, alturaTela, (Color){12, 22, 33, 235});
    DrawRectangle(x + 7, 0, largura - 14, alturaTela, (Color){37, 57, 72, 245});
    DrawRectangle(x + 12, 0, 5, alturaTela, (Color){91, 128, 147, 220});
    DrawRectangle(x + 44, 0, 4, alturaTela, (Color){13, 27, 39, 255});

    for (int y = 110; y < alturaTela; y += 180)
    {
        DrawRectangle(x + 4, y, 48, 12, (Color){18, 31, 43, 255});
        DrawRectangle(x + 10, y + 3, 36, 3, (Color){104, 139, 154, 225});
    }
}

static void desenharCenarios(float centroCameraX)
{
    float metadeTela = (float)larguraTela / 2.0f;
    float limiteEsquerdo = centroCameraX - metadeTela;
    float limiteDireito = centroCameraX + metadeTela;
    int primeiroCiclo = (int)floorf(limiteEsquerdo / larguraMundo);
    int ultimoCiclo = (int)floorf(limiteDireito / larguraMundo);

    for (int ciclo = primeiroCiclo; ciclo <= ultimoCiclo; ciclo++)
    {
        for (int i = 0; i < QUANTIDADE_CENARIOS; i++)
        {
            if (cenarios[i].id == 0) continue;

            float posicaoX = (float)ciclo * larguraMundo + posicaoCenarios[i];
            Rectangle origem = {0.0f, 0.0f, (float)cenarios[i].width, (float)cenarios[i].height};
            Rectangle destino = {posicaoX, 0.0f, larguraCenarios[i], (float)alturaTela};
            DrawTexturePro(cenarios[i], origem, destino, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        }
    }

}

static void desenharColunasCenarios(float centroCameraX)
{
    float metadeTela = (float)larguraTela / 2.0f;
    float limiteEsquerdo = centroCameraX - metadeTela;
    float limiteDireito = centroCameraX + metadeTela;
    int primeiroCiclo = (int)floorf(limiteEsquerdo / larguraMundo) - 1;
    int ultimoCiclo = (int)floorf(limiteDireito / larguraMundo) + 1;

    for (int ciclo = primeiroCiclo; ciclo <= ultimoCiclo; ciclo++)
    {
        for (int i = 0; i < QUANTIDADE_CENARIOS; i++)
        {
            if (cenarios[i].id == 0) continue;

            float posicaoEmenda = (float)ciclo * larguraMundo + posicaoCenarios[i] + larguraCenarios[i];
            if (posicaoEmenda < limiteEsquerdo - LARGURA_COLUNA_CENARIO / 2.0f ||
                posicaoEmenda > limiteDireito + LARGURA_COLUNA_CENARIO / 2.0f) continue;
            desenharColunaCenario(posicaoEmenda);
        }
    }
}

static void limitarPersonagensAoMundo(void)
{
    if (personagem.posX < 0.0f) personagem.posX = 0.0f;
    if (inimigo.posX < 0.0f) inimigo.posX = 0.0f;

    if (paredeAtiva)
    {
        float limitePersonagem = posicaoParede - personagem.largura;
        float limiteInimigo = posicaoParede - inimigo.largura;
        if (personagem.posX > limitePersonagem) personagem.posX = limitePersonagem;
        if (inimigo.posX > limiteInimigo) inimigo.posX = limiteInimigo;
    }
}

static void atualizarCamera(Camera2D *camera, float dt)
{
    float metadeTela = (float)larguraTela / 2.0f;
    float alvoX = personagem.posX + personagem.largura / 2.0f;
    if (alvoX < metadeTela) alvoX = metadeTela;

    float suavizacao = 1.0f - expf(-8.0f * dt);
    camera->target.x += (alvoX - camera->target.x) * suavizacao;
    camera->target.y = (float)alturaTela / 2.0f;
}

static void atualizarTempoCenarios(float dt)
{
    if (paredeAtiva) return;

    tempoSequenciaCenarios += dt;
    if (tempoSequenciaCenarios >= DURACAO_CENARIOS_SEGUNDOS)
    {
        paredeAtiva = true;
        posicaoParede = personagem.posX + personagem.largura + 720.0f;
    }
}

static void desenharParede(void)
{
    if (!paredeAtiva) return;

    Rectangle parede = {posicaoParede, 0.0f, LARGURA_PAREDE, (float)alturaTela};
    DrawRectangleRec(parede, (Color){37, 46, 56, 255});

    for (float x = posicaoParede; x < posicaoParede + LARGURA_PAREDE; x += 180.0f)
    {
        DrawRectangle((int)x, 0, 18, alturaTela, (Color){24, 31, 39, 255});
        DrawLine((int)x + 22, 0, (int)x + 22, alturaTela, (Color){91, 105, 117, 255});
    }

    for (int y = 110; y < alturaTela - 110; y += 180)
    {
        DrawRectangle((int)posicaoParede + 30, y, (int)LARGURA_PAREDE - 60, 8, (Color){66, 79, 91, 255});
    }

    DrawRectangle((int)posicaoParede, (int)piso.y - 72, (int)LARGURA_PAREDE, 72, (Color){105, 77, 35, 255});
    for (float x = posicaoParede + 12.0f; x < posicaoParede + LARGURA_PAREDE; x += 90.0f)
    {
        DrawRectangle((int)x, (int)piso.y - 72, 42, 72, (Color){224, 172, 55, 255});
    }
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
    inicializarRecursos();
    carregarRecordes();

    Camera2D camera = {0};
    camera.offset = (Vector2){larguraTela / 2.0f, alturaTela / 2.0f};
    camera.target = (Vector2){personagem.posX + personagem.largura / 2.0f, alturaTela / 2.0f};
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        // dt representa o tempo desde o último quadro e deixa o movimento independente do FPS.
        float dt = GetFrameTime();
        float v = 500.0f * dt;

        if (gameOver && IsKeyPressed(KEY_R))
        {
            gameOver = false;
            tempoPartida = 0.0f;
            pontuacaoPartida = 0;
            pontuacaoRegistrada = false;
            tempoSequenciaCenarios = 0.0f;
            paredeAtiva = false;
            posicaoParede = 0.0f;
            personagem.posX = 1300.0f;
            personagem.altura = alturaPadraoPersonagem;
            personagem.posY = piso.y - personagem.altura;
            personagem.velocidadeY = 0.0f;
            inimigo.posX = 500.0f;
            inimigo.posY = piso.y - alturaPadraoInimigo + 89.0f;
            inimigo.velocidadeY = 0.0f;
            isGroundedPersonagem = true;
            isGroundedInimigo = true;
            tempoAterrissagem = 0.0f;
            tempoAterrissagemInimigo = 0.0f;
            jumpBufferPersonagem = 0.0f;
            coyoteTimerPersonagem = 0.0f;
            camera.target.x = personagem.posX + personagem.largura / 2.0f;
        }

        if (!gameOver)
        {
            tempoPartida += dt;
            atualizarTempoCenarios(dt);
            if (tempoPuloInicioPersonagem > 0.0f) tempoPuloInicioPersonagem -= dt;

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
            limitarPersonagensAoMundo();
        }

        if (gameOver && !pontuacaoRegistrada)
        {
            pontuacaoPartida = (int)tempoPartida;
            salvarRecorde(pontuacaoPartida);
            pontuacaoRegistrada = true;
        }

        atualizarCamera(&camera, dt);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        desenharCenarios(camera.target.x);
        desenharParede();
        desenharPersonagem();
        desenharInimigo();
        desenharColunasCenarios(camera.target.x);
        EndMode2D();
        DrawText("Setas laterais: mover | Seta para cima: pular | Seta para baixo: abaixar", 10, 10, 20, WHITE);
        if (!paredeAtiva)
        {
            int segundosRestantes = (int)(DURACAO_CENARIOS_SEGUNDOS - tempoSequenciaCenarios);
            DrawText(TextFormat("Local de saida em %02d:%02d", segundosRestantes / 60, segundosRestantes % 60), 10, 38, 20, WHITE);
        }
        else DrawText("LOCAL DE SAIDA DISPONIVEL", 10, 38, 20, YELLOW);
        if (gameOver)
        {
            DrawRectangle(0, 0, larguraTela, alturaTela, (Color){0, 0, 0, 190});
            const char *mensagemGameOver = "GAME OVER";
            const char *mensagemReiniciar = "Pressione R para tentar novamente";
            DrawText(mensagemGameOver, (larguraTela - MeasureText(mensagemGameOver, 72)) / 2, 350, 72, RED);
            DrawText(TextFormat("Tempo sobrevivido: %d s", pontuacaoPartida), (larguraTela - MeasureText(TextFormat("Tempo sobrevivido: %d s", pontuacaoPartida), 28)) / 2, 455, 28, WHITE);
            DrawText(mensagemReiniciar, (larguraTela - MeasureText(mensagemReiniciar, 28)) / 2, 505, 28, WHITE);
            DrawText("Melhores tempos", (larguraTela - MeasureText("Melhores tempos", 24)) / 2, 560, 24, YELLOW);
            for (int i = 0; i < quantidadeRecordes && i < 5; i++)
            {
                const char *linhaRecorde = TextFormat("%d. %d s", i + 1, recordes[i]);
                DrawText(linhaRecorde, (larguraTela - MeasureText(linhaRecorde, 22)) / 2, 600 + i * 32, 22, WHITE);
            }
        }
        EndDrawing();
    }

    liberarRecursos();
    CloseWindow();
    return 0;
}
