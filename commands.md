**Compile for example** : `g++ -std=c++17 -o example example.cpp lib/*.cpp`
**Run executable** : `./example`
**Compile for testing** : `g++ -std=c++17 -o tests -I/opt/homebrew/Cellar/googletest/1.14.0/include tests.cpp ./lib/*.cpp ./lib/tests/*.cpp -L/opt/homebrew/lib -lgtest -lgtest_main -pthread`
**Run tests** : `./tests`
