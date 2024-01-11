**Compile** : `g++ -std=c++14 -o main -I./src main.cpp src/*.cpp`
**Run executable** : `./main`
**Compile with tests** : `g++ -std=c++14 -o tests -I./src tests.cpp ./src/*.cpp ./src/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests** : `./tests`
