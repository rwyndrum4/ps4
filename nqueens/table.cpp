#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

using namespace std;
/*
I used https://www.geeksforgeeks.org/n-queen-problem-local-search-using-hill-climbing-with-random-neighbour/
as guidance for recreating this scenario

*/

table::table(){
//  board = new char[8][8];//Initialize a new board of size 8x8.
  tablesize = 8;
  for(int i = 0; i < tablesize; i++){
    for(int j = 0; j < tablesize; j++){
      board[i][j] = '0';
      possible[i][j] = '0';
    }
  }
  srand(time(NULL));
/*
  for(int i = 0; i < tablesize; i++) {
        board[(rand()%8)][i] = 'Q';
  }
  for(int i = 0; i < tablesize; i++){
    for(int j = 0; j < tablesize; j++){
      if(board[i][j] == 'Q'){
        currentstate.pos[i] = j;
      }
    }
  }
*/
  for(int i = 0; i < tablesize; i++) {
    currentstate.pos[i] = (rand()%8);
  }
  for(int i = 0; i < tablesize; i++){
    board[currentstate.pos[i]][i] = 'Q';
  }
}

table::~table(){


}

void table::print(){
  for(int i = 0; i < tablesize; i++){
    for(int j = 0; j < tablesize; j++){
      cout << setw(3) << board[i][j];
    }
    cout << "\n";
  }
  cout << "Current state: { ";
  for(int i = 0; i < tablesize; i++){
    cout << currentstate.pos[i] << " ";
  }
  cout << "}\n";
  int score = calculateObjective(board, currentstate);
  std::cout << "Score: " << score << "\n";
}

bool table::isValid(int row, int col){
  for (int i = 0; i < col; i++){//check whether there is queen to the left
      if (board[row][i] == 'Q'){
        return false;
      }
    }
  for(int i=row, j=col; i>=0 && j>=0; i--, j--){
      if (board[i][j] == 'Q'){ //check whether there is queen in the left upper diagonal
         return false;
       }
    }
   for (int i=row, j=col; j>=0 && i<tablesize; i++, j--){
      if (board[i][j] == 'Q'){ //check whether there is queen in the left lower diagonal
         return false;
       }
    }
   return true;
}

state* table::randomConfig(){
  srand(time(NULL));
  state* response;
  for(int i = 0; i < tablesize; i++){
    response->pos[i] = rand() % 8;
  }
  return(response);
}

bool table::compareStates(state* state1, state* state2){
  for(int i = 0; i < tablesize; i++){
    if(state1->pos[i] != state2->pos[i]){
      return false;
    }
  }
  return true;
}

int table::calculateObjective(char board1[8][8],state state1)
{
    // Number of queens attacking each other,
    // initially zero.
    int attacking = 0;

    // Variables to index a particular
    // row and column on board.
    int row, col;
    for (int i = 0; i < tablesize; i++) {

        // At each column 'i', the queen is
        // placed at row 'state[i]', by the
        // definition of our state.

        // To the left of same row
        // (row remains constant
        // and col decreases)
        row = state1.pos[i], col = i - 1;
        while (col >= 0 && board1[row][col] != 'Q') {
            col--;
        }
        if (col >= 0 && board1[row][col] == 'Q') {
            attacking++;
        }

        // To the right of same row
        // (row remains constant
        // and col increases)
        row = state1.pos[i], col = i + 1;
        while (col < tablesize && board1[row][col] != 'Q') {
            col++;
        }
        if (col < tablesize && board1[row][col] == 'Q') {
            attacking++;
        }

        // Diagonally to the left up
        // (row and col simultaneously
        // decrease)
        row = state1.pos[i] - 1, col = i - 1;
        while (col >= 0 && row >= 0 && board1[row][col] != 'Q') {
            col--;
            row--;
        }
        if (col >= 0 && row >= 0 && board1[row][col] == 'Q') {
            attacking++;
        }

        // Diagonally to the right down
        // (row and col simultaneously
        // increase)
        row = state1.pos[i] + 1, col = i + 1;
        while (col < tablesize && row < tablesize && board1[row][col] != 'Q') {
            col++;
            row++;
        }
        if (col < tablesize && row < tablesize && board1[row][col] == 'Q') {
            attacking++;
        }

        // Diagonally to the left down
        // (col decreases and row
        // increases)
        row = state1.pos[i] + 1, col = i - 1;
        while (col >= 0 && row < tablesize && board1[row][col] != 'Q') {
            col--;
            row++;
        }
        if (col >= 0 && row < tablesize && board1[row][col] == 'Q') {
            attacking++;
        }

        // Diagonally to the right up
        // (col increases and row
        // decreases)
        row = state1.pos[i] - 1, col = i + 1;
        while (col < tablesize && row >= 0 && board1[row][col] != 'Q') {
            col++;
            row--;
        }
        if (col < tablesize && row >= 0 && board1[row][col] == 'Q') {
            attacking++;
        }
    }
    // Return pairs.
    currentScore = attacking / 2;
    return (int)(attacking / 2);
}
void table::generateBoard(state* state1){
  for (int i = 0; i < tablesize; i++) {
        possible[state1->pos[i]][i] = 'Q';
    }
}

void table::copyState(state* state1, state* state2)
{
    for (int i = 0; i < tablesize; i++) {
        state1->pos[i] = state2->pos[i];
    }
}

void table::solve(){
  do{
    state neighbour = {};

    //for(int i = 0; i < tablesize; i++) {
      int randomspot = (rand()%8);
      for(int i = 0; i < 8; i++){
      if(i == randomspot){
      neighbour.pos[randomspot] = randomspot;
      }
      else{
        neighbour.pos[i] = currentstate.pos[i];
      }
    }
    //}
    for(int i = 0; i < tablesize; i++){
      possible[neighbour.pos[i]][i] = 'Q';
    }
    int score1 = calculateObjective(possible, neighbour);
    int score2 = calculateObjective(board, currentstate);
    if(score1 < score2){ //if new board is better, swap them and continue
      for(int i = 0; i < tablesize; i++){
        for(int j = 0; j < tablesize; j++){
          board[i][j] = possible[i][j];
        }
      }
      for(int i = 0; i < tablesize; i++){
        currentstate.pos[i] = neighbour.pos[i];
      }
        print();
    }
    //print();

  }while(currentScore != 0);
}
