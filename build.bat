@echo off
echo Building the OpenGL Application...

:: Step 1: Force close any running instance so the file isn't locked
taskkill /f /im myProgram.exe >nul 2>&1

:: Step 2: Compile
g++ src/main.cpp src/glad.c -o myProgram.exe -Iinclude -lglfw3 -lopengl32 -lgdi32

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Launching myProgram...
    :: Use 'start' so the terminal stays free
    start myProgram.exe
) else (
    echo Build failed! Look at the errors above.
    pause
)