# Build script for Claude Engine
param(
    [string]$Config = "Release"
)

Write-Host "==================================" -ForegroundColor Cyan
Write-Host "   Building Claude Engine         " -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan
Write-Host ""

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "build" -Force | Out-Null
}

# Configure CMake
Write-Host "Configuring CMake..." -ForegroundColor Yellow
Set-Location build
cmake .. -G "Visual Studio 17 2022" -A x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: CMake configuration failed" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Build
Write-Host ""
Write-Host "Building project ($Config)..." -ForegroundColor Yellow
cmake --build . --config $Config

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: Build failed" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..

# Copy assets to executable directory
Write-Host ""
Write-Host "Copying assets..." -ForegroundColor Yellow
$assetDest = "build\bin\$Config\assets"
New-Item -ItemType Directory -Force -Path "$assetDest\shaders" | Out-Null

if (Test-Path "assets\shaders") {
    Copy-Item "assets\shaders\*" -Destination "$assetDest\shaders\" -Force
    Write-Host "  Shaders copied" -ForegroundColor Gray
}

if (Test-Path "assets\models") {
    New-Item -ItemType Directory -Force -Path "$assetDest\models" | Out-Null
    Copy-Item "assets\models\*" -Destination "$assetDest\models\" -Force -Recurse
    Write-Host "  Models copied" -ForegroundColor Gray
}

if (Test-Path "assets\textures") {
    New-Item -ItemType Directory -Force -Path "$assetDest\textures" | Out-Null
    Copy-Item "assets\textures\*" -Destination "$assetDest\textures\" -Force -Recurse
    Write-Host "  Textures copied" -ForegroundColor Gray
}

Write-Host ""
Write-Host "==================================" -ForegroundColor Green
Write-Host "   Build Complete!                " -ForegroundColor Green
Write-Host "==================================" -ForegroundColor Green
Write-Host ""
Write-Host "Executable: build\bin\$Config\ClaudeEditor.exe" -ForegroundColor Cyan
Write-Host ""
Write-Host "To run the editor:" -ForegroundColor Yellow
Write-Host "  .\run.ps1" -ForegroundColor Cyan
Write-Host "or" -ForegroundColor Yellow
Write-Host "  cd build\bin\$Config; .\ClaudeEditor.exe" -ForegroundColor Cyan
