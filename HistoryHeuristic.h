#ifndef HISTORYHEURISTIC_H
#define HISTORYHEURISTIC_H

#include "utils.h"

class HistoryHeuristic {
 public:
  HistoryHeuristic();
  // 清空历史得分表
  void ResetHistoryTable();
  // 获取某个走法的历史得分
  int GetHistoryScore(const ChessMove &move);
  // 最佳走法加入历史得分表
  void EnterHistoryScore(const ChessMove &move, int depth);

 private:
  int m_HistoryTable[100][100][100];  // 历史得分表
};

#endif  // HISTORYHEURISTIC_H
