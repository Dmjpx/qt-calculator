param([string]$QtRoot = 'D:\Qt', [ValidateSet('Debug', 'Release')][string]$BuildType = 'Release')
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
$qt = Join-Path $QtRoot '6.11.1\llvm-mingw_64'
$compiler = Join-Path $QtRoot 'Tools\llvm-mingw1706_64\bin'
$cmake = Join-Path $QtRoot 'Tools\CMake_64\bin\cmake.exe'
$ninja = Join-Path $QtRoot 'Tools\Ninja\ninja.exe'
$env:PATH = "$compiler;$(Join-Path $qt 'bin');$env:PATH"
& $cmake --fresh -S . -B build -G Ninja "-DCMAKE_BUILD_TYPE=$BuildType" "-DCMAKE_PREFIX_PATH=$qt" "-DCMAKE_CXX_COMPILER=$compiler/clang++.exe" "-DCMAKE_MAKE_PROGRAM=$ninja"
if ($LASTEXITCODE -ne 0) { throw 'CMake configure failed.' }
& $cmake --build build
if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }

# Resolve imports recursively. This also works when windeployqt cannot handle
# the non-ASCII project directory with this Qt distribution.
New-Item -ItemType Directory -Path dist, dist\platforms -Force | Out-Null
Copy-Item -LiteralPath 'build\吴小东的计算器.exe' -Destination dist
Copy-Item -LiteralPath (Join-Path $qt 'plugins\platforms\qwindows.dll') -Destination dist\platforms
$reader = Join-Path $compiler 'llvm-readobj.exe'
$queue = [System.Collections.Generic.Queue[string]]::new()
$queue.Enqueue((Join-Path $PSScriptRoot 'dist\吴小东的计算器.exe'))
$queue.Enqueue((Join-Path $PSScriptRoot 'dist\platforms\qwindows.dll'))
$seen = @{}
while ($queue.Count -gt 0) {
    $file = $queue.Dequeue()
    $imports = & $reader --coff-imports $file
    if ($LASTEXITCODE -ne 0) { throw "Cannot inspect $file" }
    foreach ($line in $imports) {
        if ($line -match '^\s+Name: (.+\.dll)$') {
            $name = $Matches[1]
            if ($seen.ContainsKey($name)) { continue }
            $seen[$name] = $true
            foreach ($folder in @((Join-Path $qt 'bin'), $compiler)) {
                $source = Join-Path $folder $name
                if (Test-Path -LiteralPath $source) {
                    Copy-Item -LiteralPath $source -Destination dist
                    $queue.Enqueue((Join-Path $PSScriptRoot "dist\$name"))
                    break
                }
            }
        }
    }
}
@('[Paths]', 'Prefix=.', 'Plugins=.') | Set-Content -LiteralPath dist\qt.conf -Encoding ascii
Write-Host 'Build and deployment completed: dist'
