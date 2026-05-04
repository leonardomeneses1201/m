@echo off
setlocal enabledelayedexpansion

:: ============================================================
::  build.bat  –  Configure & compile CraftCPP on Windows
::  Usage:
::    build.bat           -> Release build
::    build.bat debug     -> Debug build
::    build.bat clean     -> Remove build folder and rebuild
:: ============================================================

set BUILD_TYPE=Release
set BUILD_DIR=build

if /I "%1"=="debug" set BUILD_TYPE=Debug
if /I "%1"=="clean" (
    echo [INFO] Cleaning build directory...
    if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
)

:: ── Read vcpkg toolchain path ─────────────────────────────────────────────────
set TOOLCHAIN_FILE=
if exist ".vcpkg_toolchain" (
    set /p TOOLCHAIN_FILE=<.vcpkg_toolchain
    echo [INFO] Using vcpkg toolchain: !TOOLCHAIN_FILE!
) else (
    echo [WARN] .vcpkg_toolchain not found. Run setup_windows.ps1 first.
    echo        Attempting build without vcpkg toolchain...
)

:: ── Find Visual Studio via vswhere ────────────────────────────────────────────
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist %VSWHERE% (
    for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -property installationPath`) do (
        set VS_PATH=%%i
    )
    set VCVARS=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat
    if exist "!VCVARS!" (
        echo [INFO] Activating MSVC x64 environment...
        call "!VCVARS!" >nul 2>&1
    )
)

:: ── CMake configure ───────────────────────────────────────────────────────────
echo.
echo [INFO] Configuring CMake (%BUILD_TYPE%)...
echo ────────────────────────────────────────────────────────

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

if defined TOOLCHAIN_FILE (
    cmake -S . -B %BUILD_DIR% ^
        -G "Visual Studio 17 2022" -A x64 ^
        -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
        -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN_FILE%" ^
        -DVCPKG_TARGET_TRIPLET=x64-windows
) else (
    cmake -S . -B %BUILD_DIR% ^
        -G "Visual Studio 17 2022" -A x64 ^
        -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
)

if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed!
    echo         Check that Visual Studio 2022 and all dependencies are installed.
    echo         Run setup_windows.ps1 to install dependencies automatically.
    pause
    exit /b 1
)

:: ── CMake build ───────────────────────────────────────────────────────────────
echo.
echo [INFO] Building (%BUILD_TYPE%)...
echo ────────────────────────────────────────────────────────

cmake --build %BUILD_DIR% --config %BUILD_TYPE% --parallel

if errorlevel 1 (
    echo.
    echo [ERROR] Build failed! See errors above.
    pause
    exit /b 1
)

:: ── Locate the exe ────────────────────────────────────────────────────────────
set EXE_PATH=
for /r %BUILD_DIR% %%f in (CraftCPP.exe) do set EXE_PATH=%%f

if not defined EXE_PATH (
    echo [WARN] CraftCPP.exe not found in %BUILD_DIR%.
    pause
    exit /b 1
)

echo.
echo ════════════════════════════════════════════════════════
echo   Build successful!
echo   Executable: !EXE_PATH!
echo ════════════════════════════════════════════════════════
echo.

:: ── Ask to run ────────────────────────────────────────────────────────────────
set /p RUN_NOW="Launch CraftCPP now? [Y/n]: "
if /I not "!RUN_NOW!"=="n" (
    echo [INFO] Launching CraftCPP...
    start "" "!EXE_PATH!"
)

endlocal
