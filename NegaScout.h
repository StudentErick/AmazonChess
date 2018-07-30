#ifndef NEGASCOUT_H
#define NEGASCOUT_H

#include <ctime>
#include "EvaluateEngine.h"
#include "HashTable.h"
#include "HistoryHeuristic.h"
#include "MoveGenerator.h"
#include "SearchEngine.h"

class NegaScout : public SearchEngine {
 public:
  // 默认深度是5 时间最长10秒，传入时间以秒为单位
  NegaScout(int depth = 5, int timeLimit = 10);
  virtual ~NegaScout();
  virtual void SearchAGoodMove(int Board[10][10]);

 protected:
  // 根节点搜索，因为第一步有2000多步走法，开局分支太大，需要进行优化
  void RootSearch();
  // 常规的NegaScout搜索
  double NegaScoutSearch(int depth, double alpha, double beta);

  HashTable* m_pHashTable;                // 哈希表
  HistoryHeuristic* m_pHistoryHeuristic;  // 历史启发
  int m_timeLimt;                         // 时间限制，以秒为单位
  time_t m_beginTime;                     // 开始时间
  time_t m_curTime;                       // 当前时间
  bool m_ContinueSearch;                  // 是否继续搜索
  // 自定义比较函数，用于构造大顶堆
  struct cmp {
    bool operator()(ChessMove& m1, ChessMove& m2) {
      return m1.eval_score < m2.eval_score;
    }
  };
};

#endif  // NEGASCOUT_H
