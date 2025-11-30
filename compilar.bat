@echo off
setlocal enabledelayedexpansion

REM Borra el ejecutable viejo
if exist programa.exe del programa.exe

REM Construye la lista de archivos .cpp dentro de src
set SOURCES=
for %%f in (src\*.cpp) do set SOURCES=!SOURCES! %%f

REM Compila con include en la carpeta include
g++ -std=c++17 -I include !SOURCES! -o programa.exe
if %errorlevel% neq 0 (
    echo Hubo un error de compilacion
    pause
    exit /b
)

REM Ejecuta el programa
programa.exe
pause