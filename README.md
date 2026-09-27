# Projeto-Jogo

Este projeto foi ajustado para funcionar em qualquer máquina, sem depender do caminho absoluto do seu computador.

## Como rodar

### Linux

No terminal, na raiz do projeto:

```bash
./run_jogo.sh
```

### Windows

Na pasta do projeto, clique duas vezes em:

```bat
run_jogo.bat
```

Se preferir compilar pela linha de comando no Windows com MinGW:

```bat
gcc -Wall -Wextra -g3 "Jogo\inicio\main.c" "Jogo\inicio\Prota.c" "Jogo\inicio\Robo.c" -o "Jogo\inicio\output\Principais_Funcoes.exe" -I"Jogo" -lraylib -lopengl32 -lgdi32 -lwinmm
"Jogo\inicio\output\Principais_Funcoes.exe"
```

## O que foi corrigido

- remoção de caminhos absolutos do tipo `/home/usuario/...`
- uso de caminhos relativos ao projeto
- scripts prontos para Linux e Windows
- execução mais simples para o time compartilhar o mesmo projeto
