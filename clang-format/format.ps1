# 1. 起始目录 = 当前目录的父目录
$root = Split-Path -Parent (Get-Location)

# 2. 强制使用脚本目录的 .clang-format
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$config = Join-Path $scriptDir ".clang-format"
$configArg = "--style=""file:$config"""

# 3. 获取所有 C/C++ 文件（仅排除 $root\build\）
$buildDir = Join-Path $root "build"

$files = Get-ChildItem -Path $root -Recurse -File -Include *.c, *.h, *.cpp, *.hpp |
    Where-Object { $_.FullName -notlike "$buildDir\*" }

foreach ($f in $files) {
    Write-Host "Formatting $($f.FullName)"
    clang-format -i $configArg --assume-filename=foo.cpp $f.FullName
}
