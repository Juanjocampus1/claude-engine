# GLAD Generator Script for Claude Engine
# This script creates a minimal GLAD loader for OpenGL 4.6 Core

Write-Host "Generating GLAD loader for OpenGL 4.6 Core..." -ForegroundColor Cyan

$gladDir = "dependencies/glad"
$includeDir = "$gladDir/include"
$srcDir = "$gladDir/src"

# Create directories
New-Item -ItemType Directory -Path "$includeDir/glad" -Force | Out-Null
New-Item -ItemType Directory -Path "$includeDir/KHR" -Force | Out-Null
New-Item -ItemType Directory -Path $srcDir -Force | Out-Null

# Download files from GLAD repository
$files = @{
    "https://raw.githubusercontent.com/Dav1dde/glad/refs/heads/master/include/glad/gl.h" = "$includeDir/glad/glad.h"
    "https://raw.githubusercontent.com/Dav1dde/glad/refs/heads/master/include/KHR/khrplatform.h" = "$includeDir/KHR/khrplatform.h"
}

foreach ($url in $files.Keys) {
    $destination = $files[$url]
    Write-Host "Downloading $(Split-Path $url -Leaf)..." -ForegroundColor Yellow
    try {
        Invoke-WebRequest -Uri $url -OutFile $destination -ErrorAction Stop
        Write-Host "  Downloaded successfully" -ForegroundColor Green
    } catch {
        Write-Host "  Failed to download from $url" -ForegroundColor Red
        exit 1
    }
}

# Create a minimal glad.c
$gladC = @"
#include <glad/glad.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__CYGWIN__)
    #include <windows.h>
#endif

static void* get_proc(const char *namez);

#ifdef __cplusplus
extern "C" {
#endif

int gladLoadGL(GLADloadfunc load) {
    // Simplified loader - full implementation would be generated
    return 1;
}

#ifdef __cplusplus
}
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
static HMODULE libGL;

static void* get_proc(const char *namez) {
    void* p = (void*)wglGetProcAddress(namez);
    if(p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        p = (void*)GetProcAddress(libGL, namez);
    }
    return p;
}

int gladLoadGLLoader(GLADloadfunc load) {
    return gladLoadGL(load);
}
#endif
"@

# For now, we'll use a Python script to generate properly
Write-Host ""
Write-Host "Note: For a production build, you should generate GLAD from https://glad.dav1d.de/" -ForegroundColor Yellow
Write-Host "Using Python glad generator..." -ForegroundColor Cyan

# Check if Python is available
$pythonCmd = $null
if (Get-Command python -ErrorAction SilentlyContinue) {
    $pythonCmd = "python"
} elseif (Get-Command python3 -ErrorAction SilentlyContinue) {
    $pythonCmd = "python3"
}

if ($pythonCmd) {
    Write-Host "Python found, installing glad generator..." -ForegroundColor Green
    & $pythonCmd -m pip install glad --quiet
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Generating GLAD files..." -ForegroundColor Yellow
        & $pythonCmd -m glad --api="gl:core=4.6" --out-path=$gladDir c
        if ($LASTEXITCODE -eq 0) {
            Write-Host "GLAD generated successfully!" -ForegroundColor Green
            exit 0
        }
    }
}

Write-Host ""
Write-Host "Automatic GLAD generation failed or Python not found." -ForegroundColor Yellow
Write-Host "Please generate GLAD manually:" -ForegroundColor Cyan
Write-Host "1. Visit https://glad.dav1d.de/" -ForegroundColor White
Write-Host "2. Select: API = gl, Version = 4.6, Profile = Core" -ForegroundColor White
Write-Host "3. Click Generate" -ForegroundColor White
Write-Host "4. Download and extract to: $gladDir" -ForegroundColor White
Write-Host ""

exit 1
