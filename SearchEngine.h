#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "EvaluateEngine.h"
#include "MoveGenerator.h"
#include "utils.h"

class SearchEngine {
 public:
  SearchEngine();
  virtual ~SearchEngine();
  // 博弈接口
  virtual void SearchAGoodMove(int Board[10][10]) = 0;

 protected:
  void MakeMove(const ChessMove &move);
  void UnMakeMove(const ChessMove &move);
  // 游戏结束，黑方胜返回BLACK，白方胜返回WHITE，没结束返回0
  int IsGameOver(int Board[10][10]);

  ChessMove m_BestMove;               // 最佳走法
  MoveGenerator *m_pMoveGenerator;    // 步法生成
  EvaluateEngine *m_pEvaluateEngine;  // 评估引擎
  int m_Board[10][10];                // 棋盘
  int m_SearchDepth;                  // 搜索深度
};

#endif  // SEARCHENGINE_H
