#include "NegaScout.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <queue>
#include <vector>

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
  // 生成所有的步法
  m_pMoveGenerator->CreatePossibleMove(m_Board, BLACK, MoveList);
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
  int flag = IsGameOver(m_Board);
  if (flag == BLACK) {
    return D_INF;
  } else if (flag == WHITE) {
    return -D_INF;
  }
  double score = m_pHashTable->LookUpHashTable(alpha, beta, depth);
  // 浮点数不能直接比较大小，而是使用误差方式判断相等比较安全
  if (fabs(score - NOT_HIT_TARGET) < eps) {
    return score;
  }
  if (depth <= 0) {
    score = m_pEvaluateEngine->evaluate(m_Board);
    m_pHashTable->EnterHashTalbe(exact, score, depth);  // 数据存入哈希表
    return score;
  }
  std::vector<ChessMove> MList;
  int side = 0;  // 走棋方
  if ((m_SearchDepth - depth) % 2 == 0) {
    side = BLACK;
  } else {
    side = WHITE;
  }
  m_pMoveGenerator->CreatePossibleMove(m_Board, side, MList);
  // 获取历史得分
  for (auto p : MList) {
    p.his_score = m_pHistoryHeuristic->GetHistoryScore(p);
  }
  // 按照历史得分排序
  std::sort(MList.begin(), MList.end(),
            [](const ChessMove &m1, const ChessMove &m2) {
              return m1.his_score > m2.his_score;
            });
  int cur = 0;  // 当前迭代器的位置
  int eval_is_exact = 0;
  ChessMove bestMove;  // 最佳走法
  for (auto p : MList) {
    if (!m_ContinueSearch) {  // 超时
      break;
    }
    m_pHashTable->Hash_MakeMove(p);  // 生成子节点的哈希值
    MakeMove(p);
    // 递归搜索，第一个是全窗口，其他空窗探测
    score = -NegaScoutSearch(depth - 1, -beta, -alpha);
    if (score > alpha && score < beta && cur > 0) {
      alpha = -NegaScoutSearch(depth - 1, -beta, -score);  // 重新搜索
      eval_is_exact = 1;
      bestMove = p;  // 记录最佳走法
    }
    m_pHashTable->Hash_UnMakeMove(p);
    if (alpha < score) {  // 首次搜索命中
      eval_is_exact = 1;
      alpha = score;
    }
    if (alpha >= beta) {
      m_pHashTable->EnterHashTalbe(lower_bound, alpha, depth);
      m_pHistoryHeuristic->EnterHistoryScore(p, depth);
      return alpha;  // beta剪枝
    }
    beta = alpha + 0.01;  // 设定新的空窗
    ++cur;
  }
  // 最佳走法加入历史记录
  m_pHistoryHeuristic->EnterHistoryScore(bestMove, depth);
  // 搜索结果加入置换表
  if (eval_is_exact) {
    m_pHashTable->EnterHashTalbe(exact, alpha, depth);
  } else {
    m_pHashTable->EnterHashTalbe(upper_bound, alpha, depth);
  }
  return alpha;
}
