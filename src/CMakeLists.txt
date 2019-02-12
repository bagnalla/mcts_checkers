include_directories(${checkers_cpp_SOURCE_DIR}/include)

add_executable(checkers_cpp main.cc board.cc mcts.cc minimax.cc state.cc
  tree.cc util.cc)

target_link_libraries(checkers_cpp)
