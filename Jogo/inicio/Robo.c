#include "jogo.h"

extern Texture2D inimigoParado[1];
extern Texture2D animCorrendoRobo[3];
extern Texture2D inimigoPulando[2];
extern float escalaInimigo, larguraPadraoInimigo, alturaPadraoInimigo;
extern int frameInimigo;
extern float tempoAnimacaoInimigo, tempoAterrissagemInimigo;
extern bool olhandoParaEsquerdaRobo, roboCorrendo, isGroundedInimigo, estavaNoArInimigo;
extern Rectangle piso;

void atualizarInimigo(float dt, float v)
{
    const float forcaPulo = -700.0f;
    const float tempoPorFrame = 0.08f;
    bool inimigoEmMovimento = IsKeyDown(KEY_D) || IsKeyDown(KEY_A);

    if (IsKeyDown(KEY_D))
    {
        inimigo.posX += v;
        olhandoParaEsquerdaRobo = false;
    }
    if (IsKeyDown(KEY_A))
    {
        inimigo.posX -= v;
        olhandoParaEsquerdaRobo = true;
    }

    estavaNoArInimigo = !isGroundedInimigo;
    if (!isGroundedInimigo) inimigo.velocidadeY += 1000.0f * dt;
    inimigo.posY += inimigo.velocidadeY * dt;
    isGroundedInimigo = false;

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

    if (estavaNoArInimigo && isGroundedInimigo) tempoAterrissagemInimigo = 0.15f;
    if (tempoAterrissagemInimigo > 0.0f) tempoAterrissagemInimigo -= dt;

    if (isGroundedInimigo && IsKeyPressed(KEY_W))
    {
        inimigo.velocidadeY = forcaPulo;
        isGroundedInimigo = false;
        tempoAterrissagemInimigo = 0.0f;
    }

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

