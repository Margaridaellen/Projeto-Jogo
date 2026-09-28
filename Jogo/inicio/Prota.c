#include "jogo.h"

// A palavra extern informa que essas variáveis foram criadas em outro arquivo.
// Este arquivo apenas usa os mesmos dados para atualizar e desenhar o protagonista.
extern Texture2D personagemParado[2];
extern Texture2D animCorrendoProta[5];
extern Texture2D personagemPulando[4];
extern float escalaPersonagem, larguraPadraoPersonagem, alturaPadraoPersonagem;
extern int framePersonagem;
extern float tempoAnimacaoPersonagem, tempoAterrissagem, tempoPuloInicioPersonagem;
extern bool olhandoParaEsquerda, protagonistaCorrendo, isGroundedPersonagem, estavaNoAr;
extern float jumpBufferPersonagem, coyoteTimerPersonagem;
extern Rectangle piso;

// Atualiza a posição, o pulo, a colisão e a animação do protagonista.
void atualizarPersonagem(float dt, float v)
{
    // A força do pulo aponta para cima, por isso seu valor é negativo no eixo Y.
    const float forcaPulo = -700.0f;
    const float tempoPorFrame = 0.08f;
    // Verifica se alguma tecla de movimento está sendo pressionada neste quadro.
    bool personagemEmMovimento = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT);

    // Move o protagonista e registra para qual lado ele está olhando.
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

    // Aplica a gravidade e atualiza a posição vertical do personagem.
    estavaNoAr = !isGroundedPersonagem;
    if (!isGroundedPersonagem) personagem.velocidadeY += 1000.0f * dt;
    personagem.posY += personagem.velocidadeY * dt;
    isGroundedPersonagem = false;

    // Cria um retângulo de colisão para verificar se o personagem tocou no piso.
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

    // Quando o personagem volta ao chão, ativa temporariamente a imagem de aterrissagem.
    if (estavaNoAr && isGroundedPersonagem) tempoAterrissagem = 0.15f;
    if (tempoAterrissagem > 0.0f) tempoAterrissagem -= dt;

    // Executa o pulo se o comando ainda estiver guardado e o personagem puder pular.
    if (jumpBufferPersonagem > 0.0f && coyoteTimerPersonagem > 0.0f)
    {
        personagem.velocidadeY = forcaPulo;
        isGroundedPersonagem = false;
        coyoteTimerPersonagem = 0.0f;
        jumpBufferPersonagem = 0.0f;
        tempoAterrissagem = 0.0f;
        tempoPuloInicioPersonagem = 0.12f;
    }

    // Escolhe a animação de corrida, pulo ou repouso conforme o estado atual.
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

// Escolhe a textura correta e desenha o protagonista na tela.
void desenharPersonagem(void)
{
    // A escala e a posição podem mudar durante o pulo para acompanhar cada quadro.
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

    // Uma largura negativa espelha a textura quando o personagem olha para a esquerda.
    Rectangle sourcePersonagem = {0.0f, 0.0f, olhandoParaEsquerda ? -(float)texturaPersonagem.width : (float)texturaPersonagem.width, (float)texturaPersonagem.height};
    Rectangle destPersonagem = {personagem.posX, posYPersonagemRender, larguraPadraoPersonagem * escalaPersonagemRender, alturaPadraoPersonagem * escalaPersonagemRender};
    Vector2 origin = {0.0f, 0.0f};

    DrawTexturePro(texturaPersonagem, sourcePersonagem, destPersonagem, origin, 0.0f, WHITE);
}

