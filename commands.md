**Compile for training** : `g++ -std=c++17 -o main -I./src main.cpp src/*.cpp`
**Run executable** : `./main`
**Compile for testing** : `g++ -std=c++17 -o tests -I./src tests.cpp ./src/*.cpp ./src/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests** : `./tests`
