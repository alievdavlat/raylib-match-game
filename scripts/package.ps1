# Builds a Release version and packs it into dist/raylib-match-game-windows.zip
# Usage (from the project folder):  powershell -ExecutionPolicy Bypass -File scripts\package.ps1

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$name = 'raylib-match-game-windows'
$build = Join-Path $root 'build\msys2-release'
$stage = Join-Path $root "dist\$name"
$zip = Join-Path $root "dist\$name.zip"

Set-Location $root
cmake --preset msys2-release
if ($LASTEXITCODE -ne 0) { throw 'CMake configure failed' }
cmake --build --preset msys2-release
if ($LASTEXITCODE -ne 0) { throw 'Build failed' }

if (Test-Path $stage) { Remove-Item $stage -Recurse -Force }
New-Item -ItemType Directory -Force $stage | Out-Null

Copy-Item (Join-Path $build 'match-game.exe') $stage
Copy-Item (Join-Path $build 'libraylib.dll') $stage
Copy-Item (Join-Path $build 'glfw3.dll') $stage
Copy-Item (Join-Path $build 'resources') $stage -Recurse
Copy-Item (Join-Path $root 'scripts\HOW-TO-PLAY.txt') $stage

if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path $stage -DestinationPath $zip

Write-Host "Created $zip"
