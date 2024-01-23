**Compile for example** : `g++ -std=c++17 -o example -I ./src example.cpp src/*.cpp`
**Run executable** : `./example`
**Compile for testing** : `g++ -std=c++17 -o tests -I ./src tests.cpp ./src/*.cpp ./src/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests** : `./tests`
