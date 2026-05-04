# ============================================================
#  setup_windows.ps1  –  CraftCPP dependency installer
#  Run once from the project root:
#      powershell -ExecutionPolicy Bypass -File setup_windows.ps1
# ============================================================

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host ""
Write-Host "================================================" -ForegroundColor Cyan
Write-Host "  CraftCPP  –  Windows Setup Script" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# ── 1. Check prerequisites ────────────────────────────────────────────────────

# Git
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "[ERROR] Git not found. Install from https://git-scm.com/" -ForegroundColor Red
    exit 1
}
Write-Host "[OK] Git found." -ForegroundColor Green

# CMake
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "[ERROR] CMake not found. Install from https://cmake.org/download/" -ForegroundColor Red
    exit 1
}
$cmakeVer = (cmake --version | Select-String '\d+\.\d+').Matches[0].Value
Write-Host "[OK] CMake $cmakeVer found." -ForegroundColor Green

# Visual Studio / cl.exe
$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vsWhere)) {
    Write-Host "[WARN] vswhere.exe not found – make sure Visual Studio 2019/2022 is installed." -ForegroundColor Yellow
} else {
    $vsPath = & $vsWhere -latest -property installationPath
    Write-Host "[OK] Visual Studio found at: $vsPath" -ForegroundColor Green
}

# ── 2. Install / update vcpkg ─────────────────────────────────────────────────

$VcpkgDir = "$ProjectRoot\vcpkg"

if (Test-Path "$VcpkgDir\vcpkg.exe") {
    Write-Host ""
    Write-Host "[INFO] vcpkg already installed, updating..." -ForegroundColor Cyan
    Push-Location $VcpkgDir
    git pull --quiet
    Pop-Location
} else {
    Write-Host ""
    Write-Host "[INFO] Cloning vcpkg..." -ForegroundColor Cyan
    git clone https://github.com/microsoft/vcpkg.git $VcpkgDir --depth=1 --quiet

    Write-Host "[INFO] Bootstrapping vcpkg..." -ForegroundColor Cyan
    & "$VcpkgDir\bootstrap-vcpkg.bat" -disableMetrics | Out-Null
}

$vcpkg = "$VcpkgDir\vcpkg.exe"
Write-Host "[OK] vcpkg ready." -ForegroundColor Green

# ── 3. Install packages (x64-windows) ────────────────────────────────────────

$Packages = @("glfw3", "glew", "glm")
$Triplet  = "x64-windows"

Write-Host ""
Write-Host "[INFO] Installing packages for triplet: $Triplet" -ForegroundColor Cyan
foreach ($pkg in $Packages) {
    Write-Host "  -> $pkg ..." -ForegroundColor White
    & $vcpkg install "$pkg`:$Triplet" --recurse 2>&1 | Where-Object { $_ -match "(-- Installing|error)" } | Write-Host
}
Write-Host "[OK] All packages installed." -ForegroundColor Green

# ── 4. Integrate vcpkg with MSBuild / CMake ───────────────────────────────────

Write-Host ""
Write-Host "[INFO] Integrating vcpkg..." -ForegroundColor Cyan
& $vcpkg integrate install | Out-Null
Write-Host "[OK] vcpkg integrated." -ForegroundColor Green

# ── 5. Store toolchain path for build.bat ─────────────────────────────────────

$ToolchainFile = "$VcpkgDir\scripts\buildsystems\vcpkg.cmake"
$EnvFile = "$ProjectRoot\.vcpkg_toolchain"
Set-Content -Path $EnvFile -Value $ToolchainFile
Write-Host "[INFO] Toolchain path saved to .vcpkg_toolchain" -ForegroundColor Cyan

# ── 6. Done ───────────────────────────────────────────────────────────────────

Write-Host ""
Write-Host "================================================" -ForegroundColor Green
Write-Host "  Setup complete!" -ForegroundColor Green
Write-Host "  Now run:  build.bat" -ForegroundColor Green
Write-Host "================================================" -ForegroundColor Green
Write-Host ""
