#ifndef TABLE_H
#define TABLE_H

struct state{
  int pos[8] = {};
};

class table {
public:
  table();
  ~table();

  int tablesize;

  state currentstate;

  char possible[8][8];
  char board[8][8];

  int currentScore;
  void print();
  bool isValid(int row, int col);
  void solve();
  state* randomConfig();
  int calculateObjective(char theboard[8][8], state state);
  bool compareStates(state* state1, state* state2);
  void generateBoard(state* state);
  void copyState(state* state1, state* state2);
};

#include "table.cpp"
#endif
