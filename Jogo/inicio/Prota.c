#include "jogo.h"
#include "colisao.h"
#include <math.h>

// A palavra extern informa que essas variáveis foram criadas em outro arquivo.
extern Texture2D personagemParado[2];
extern Texture2D animCorrendoProta[5];
extern Texture2D personagemPulando[4];
extern Texture2D animAgachandoProta[3];
extern float escalaPersonagem, larguraPadraoPersonagem, alturaPadraoPersonagem;
// [NOVO] Necessário para saber a largura do inimigo no cálculo de separação
extern float larguraPadraoInimigo; 
extern int framePersonagem;
extern float tempoAnimacaoPersonagem, tempoAterrissagem, tempoPuloInicioPersonagem;
extern bool olhandoParaEsquerda, protagonistaCorrendo, isGroundedPersonagem, estavaNoAr;
extern float jumpBufferPersonagem, coyoteTimerPersonagem;
extern Rectangle piso;

static int frameAgachamento = 0;
static float tempoAnimacaoAgachamento = 0.0f;
static float rotacaoAgachamento = 0.0f;
static bool agachamentoAtivo = false;
static bool quedaRapidaAgachada = false;

// Atualiza a posição, o pulo, a colisão e a animação do protagonista.
void atualizarPersonagem(float dt, float v)
{
    float posXAntigo = personagem.posX;

    const float forcaPulo = -700.0f;
    const float tempoPorFrame = 0.08f;
    bool personagemEmMovimento = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT);
    if (isGroundedPersonagem && quedaRapidaAgachada) quedaRapidaAgachada = false;
    float alturaDesejada = IsKeyDown(KEY_DOWN) && isGroundedPersonagem
        ? alturaPadraoPersonagem / 2.0f
        : alturaPadraoPersonagem;

    personagem.posY += personagem.altura - alturaDesejada;
    personagem.altura = alturaDesejada;

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
    if (!isGroundedPersonagem)
    {
        const float velocidadeQuedaMaxima = 6000.0f;
        if (IsKeyDown(KEY_DOWN))
        {
            personagem.velocidadeY = velocidadeQuedaMaxima;
            quedaRapidaAgachada = true;
            frameAgachamento = 2;
            agachamentoAtivo = true;
        }
        else personagem.velocidadeY += 1000.0f * dt;
        if (personagem.velocidadeY > velocidadeQuedaMaxima)
            personagem.velocidadeY = velocidadeQuedaMaxima;
    }
    personagem.posY += personagem.velocidadeY * dt;
    isGroundedPersonagem = false;

    // Cria um retângulo de colisão para verificar se o personagem tocou no piso.
    Rectangle sensorChaoPersonagem = {personagem.posX, personagem.posY + 1.0f, larguraPadraoPersonagem, personagem.altura};
    if (CheckCollisionRecs(sensorChaoPersonagem, piso))
    {
        if (personagem.velocidadeY >= 0)
        {
            personagem.posY = piso.y - personagem.altura;
            personagem.velocidadeY = 0.0f;
            isGroundedPersonagem = true;
        }
    }

    // Quando o personagem volta ao chão, ativa temporariamente a imagem de aterrissagem.
    if (estavaNoAr && isGroundedPersonagem)
    {
        tempoAterrissagem = quedaRapidaAgachada ? 0.0f : 0.15f;
        if (quedaRapidaAgachada)
        {
            frameAgachamento = 2;
            agachamentoAtivo = true;
        }
    }
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

    bool personagemAgachado = IsKeyDown(KEY_DOWN) && isGroundedPersonagem;
    // Escolhe a animação de corrida, pulo ou repouso conforme o estado atual.
    protagonistaCorrendo = personagemEmMovimento && isGroundedPersonagem && !personagemAgachado;

    if (personagemAgachado)
    {
        if (!agachamentoAtivo)
        {
            frameAgachamento = 0;
            tempoAnimacaoAgachamento = 0.0f;
            rotacaoAgachamento = 0.0f;
        }

        if (frameAgachamento == 0 || (frameAgachamento == 1 && personagemEmMovimento))
        {
            tempoAnimacaoAgachamento += dt;
            if (tempoAnimacaoAgachamento >= tempoPorFrame)
            {
                tempoAnimacaoAgachamento -= tempoPorFrame;
                frameAgachamento++;
            }
        }
        else if (frameAgachamento == 1)
        {
            tempoAnimacaoAgachamento = 0.0f;
        }

        if (frameAgachamento == 2)
        {
            if (personagemEmMovimento)
            {
                float sentidoRotacao = IsKeyDown(KEY_LEFT) ? -1.0f : 1.0f;
                rotacaoAgachamento = fmodf(rotacaoAgachamento + sentidoRotacao * 900.0f * dt, 360.0f);
                if (rotacaoAgachamento < 0.0f) rotacaoAgachamento += 360.0f;
            }
            else
            {
                frameAgachamento = 1;
                tempoAnimacaoAgachamento = 0.0f;
                rotacaoAgachamento = 0.0f;
            }
        }

        agachamentoAtivo = true;
        tempoAterrissagem = 0.0f;
    }
    else if (quedaRapidaAgachada)
    {
        frameAgachamento = 2;
        agachamentoAtivo = true;
        if (personagemEmMovimento)
        {
            float sentidoRotacao = IsKeyDown(KEY_LEFT) ? -1.0f : 1.0f;
            rotacaoAgachamento = fmodf(rotacaoAgachamento + sentidoRotacao * 900.0f * dt, 360.0f);
            if (rotacaoAgachamento < 0.0f) rotacaoAgachamento += 360.0f;
        }
        tempoAterrissagem = 0.0f;
    }
    else
    {
        agachamentoAtivo = false;
        frameAgachamento = 0;
        tempoAnimacaoAgachamento = 0.0f;
        rotacaoAgachamento = 0.0f;

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

}

