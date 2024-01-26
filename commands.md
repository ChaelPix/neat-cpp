**Compile for example** : `g++ -std=c++17 -o example -I ./lib example.cpp lib/*.cpp`
**Run executable** : `./example`
**Compile for testing** : `g++ -std=c++17 -o tests -I ./lib tests.cpp ./lib/*.cpp ./lib/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests** : `./tests`
