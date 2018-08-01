#include "NegaScout.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
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
  for (auto &p : MoveList) {  // 注意使用引用符号&，auto不能转换成引用
    MakeMove(p);
    p.eval_score = m_pEvaluateEngine->evaluate(m_Board);
    UnMakeMove(p);
  }

  // 先排序，然后搜索，假设第一次评估的分数可以决定后续的
  std::sort(MoveList.begin(), MoveList.end(),
            [](const ChessMove &m1, const ChessMove &m2) {
              return m1.eval_score > m2.eval_score;
            });
  time(&m_beginTime);  // 获取搜索启动时间
  int n = 0;  // 用于计数的，因为如果后面的搜索不到，就没有太大的意义了
  for (auto &p : MoveList) {
    time(&m_curTime);                            // 获取当前系统时间
    if (m_curTime - m_beginTime > m_timeLimt) {  // 超时退出
      m_ContinueSearch = false;
      break;
    }
    MakeMove(p);
    p.eval_score = NegaScoutSearch(m_SearchDepth, -D_INF, D_INF);
    UnMakeMove(p);
    ++n;
  }
  int k = n;
  // 选择分值最高的走法
  m_BestMove = *MoveList.begin();
  for (auto &p : MoveList) {
    if (m_BestMove.eval_score < p.eval_score) {
      m_BestMove = p;
    }
    if (n < 0) break;
    --n;
  }
  std::cout << "搜索根节点的数量：" << k << std::endl;
}

double NegaScout::NegaScoutSearch(int depth, double alpha, double beta) {
  double score = IsGameOver(depth);
  if (fabs(score - 0.0) > eps) {
    return score;
  }
  score = m_pHashTable->LookUpHashTable(alpha, beta, depth);
  // 浮点数不能直接比较大小，而是使用误差方式判断相等比较安全
  if (fabs(score - NOT_HIT_TARGET) > eps) {
    return score;
  }
  // 获取系统时间
  time(&m_curTime);
  if (m_curTime - m_beginTime > m_timeLimt) {  // 超时退出
    m_ContinueSearch = false;
  }
  // 深度为0或者超时了
  if (depth <= 0 || !m_ContinueSearch) {
    score = m_pEvaluateEngine->evaluate(m_Board);
    // 数据存入哈希表
    m_pHashTable->EnterHashTalbe(exact, score, depth);
    // 负极大搜索对走棋方敏感，必须判断行棋方
    if ((m_SearchDepth - depth) % 2 == 0) {
      return score;
    } else {
      return -score;
    }
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
  for (auto &p : MList) {
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
  bool best = false;   // 是否有最佳走法
  double a = alpha, b = beta, t = 0.0;
  for (auto &p : MList) {
    m_pHashTable->Hash_MakeMove(p);  // 生成子节点的哈希值
    MakeMove(p);                     // 模拟走子
    // 递归搜索，第一个是全窗口，其他空窗探测
    t = -NegaScoutSearch(depth - 1, -b, -a);
    if (t > a && t < b && cur > 0) {
      a = -NegaScoutSearch(depth - 1, -beta, -t);  // 重新搜索
      eval_is_exact = 1;
      bestMove = p;  // 记录最佳走法
      best = true;
    }
    m_pHashTable->Hash_UnMakeMove(p);  // 恢复结点哈希值
    UnMakeMove(p);                     // 清空本次模拟
    if (a < t) {                       // 首次搜索命中
      eval_is_exact = 1;
      a = t;
    }
    if (a >= beta) {
      m_pHashTable->EnterHashTalbe(lower_bound, alpha, depth);
      m_pHistoryHeuristic->EnterHistoryScore(p, depth);
      return a;  // beta剪枝
    }
    b = a + 0.0001;  // 设定新的空窗
    ++cur;
  }

  // 最佳走法加入历史记录
  if (best) {
    m_pHistoryHeuristic->EnterHistoryScore(bestMove, depth);
  }
  // 搜索结果加入置换表
  if (eval_is_exact) {
    m_pHashTable->EnterHashTalbe(exact, alpha, depth);
  } else {
    m_pHashTable->EnterHashTalbe(upper_bound, alpha, depth);
  }
  return a;
}