// Escolhe a textura correta e desenha o protagonista na tela.
void desenharPersonagem(void)
{
    Texture2D texturaPersonagem;
    float escalaPersonagemRender = 1.0f;
    float posXPersonagemRender = personagem.posX;
    float posYPersonagemRender = personagem.posY;
    float alturaPersonagemRender = personagem.altura;
    float rotacaoPersonagemRender = 0.0f;
    bool desenhandoPo5 = quedaRapidaAgachada ||
        (IsKeyDown(KEY_DOWN) && isGroundedPersonagem && frameAgachamento == 2);

    if (desenhandoPo5)
    {
        texturaPersonagem = animAgachandoProta[2];
        escalaPersonagemRender = 0.624f;
        alturaPersonagemRender = larguraPadraoPersonagem;
        float tamanhoBola = larguraPadraoPersonagem * escalaPersonagemRender;
        posXPersonagemRender = personagem.posX + (larguraPadraoPersonagem - tamanhoBola) / 2.0f;
        posYPersonagemRender = personagem.posY + personagem.altura - tamanhoBola;
        rotacaoPersonagemRender = rotacaoAgachamento;
    }
    else if (!isGroundedPersonagem)
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
    else if (IsKeyDown(KEY_DOWN) && isGroundedPersonagem)
    {
        texturaPersonagem = animAgachandoProta[frameAgachamento];
        escalaPersonagemRender = 0.80f;
        alturaPersonagemRender = alturaPadraoPersonagem;
        float larguraDesenho = larguraPadraoPersonagem * escalaPersonagemRender;
        float alturaDesenho = alturaPadraoPersonagem * escalaPersonagemRender;
        posXPersonagemRender = personagem.posX + (larguraPadraoPersonagem - larguraDesenho) / 2.0f;
        posYPersonagemRender = personagem.posY + personagem.altura - alturaDesenho;
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
    if (desenhandoPo5)
    {
        sourcePersonagem = (Rectangle){25.0f, 275.0f, olhandoParaEsquerda ? -350.0f : 350.0f, 350.0f};
    }
    Rectangle destPersonagem = {posXPersonagemRender, posYPersonagemRender, larguraPadraoPersonagem * escalaPersonagemRender, alturaPersonagemRender * escalaPersonagemRender};
    Vector2 origin = {0.0f, 0.0f};
    if (rotacaoPersonagemRender != 0.0f)
    {
        origin = (Vector2){destPersonagem.width / 2.0f, destPersonagem.height / 2.0f};
        destPersonagem.x += origin.x;
        destPersonagem.y += origin.y;
    }

    DrawTexturePro(texturaPersonagem, sourcePersonagem, destPersonagem, origin, rotacaoPersonagemRender, WHITE);
}