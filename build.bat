@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Usage: %~nx0 static^|dynamic
    exit /b 1
)

if /i "%~1"=="static" goto :build_static
if /i "%~1"=="dynamic" goto :build_dynamic

echo Unknown option: %~1
echo Usage: %~nx0 static^|dynamic
exit /b 1

:build_static
echo Building static version...

if not exist bin mkdir bin

gcc -Wno-implicit-function-declaration -static src\*.c -Iinclude -o bin\cbeta -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lgdi32 -lsetupapi -lole32 -loleaut32 -limm32 -lwinmm -lversion -lcfgmgr32
if %errorlevel% neq 0 (
    echo Static build failed.
    exit /b %errorlevel%
)

if exist resources (
    xcopy /E /I /Y resources bin\resources
) else (
    echo Warning: "resources" folder not found, skipping copy.
)

echo Static build finished. Output: bin\cbeta.exe
goto :eof

:build_dynamic
echo Building dynamic version...

if not exist bin mkdir bin

if exist templib rmdir /S /Q templib
mkdir templib

:: Copy required DLLs and import library (adjust source paths if needed)
if exist SDL2.dll (
    copy /Y SDL2.dll templib\
) else (
    echo ERROR: SDL2.dll not found in current directory.
    exit /b 1
)
if exist SDL2_image.dll (
    copy /Y SDL2_image.dll templib\
) else (
    echo ERROR: SDL2_image.dll not found in current directory.
    exit /b 1
)
if exist lib\libSDL2main.a (
    copy /Y lib\libSDL2main.a templib\
) else (
    echo ERROR: lib\libSDL2main.a not found in lib\ folder.
    exit /b 1
)

gcc -Wno-implicit-function-declaration src\*.c -Iinclude -lmingw32 -Ltemplib -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lgdi32 -o bin\cbeta
if %errorlevel% neq 0 (
    echo Dynamic build failed.
    exit /b %errorlevel%
)

:: Copy DLLs to bin\ so the executable can find them at runtime
copy /Y templib\SDL2.dll bin\
copy /Y templib\SDL2_image.dll bin\

if exist resources (
    xcopy /E /I /Y resources bin\resources
) else (
    echo Warning: "resources" folder not found, skipping copy.
)

echo Dynamic build finished. Output: bin\cbeta.exe (DLLs also copied to bin\)
rmdir /S /Q templib
goto :eof