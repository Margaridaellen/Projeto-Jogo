
//Evitar que o mesmo arquivo de cabeçalho (.h) seja processado mais de uma vez pelo compilador.
#ifndef JOGO_H
#define JOGO_H

#include <raylib.h>

//cria um tipo de dado personalizado que agrupa várias variáveis relacionadas em uma única estrutura.
typedef struct
{
    float posX;
    float posY;
    float raio;
    float velocidadeY;
} GameObject;
//O "extern" vai estender o dado para o copilador saber que ele pode ir alem de um arquivo 
//fazendo com que seja possivel chamalo fora desse arquivo.
extern GameObject personagem;
extern GameObject inimigo;
//A "Texture2D" é uma struct do Raylib para trasnformar algo em visual
// de acordo com os agrupamentos de paramentros de dados impostos no "Texture2D".
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
//Determina o "Piso" do jogo como um formato geometrico ultilizando uma struct.
extern Rectangle piso;
//Adicona novas funções que serão usadas ao decorrerdo projeto.
//Pois toda função em C possuem ligações externas, então pode ser usadas fora desse arquivo, 
//navegando em todo o projeto.
void inicializarRecursos(void);
void atualizarPersonagem(float dt, float v);
void desenharPersonagem(void);
void atualizarInimigo(float dt, float v);
void desenharInimigo(void);
void liberarRecursos(void);
//marca o fim do bloco condicional aberto pelo #ifndef.
#endif
