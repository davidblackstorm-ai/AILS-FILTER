@echo off
setlocal
cd /d "%~dp0"

echo ===========================================
echo      AILS FILTER HP - VST3 BUILD
echo ===========================================
echo.

where cmake >nul 2>nul
if errorlevel 1 (
  echo [ERRORE] CMake non trovato.
  echo Installa CMake e Visual Studio 2022 con "Desktop development with C++".
  pause
  exit /b 1
)

if not exist build mkdir build

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto :fail

cmake --build build --config Release
if errorlevel 1 goto :fail

echo.
echo BUILD COMPLETATA.
echo.
for /r "build" %%F in ("AILS Filter HP.vst3") do (
  echo VST3 trovato in:
  echo %%~fF
  explorer "%%~dpF"
  goto :done
)

echo Il plugin e' stato compilato ma non trovo automaticamente la cartella finale.
explorer "%cd%\build"
goto :done

:fail
echo.
echo [ERRORE] La compilazione non e' riuscita. Copiami qui il testo dell'errore.

:done
pause
