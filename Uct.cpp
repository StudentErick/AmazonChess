#include "Uct.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <queue>

UCT::UCT(int timeLimit, double c) {
  C = c;
  m_timeLimt = timeLimit;
  m_pMoveGenerator = new MoveGenerator;
}

UCT::~UCT() {
  if (m_pMoveGenerator != nullptr) {
    delete m_pMoveGenerator;
    m_pMoveGenerator = nullptr;
  }
}

void UCT::SearchAGoodMove(int Board[10][10]) {
  memcpy(m_Board, Board, sizeof(m_Board));
  std::vector<ChessMove> MoveList;
  m_pMoveGenerator->CreatePossibleMove(m_Board, BLACK, MoveList);
  std::priority_queue<ChessMove, std::vector<ChessMove>, cmp> MoveQueue(
      MoveList.begin(), MoveList.end());
  while (true) {
  }
}

void UCT::MakeMove(const ChessMove &move, int Board[10][10]) {
  Board[move.From.x][move.From.y] = EMPTY;
  Board[move.To.x][move.To.y] = move.chessID;
  Board[move.Bar.x][move.Bar.y] = BARRIER;
}

void UCT::UnMakeMove(const ChessMove &move, int Board[10][10]) {
  Board[move.From.x][move.From.y] = move.chessID;
  Board[move.To.x][move.To.y] = EMPTY;
  Board[move.Bar.x][move.Bar.y] = EMPTY;
}

int UCT::IsSimulateOver(int Board[10][10]) {
  int nBlack = 0, nWhite = 0;  // 被堵死的棋子的数量
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (Board[i][j] != EMPTY && Board[i][j] != BARRIER) {
        int n = 0;
        // 8个方向判别
        for (int k = 0; k < 8; ++k) {
          int a = i + offset[k][0];
          int b = j + offset[k][1];
          if (a < 0 || a > 9 || b < 0 || b > 9 || Board[a][b] != EMPTY) {
            ++n;
          }
        }
        // 8个方向都被堵死，判别棋子的种类
        if (n >= 8) {
          if (Board[i][j] == BLACK) {
            ++nBlack;
          } else {
            ++nWhite;
          }
        }
      }
    }
  }

  if (nBlack >= 4) {  // 黑子被堵死了
    return -1;
  } else if (nWhite >= 4) {  // 白子被堵死了
    return 1;
  } else {  // 未结束
    return 0;
  }
}

double UCT::TreePolicy(ChessMove &move, int &parent_Ns) {
  double Qsa = 0.0;
  ++parent_Ns;  // 访问子结点，父结点的访问次数必然增加一个
  time(&m_curTime);  // 获取系统时间
  // 超时结束
  if (m_curTime - m_beginTime > m_timeLimt) {
    return Qsa;
  }
  return 0.0;
}

int UCT::DefaultPolicy(int side) {
  int board[10][10];
  memcpy(board, m_Board, sizeof(board));
  while (IsSimulateOver(board) == 0) {
    std::vector<ChessMove> movelist;
    m_pMoveGenerator->CreatePossibleMove(board, side, movelist);
    auto i = rand() % static_cast<int>(movelist.size());
    //随机选择一个步法，并走出
    MakeMove(movelist[static_cast<unsigned long>(i)], board);
    side = (-3 - side);  // 转换行棋方
    movelist.clear();    // 清空步法队列
  }
  return IsSimulateOver(board);  // 返回模拟值 -1或者1
}
