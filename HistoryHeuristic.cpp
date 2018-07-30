#include "HistoryHeuristic.h"
#include <cstring>

HistoryHeuristic::HistoryHeuristic() {}

void HistoryHeuristic::ResetHistoryTable() {
  memset(m_HistoryTable, 0, sizeof(m_HistoryTable));
}

int HistoryHeuristic::GetHistoryScore(const ChessMove &move) {
  int nFrom = move.From.x * 10 + move.From.y;
  int nTo = move.To.x * 10 + move.To.y;
  int nBar = move.Bar.x * 10 + move.Bar.y;
  return m_HistoryTable[nFrom][nTo][nBar];
}

void HistoryHeuristic::EnterHistoryScore(const ChessMove &move, int depth) {
  int nFrom = move.From.x * 10 + move.From.y;
  int nTo = move.To.x * 10 + move.To.y;
  int nBar = move.Bar.x * 10 + move.Bar.y;
  m_HistoryTable[nFrom][nTo][nBar] += 2 << depth;
}
