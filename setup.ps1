# Setup script for Claude Engine
Write-Host "==================================" -ForegroundColor Cyan
Write-Host "   Claude Engine Setup Script    " -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan
Write-Host ""

# Check if git is installed
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "Error: Git is not installed or not in PATH" -ForegroundColor Red
    exit 1
}

# Initialize and update git submodules
Write-Host "Cloning dependencies (this may take a while)..." -ForegroundColor Yellow
git submodule update --init --recursive

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: Failed to clone submodules" -ForegroundColor Red
    exit 1
}

# Setup GLAD
Write-Host ""
Write-Host "Setting up GLAD..." -ForegroundColor Yellow
$gladDir = "dependencies/glad"
if (-not (Test-Path "$gladDir/src/glad.c")) {
    Write-Host "GLAD not found. Generating GLAD loader..." -ForegroundColor Yellow
    Write-Host "Please visit: https://glad.dav1d.de/" -ForegroundColor Cyan
    Write-Host "  - Language: C/C++" -ForegroundColor White
    Write-Host "  - API: gl=Version 4.6, Profile: Core" -ForegroundColor White
    Write-Host "  - Generate a loader" -ForegroundColor White
    Write-Host "  - Download and extract to: $gladDir" -ForegroundColor White
    Write-Host "" -ForegroundColor White
    Write-Host "Creating directory structure..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "$gladDir/include/glad" -Force | Out-Null
    New-Item -ItemType Directory -Path "$gladDir/include/KHR" -Force | Out-Null
    New-Item -ItemType Directory -Path "$gladDir/src" -Force | Out-Null
    
    # Try to download pre-generated GLAD (OpenGL 4.6 Core)
    try {
        Write-Host "Attempting to download GLAD from GitHub..." -ForegroundColor Yellow
        $gladBaseUrl = "https://raw.githubusercontent.com/Dav1dde/glad/glad2/cmake/glad/include/glad"
        Invoke-WebRequest -Uri "https://raw.githubusercontent.com/Dav1dde/glad/master/include/glad/glad.h" -OutFile "$gladDir/include/glad/glad.h" -ErrorAction Stop
        Invoke-WebRequest -Uri "https://raw.githubusercontent.com/Dav1dde/glad/master/include/KHR/khrplatform.h" -OutFile "$gladDir/include/KHR/khrplatform.h" -ErrorAction Stop
        Invoke-WebRequest -Uri "https://raw.githubusercontent.com/Dav1dde/glad/master/src/glad.c" -OutFile "$gladDir/src/glad.c" -ErrorAction Stop
        Write-Host "GLAD files downloaded successfully" -ForegroundColor Green
    } catch {
        Write-Host "Automatic download failed." -ForegroundColor Red
        Write-Host "Please generate GLAD manually from https://glad.dav1d.de/ and place in $gladDir" -ForegroundColor Yellow
    }
}

# Setup stb_image
Write-Host ""
Write-Host "Setting up stb_image..." -ForegroundColor Yellow
$stbDir = "dependencies/stb"
if (-not (Test-Path $stbDir)) {
    New-Item -ItemType Directory -Path $stbDir -Force | Out-Null
}

# Download stb_image.h
$stbImageUrl = "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
$stbImagePath = "$stbDir/stb_image.h"
if (-not (Test-Path $stbImagePath) -or (Get-Item $stbImagePath).Length -lt 1000) {
    Write-Host "Downloading stb_image.h..." -ForegroundColor Yellow
    try {
        Invoke-WebRequest -Uri $stbImageUrl -OutFile $stbImagePath -ErrorAction Stop
        Write-Host "stb_image.h downloaded successfully" -ForegroundColor Green
    } catch {
        Write-Host "Warning: Could not download stb_image.h automatically" -ForegroundColor Yellow
        Write-Host "Please download manually from: $stbImageUrl" -ForegroundColor Yellow
    }
}

# Create build directory
Write-Host ""
Write-Host "Creating build directory..." -ForegroundColor Yellow
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" -Force | Out-Null
}

Write-Host ""
Write-Host "==================================" -ForegroundColor Green
Write-Host "   Setup Complete!                " -ForegroundColor Green
Write-Host "==================================" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. cd build" -ForegroundColor White
Write-Host "2. cmake .." -ForegroundColor White
Write-Host "3. cmake --build . --config Release" -ForegroundColor White
Write-Host ""
Write-Host "Or simply run: .\build.ps1" -ForegroundColor Yellow
