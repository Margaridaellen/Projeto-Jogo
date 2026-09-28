@echo off
setlocal

set "ROOT=%~dp0"
set "RAYLIB_ROOT=C:\raylib\w64devkit"
set "GCC=gcc"

if exist "%RAYLIB_ROOT%\bin\gcc.exe" (
  set "GCC=%RAYLIB_ROOT%\bin\gcc.exe"
  set "PATH=%RAYLIB_ROOT%\bin;%PATH%"
)

mkdir "%ROOT%Jogo\inicio\output" >nul 2>&1

"%GCC%" -std=c11 -Wall -Wextra -g3 ^
  "%ROOT%Jogo\inicio\main.c" ^
  "%ROOT%Jogo\inicio\Prota.c" ^
  "%ROOT%Jogo\inicio\Robo.c" ^
  -o "%ROOT%Jogo\inicio\output\Principais_Funcoes.exe" ^
  -I"%ROOT%Jogo" ^
  -L"%RAYLIB_ROOT%\lib" ^
  -lraylib -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo Falha na compilacao do projeto.
    exit /b 1
)

if /i "%~1"=="--build-only" exit /b 0

"%ROOT%Jogo\inicio\output\Principais_Funcoes.exe"
