# Script para ejecutar Claude Engine Editor
# Este script copia los assets necesarios y ejecuta el editor

Write-Host "=== Claude Engine - Launch Script ===" -ForegroundColor Cyan

# Verificar que estamos en el directorio correcto
if (-not (Test-Path "build\bin\Release\ClaudeEditor.exe")) {
    Write-Host "[ERROR] No se encontró ClaudeEditor.exe" -ForegroundColor Red
    Write-Host "        Asegúrate de estar en el directorio raíz del proyecto" -ForegroundColor Yellow
    Write-Host "        y haber compilado el proyecto primero." -ForegroundColor Yellow
    pause
    exit 1
}

# Crear directorio de assets si no existe
Write-Host "[INFO] Verificando assets..." -ForegroundColor Green
New-Item -ItemType Directory -Force -Path "build\bin\Release\assets\shaders" | Out-Null

# Copiar shaders si existen
if (Test-Path "assets\shaders") {
    Write-Host "[INFO] Copiando shaders..." -ForegroundColor Green
    Copy-Item "assets\shaders\*" -Destination "build\bin\Release\assets\shaders\" -Force
}

# Copiar modelos si existen
if (Test-Path "assets\models") {
    Write-Host "[INFO] Copiando modelos..." -ForegroundColor Green
    New-Item -ItemType Directory -Force -Path "build\bin\Release\assets\models" | Out-Null
    Copy-Item "assets\models\*" -Destination "build\bin\Release\assets\models\" -Force -Recurse
}

# Copiar texturas si existen
if (Test-Path "assets\textures") {
    Write-Host "[INFO] Copiando texturas..." -ForegroundColor Green
    New-Item -ItemType Directory -Force -Path "build\bin\Release\assets\textures" | Out-Null
    Copy-Item "assets\textures\*" -Destination "build\bin\Release\assets\textures\" -Force -Recurse
}

Write-Host ""
Write-Host "=== Iniciando Claude Engine Editor ===" -ForegroundColor Cyan
Write-Host ""

# Cambiar al directorio del ejecutable y ejecutar
Set-Location "build\bin\Release"
.\ClaudeEditor.exe

# Volver al directorio original
Set-Location "..\..\..\"

Write-Host ""
Write-Host "=== Editor cerrado ===" -ForegroundColor Cyan
