#include "jogo.h"

extern Texture2D personagemParado[2];
extern Texture2D animCorrendoProta[5];
extern Texture2D personagemPulando[4];
extern float escalaPersonagem, larguraPadraoPersonagem, alturaPadraoPersonagem;
extern int framePersonagem;
extern float tempoAnimacaoPersonagem, tempoAterrissagem, tempoPuloInicioPersonagem;
extern bool olhandoParaEsquerda, protagonistaCorrendo, isGroundedPersonagem, estavaNoAr;
extern float jumpBufferPersonagem, coyoteTimerPersonagem;
extern Rectangle piso;

void atualizarPersonagem(float dt, float v)
{
    const float forcaPulo = -700.0f;
    const float tempoPorFrame = 0.08f;
    bool personagemEmMovimento = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT);

    if (IsKeyDown(KEY_RIGHT))
    {
        personagem.posX += v;
        olhandoParaEsquerda = false;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        personagem.posX -= v;
        olhandoParaEsquerda = true;
    }

    estavaNoAr = !isGroundedPersonagem;
    if (!isGroundedPersonagem) personagem.velocidadeY += 1000.0f * dt;
    personagem.posY += personagem.velocidadeY * dt;
    isGroundedPersonagem = false;

    Rectangle sensorChaoPersonagem = {personagem.posX, personagem.posY + 1.0f, larguraPadraoPersonagem, alturaPadraoPersonagem};
    if (CheckCollisionRecs(sensorChaoPersonagem, piso))
    {
        if (personagem.velocidadeY >= 0)
        {
            personagem.posY = piso.y - alturaPadraoPersonagem;
            personagem.velocidadeY = 0.0f;
            isGroundedPersonagem = true;
        }
    }

    if (estavaNoAr && isGroundedPersonagem) tempoAterrissagem = 0.15f;
    if (tempoAterrissagem > 0.0f) tempoAterrissagem -= dt;

    if (jumpBufferPersonagem > 0.0f && coyoteTimerPersonagem > 0.0f)
    {
        personagem.velocidadeY = forcaPulo;
        isGroundedPersonagem = false;
        coyoteTimerPersonagem = 0.0f;
        jumpBufferPersonagem = 0.0f;
        tempoAterrissagem = 0.0f;
        tempoPuloInicioPersonagem = 0.12f;
    }

    protagonistaCorrendo = personagemEmMovimento && isGroundedPersonagem;

    if (protagonistaCorrendo)
    {
        tempoAnimacaoPersonagem += dt;
        if (tempoAnimacaoPersonagem >= tempoPorFrame)
        {
            tempoAnimacaoPersonagem = 0.0f;
            framePersonagem = (framePersonagem + 1) % 5;
        }
        tempoAterrissagem = 0.0f;
    }
    else if (!isGroundedPersonagem)
    {
        if (personagemEmMovimento) framePersonagem = 2;
        else if (tempoPuloInicioPersonagem > 0.0f) framePersonagem = 0;
        else framePersonagem = 1;
    }
    else
    {
        tempoAnimacaoPersonagem += dt;
        if (tempoAnimacaoPersonagem >= tempoPorFrame * 3.0f)
        {
            tempoAnimacaoPersonagem = 0.0f;
            framePersonagem = (framePersonagem + 1) % 2;
        }
    }
}

void desenharPersonagem(void)
{
    Texture2D texturaPersonagem;
    float escalaPersonagemRender = 1.0f;
    float posYPersonagemRender = personagem.posY;

    if (!isGroundedPersonagem)
    {
        if (framePersonagem == 2)
        {
            escalaPersonagemRender = 1.15f;
            posYPersonagemRender = personagem.posY - (alturaPadraoPersonagem * (escalaPersonagemRender - 1.0f));
        }
        else
        {
            escalaPersonagemRender = 0.90f;
            posYPersonagemRender = personagem.posY - (alturaPadraoPersonagem * (escalaPersonagemRender - 1.0f));
        }
        texturaPersonagem = personagemPulando[framePersonagem];
    }
    else if (tempoAterrissagem > 0.0f)
    {
        texturaPersonagem = personagemPulando[3];
    }
    else if (protagonistaCorrendo)
    {
        escalaPersonagemRender = 0.90f;
        posYPersonagemRender = personagem.posY - (alturaPadraoPersonagem * (escalaPersonagemRender - 1.0f));
        texturaPersonagem = animCorrendoProta[framePersonagem % 5];
    }
    else
    {
        texturaPersonagem = personagemParado[framePersonagem % 2];
    }

    Rectangle sourcePersonagem = {0.0f, 0.0f, olhandoParaEsquerda ? -(float)texturaPersonagem.width : (float)texturaPersonagem.width, (float)texturaPersonagem.height};
    Rectangle destPersonagem = {personagem.posX, posYPersonagemRender, larguraPadraoPersonagem * escalaPersonagemRender, alturaPadraoPersonagem * escalaPersonagemRender};
    Vector2 origin = {0.0f, 0.0f};

    DrawTexturePro(texturaPersonagem, sourcePersonagem, destPersonagem, origin, 0.0f, WHITE);
}

