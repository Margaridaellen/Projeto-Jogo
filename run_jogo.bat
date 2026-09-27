@echo off
setlocal

set "ROOT=%~dp0"

mkdir "%ROOT%Jogo\inicio\output" >nul 2>&1

gcc -Wall -Wextra -g3 ^
  "%ROOT%Jogo\inicio\main.c" ^
  "%ROOT%Jogo\inicio\Prota.c" ^
  "%ROOT%Jogo\inicio\Robo.c" ^
  -o "%ROOT%Jogo\inicio\output\Principais_Funcoes.exe" ^
  -I"%ROOT%Jogo" ^
  -lraylib -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo Falha na compilacao do projeto.
    exit /b 1
)

"%ROOT%Jogo\inicio\output\Principais_Funcoes.exe"
