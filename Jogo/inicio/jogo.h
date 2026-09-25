#ifndef JOGO_H
#define JOGO_H

#include <raylib.h>

typedef struct
{
    float posX;
    float posY;
    float raio;
    float velocidadeY;
} GameObject;

extern GameObject personagem;
extern GameObject inimigo;

extern Texture2D TexFundo;
extern Texture2D personagemParado[2];
extern Texture2D animCorrendoProta[5];
extern Texture2D personagemPulando[4];
extern Texture2D inimigoParado[1];
extern Texture2D animCorrendoRobo[3];
extern Texture2D inimigoPulando[2];

extern float escalaPersonagem;
extern float escalaInimigo;
extern float larguraPadraoPersonagem;
extern float alturaPadraoPersonagem;
extern float larguraPadraoInimigo;
extern float alturaPadraoInimigo;

extern int framePersonagem;
extern int frameInimigo;
extern float tempoAnimacaoPersonagem;
extern float tempoAnimacaoInimigo;
extern float tempoAterrissagem;
extern float tempoAterrissagemInimigo;
extern float tempoPuloInicioPersonagem;
extern float jumpBufferPersonagem;
extern float coyoteTimerPersonagem;

extern bool olhandoParaEsquerda;
extern bool olhandoParaEsquerdaRobo;
extern bool protagonistaCorrendo;
extern bool roboCorrendo;
extern bool isGroundedPersonagem;
extern bool isGroundedInimigo;
extern bool estavaNoAr;
extern bool estavaNoArInimigo;

extern Rectangle piso;

void inicializarRecursos(void);
void atualizarPersonagem(float dt, float v);
void desenharPersonagem(void);
void atualizarInimigo(float dt, float v);
void desenharInimigo(void);
void liberarRecursos(void);

#endif
