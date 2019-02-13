# mcts_checkers
MCTS+UCB (UCT) agent for checkers in C++. Also includes a basic
alpha-beta minimax agent for playing against.

CMake is required.

To build Debug:
```
mkdir Debug
cd Debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
```

To build Release:
```
mkdir Release
cd Release
cmake -D CMAKE_BUILD_TYPE=Release ..
make
```
