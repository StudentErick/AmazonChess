#include <unistd.h>
#include <ctime>
#include <iostream>
#include <typeinfo>
#include "utils.h"

using namespace std;

/*
├ ┼ ─ ┬ ┴ └ ┌ ┘ │
*/
void showBoard(int Board[10][10]) {
  std::cout << "    0   1   2   3   4   5   6   7   8   9" << endl;
  cout << "  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
  for (int i = 0; i < 10; ++i) {
    cout << i << " │";
    for (int j = 0; j < 10; ++j) {
      if (Board[i][j] == BLACK) {
        cout << " ○ │";
      } else if (Board[i][j] == WHITE) {
        cout << " ● │";
      } else if (Board[i][j] == BARRIER) {
        cout << " ▲ │";
      } else {
        cout << "   │";
      }
    }
    if (i != 9) {
      cout << "\n  ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
    } else {
      cout << "\n  └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
    }
  }
}

// static int Board[10][10];

int main() {
  time_t a;
  time(&a);
  // sleep(1);
  time_t b;
  time(&b);
  cout << typeid(b - a).name() << endl;
  long c = 0;
  cout << typeid(c).name() << endl;
  return 0;
}
