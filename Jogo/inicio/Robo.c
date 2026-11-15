#include "jogo.h"
#include "colisao.h"

// Variáveis compartilhadas com outros arquivos
extern Texture2D inimigoParado[1];
extern Texture2D animCorrendoRobo[3];
extern Texture2D inimigoPulando[2];
extern float escalaInimigo, larguraPadraoInimigo, alturaPadraoInimigo;
extern float larguraPadraoPersonagem, alturaPadraoPersonagem;
extern int frameInimigo;
extern float tempoAnimacaoInimigo, tempoAterrissagemInimigo;
extern bool olhandoParaEsquerdaRobo, roboCorrendo, isGroundedInimigo, estavaNoArInimigo;
extern Rectangle piso;
extern bool gameOver;

static float tempoFaseVelocidadeRobo = 0.0f;
static bool roboRapido = false;

void atualizarInimigo(float dt, float v)
{
    if (gameOver) return;

    float posXAntigoInimigo = inimigo.posX;

    const float tempoPorFrame = 0.08f;
    if (tempoFaseVelocidadeRobo <= 0.0f)
    {
        roboRapido = !roboRapido;
        tempoFaseVelocidadeRobo = roboRapido ? 0.8f : 2.8f;
    }
    tempoFaseVelocidadeRobo -= dt;
    float velocidadeRobo = v * (roboRapido ? 1.3f : 0.55f);

    float centroPersonagem = personagem.posX + personagem.largura / 2.0f;
    float centroInimigo = inimigo.posX + inimigo.largura / 2.0f;
    if (centroPersonagem - centroInimigo > 850.0f && IsKeyDown(KEY_RIGHT))
        velocidadeRobo = v;

    if (centroPersonagem > centroInimigo)
    {
        inimigo.posX += velocidadeRobo;
        olhandoParaEsquerdaRobo = false;
    }
    else if (centroPersonagem < centroInimigo)
    {
        inimigo.posX -= velocidadeRobo;
        olhandoParaEsquerdaRobo = true;
    }

    // Gravidade e movimentação vertical
    estavaNoArInimigo = !isGroundedInimigo;
    if (!isGroundedInimigo) inimigo.velocidadeY += 1000.0f * dt;
    inimigo.posY += inimigo.velocidadeY * dt;
    isGroundedInimigo = false;

    // Colisão com o piso
    Rectangle sensorChaoInimigo = {inimigo.posX, inimigo.posY + 1.0f, larguraPadraoInimigo, alturaPadraoInimigo};
    if (CheckCollisionRecs(sensorChaoInimigo, piso))
    {
        if (inimigo.velocidadeY >= 0)
        {
            inimigo.posY = piso.y - alturaPadraoInimigo;
            inimigo.velocidadeY = 0.0f;
            isGroundedInimigo = true;
        }
    }

    // Animação de aterrissagem e pulo
    if (estavaNoArInimigo && isGroundedInimigo) tempoAterrissagemInimigo = 0.15f;
    if (tempoAterrissagemInimigo > 0.0f) tempoAterrissagemInimigo -= dt;

    if (checarColisaoObjetos(personagem, inimigo))
    {
        gameOver = true;
        roboCorrendo = false;
        return;
    }

    bool inimigoEmMovimento = (inimigo.posX != posXAntigoInimigo);
    roboCorrendo = inimigoEmMovimento && isGroundedInimigo;

    if (roboCorrendo)
    {
        tempoAnimacaoInimigo += dt;
        if (tempoAnimacaoInimigo >= tempoPorFrame)
        {
            tempoAnimacaoInimigo = 0.0f;
            frameInimigo = (frameInimigo + 1) % 3;
        }
        tempoAterrissagemInimigo = 0.0f;
    }
    else
    {
        frameInimigo = 0;
    }
}

void desenharInimigo(void)
{
    Texture2D texturaInimigo;
    float escalaRoboRender = 1.20f;
    float posYRoboRender = inimigo.posY;

    if (!isGroundedInimigo)
    {
        escalaRoboRender = 1.40f;
        posYRoboRender = inimigo.posY - (alturaPadraoInimigo * (escalaRoboRender - 1.0f));
        texturaInimigo = inimigoPulando[0];
    }
    else if (tempoAterrissagemInimigo > 0.0f)
    {
        texturaInimigo = inimigoPulando[1];
    }
    else if (roboCorrendo)
    {
        posYRoboRender = inimigo.posY - (alturaPadraoInimigo * (escalaRoboRender - 1.0f));
        texturaInimigo = animCorrendoRobo[frameInimigo % 3];
    }
    else
    {
        texturaInimigo = inimigoParado[0];
    }

    Rectangle sourceInimigo = {0.0f, 0.0f, olhandoParaEsquerdaRobo ? -(float)texturaInimigo.width : (float)texturaInimigo.width, (float)texturaInimigo.height};
    Rectangle destInimigo = {inimigo.posX, posYRoboRender, larguraPadraoInimigo * escalaRoboRender, alturaPadraoInimigo * escalaRoboRender};
    Vector2 origin = {0.0f, 0.0f};

    DrawTexturePro(texturaInimigo, sourceInimigo, destInimigo, origin, 0.0f, WHITE);
}