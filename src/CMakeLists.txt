include_directories(${mcts_checkers_SOURCE_DIR}/include)

add_executable(mcts_checkers main.cc board.cc mcts.cc minimax.cc
  state.cc tree.cc)

target_link_libraries(mcts_checkers)
