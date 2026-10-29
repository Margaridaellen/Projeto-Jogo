#include <stdbool.h>
#include "jogo.h"
bool checarColisaoObjetos(GameObject objprota, GameObject objinimigo){
    if (objprota.posX + objprota.largura > objinimigo.posX && objprota.posX < objinimigo.posX + objinimigo.largura)
    {
        if (objprota.posY + objprota.altura > objinimigo.posY && objprota.posY < objinimigo.posY + objinimigo.altura)
        {
            return true;
        }
        
    }
    return false;
}