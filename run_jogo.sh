#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"
mkdir -p "Jogo/inicio/output"

gcc -Wall -Wextra -g3 \
  "Jogo/inicio/main.c" \
  "Jogo/inicio/Prota.c" \
  "Jogo/inicio/Robo.c" \
  -o "Jogo/inicio/output/Principais_Funcoes" \
  -I"/home/samuel/Área de trabalho/Projeto_Jogo/Jogo" \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

./Jogo/inicio/output/Principais_Funcoes
