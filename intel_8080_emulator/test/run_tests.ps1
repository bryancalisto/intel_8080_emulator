$testing_dir = "testing_env"
$cmd = $args[0]

if ($cmd -eq '-verbose') {
  # Run tests with verbose output
  make
  ctest -V
}
elseif ($cmd -eq '-new') {
  # Create testing environment with cmake and run tests
  mkdir $testing_dir
  Set-Location $testing_dir
  cmake ..
  make
  ctest
}
else {
  # Run tests
  make
  ctest
}