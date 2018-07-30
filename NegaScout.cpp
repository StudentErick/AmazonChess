#include "NegaScout.h"
#include <cstring>
#include <queue>

NegaScout::NegaScout(int depth, int timeLimit) {
  m_SearchDepth = depth;
  m_pEvaluateEngine = new EvaluateEngine;
  m_pMoveGenerator = new MoveGenerator;
  m_pHashTable = new HashTable;
  m_pHistoryHeuristic = new HistoryHeuristic;
  m_timeLimt = timeLimit;
  m_ContinueSearch = true;
  m_beginTime = 0;
  m_curTime = 0;
}

NegaScout::~NegaScout() {
  if (m_pHashTable != nullptr) {
    delete m_pHashTable;
    m_pHashTable = nullptr;
  }
  if (m_pHistoryHeuristic != nullptr) {
    delete m_pHistoryHeuristic;
    m_pHistoryHeuristic = nullptr;
  }
}

void NegaScout::SearchAGoodMove(int Board[10][10]) {
  memcpy(m_Board, Board, sizeof(m_Board));
  m_pHashTable->CalculateInitHashKey(m_Board);  // 计算初始化的哈希值
  m_pHistoryHeuristic->ResetHistoryTable();     // 初始化历史启发值
  RootSearch();
  MakeMove(m_BestMove);
  memcpy(Board, m_Board, sizeof(m_Board));
}

void NegaScout::RootSearch() {
  std::vector<ChessMove> MoveList;
  m_pMoveGenerator->CreatePossibleMove(m_Board, BLACK,
                                       MoveList);  // 生成所有的步法
  // 评估结点的分数
  for (auto p : MoveList) {
    MakeMove(p);
    p.eval_score = m_pEvaluateEngine->evaluate(m_Board);
    UnMakeMove(p);
  }
  // 先进行一次评估，然后根据第一步最佳值把最佳步法进行堆化，防止全部搜索超时
  std::priority_queue<ChessMove, std::vector<ChessMove>, cmp> MoveQueue(
      MoveList.begin(), MoveList.end());

  time(&m_beginTime);  // 获取搜索启动时间

  // 队列不空而且没有超时的情况
  while (!MoveQueue.empty() && m_ContinueSearch) {
    auto move = MoveQueue.top();
    MoveQueue.pop();
    MakeMove(move);
    move.eval_score = NegaScoutSearch(m_SearchDepth, -D_INF, D_INF);
    UnMakeMove(move);
    MoveQueue.push(move);
    time(&m_curTime);  // 获取当前系统时间
    // 超时处理
    if (m_curTime - m_beginTime > m_timeLimt) {
      m_ContinueSearch = false;
    }
  }
  m_BestMove = MoveQueue.top();  // 获取最佳走法
}

double NegaScout::NegaScoutSearch(int depth, double alpha, double beta) {
  return 0.0;
}
