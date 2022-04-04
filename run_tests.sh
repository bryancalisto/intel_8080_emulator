testing_dir=testing_env

if [[ $1 == '-V' ]] # Run tests with verbose output
then
  make && ctest -V
elif [[ $1 == '-N' ]] # Create testing environment with cmake and run tests
then
  mkdir $testing_dir
  cd $testing_dir
  cmake .. && make && ctest
else # Run tests
  make && ctest
fi