#ifndef UCT_H
#define UCT_H

#include <ctime>
#include <vector>
#include "MoveGenerator.h"
#include "utils.h"

class UCT {
 public:
  // 默认10秒的搜索算法
  UCT(int timeLimit = 10, double c = 0.5);
  ~UCT();
  void SearchAGoodMove(int Board[10][10]);

 private:
  void MakeMove(const ChessMove &move, int Board[10][10]);
  void UnMakeMove(const ChessMove &move, int Board[10][10]);
  double TreePolicy(ChessMove &move, int &parent_Ns);
  int DefaultPolicy(int side);
  int IsSimulateOver(int Board[10][10]);  // 判断仿真是否结束

  MoveGenerator *m_pMoveGenerator;
  double C;               // UCT 的常数系数
  int m_timeLimt;         // 时间限制，以秒为单位
  time_t m_beginTime;     // 开始时间
  time_t m_curTime;       // 当前时间
  bool m_ContinueSearch;  // 是否继续搜索
  int m_Board[10][10];

  // 自定义比较结构，用于构建最大堆
  struct cmp {
    bool operator()(const ChessMove &m1, const ChessMove &m2) {
      return m1.Qsa < m2.Qsa;
    }
  };
};

#endif  // UCT_H
