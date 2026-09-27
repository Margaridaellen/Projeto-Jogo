#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$ROOT_DIR/Jogo/inicio/output"

gcc -Wall -Wextra -g3 \
  "$ROOT_DIR/Jogo/inicio/main.c" \
  "$ROOT_DIR/Jogo/inicio/Prota.c" \
  "$ROOT_DIR/Jogo/inicio/Robo.c" \
  -o "$ROOT_DIR/Jogo/inicio/output/Principais_Funcoes" \
  -I"$ROOT_DIR/Jogo" \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

"$ROOT_DIR/Jogo/inicio/output/Principais_Funcoes"
