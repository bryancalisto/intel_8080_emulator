$root_dir = $pwd
$build_dir = "build"

If (!(test-path -PathType container $build_dir)) {
  New-Item -ItemType Directory -Path $build_dir
  Set-Location $build_dir
  cmake -GNinja ..
  Set-Location $root_dir
}

cmake --build $build_dir

Invoke-Expression ./$build_dir/intel_8080_emulator.exe