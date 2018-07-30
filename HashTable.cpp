#include "HashTable.h"
#include <cstdlib>
#include <random>

static std::mt19937 mt(01234567);

// Generates a Randome number from 0 to 2^64-1
unsigned long long int randomInt64() {
  std::uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX);
  return dist(mt);
}

// Generates a Random number from 0 to 2^32-1
unsigned long long int randomInt32() {
  std::uniform_int_distribution<unsigned long long int> dist(0, UINT32_MAX);
  return dist(mt);
}

HashTable::HashTable(int TableSize) {
  m_nTableSize = TableSize;
  InitializeHashKey();
}

HashTable::~HashTable() {
  delete[] m_pHashTable;
  m_pHashTable = nullptr;
}

void HashTable::InitializeHashKey() {
  m_HashKey32 = 0;
  m_HashKey64 = 0;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 10; ++j) {
      for (int k = 0; k < 10; ++k) {
        m_HashBoard32[i][j][k] = randomInt32();
        m_HashBoard64[i][j][k] = randomInt64();
      }
    }
  }
  m_pHashTable = new HashItem[m_nTableSize * m_nTableSize];
}

void HashTable::CalculateInitHashKey(int Board[10][10]) {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 10; ++j) {
      for (int k = 0; k < 10; ++k) {
        int side = Board[i][j];
        if (side != EMPTY) {
          // 因为不是从0~3，而是-1 -2 -3，因此要+3
          m_HashKey32 = m_HashKey32 ^ m_HashBoard32[side + 3][j][k];
          m_HashKey64 = m_HashKey64 ^ m_HashBoard64[side + 3][j][k];
        }
      }
    }
  }
}

void HashTable::Hash_MakeMove(const ChessMove &move) {
  int side = move.chessID + 3;
  m_HashKey32 = m_HashKey32 ^ m_HashBoard32[side][move.From.x][move.From.y];
  m_HashKey64 = m_HashKey64 ^ m_HashBoard64[side][move.From.x][move.From.y];
  m_HashKey32 = m_HashKey32 ^ m_HashBoard32[side][move.To.x][move.To.y];
  m_HashKey64 = m_HashKey64 ^ m_HashBoard64[side][move.To.x][move.To.y];
  m_HashKey32 =
      m_HashKey32 ^ m_HashBoard32[BARRIER + 3][move.Bar.x][move.Bar.y];
  m_HashKey64 =
      m_HashKey64 ^ m_HashBoard64[BARRIER + 3][move.Bar.x][move.Bar.y];
}

void HashTable::Hash_UnMakeMove(const ChessMove &move) {
  int side = move.chessID + 3;
  // 操作与前一个相反
  m_HashKey32 =
      m_HashKey32 ^ m_HashBoard32[BARRIER + 3][move.Bar.x][move.Bar.y];
  m_HashKey64 =
      m_HashKey64 ^ m_HashBoard64[BARRIER + 3][move.Bar.x][move.Bar.y];
  m_HashKey32 = m_HashKey32 ^ m_HashBoard32[side][move.To.x][move.To.y];
  m_HashKey64 = m_HashKey64 ^ m_HashBoard64[side][move.To.x][move.To.y];
  m_HashKey32 = m_HashKey32 ^ m_HashBoard32[side][move.From.x][move.From.y];
  m_HashKey64 = m_HashKey64 ^ m_HashBoard64[side][move.From.x][move.From.y];
}

double HashTable::LookUpHashTable(double alpha, double beta, double depth) {
  auto x = m_HashKey32 %
           static_cast<unsigned long long>(m_nTableSize * m_nTableSize);
  HashItem *pht = &m_pHashTable[x];
  if (pht->depth >= depth && pht->checksum == m_HashKey64) {
    switch (pht->EntryType) {
      case exact:
        return pht->eval;
      case lower_bound:
        if (pht->eval >= beta) {
          return pht->eval;
        } else {
          break;
        }
      case upper_bound:
        if (pht->eval <= alpha) {
          return pht->eval;
        } else {
          break;
        }
    }
  }
  return NOT_HIT_TARGET;
}

void HashTable::EnterHashTalbe(entry_type EntryType, double eval, int depth) {
  auto x = m_HashKey32 %
           static_cast<unsigned long long>(m_nTableSize * m_nTableSize);
  HashItem *pht = &m_pHashTable[x];
  pht->checksum = m_HashKey64;
  pht->EntryType = EntryType;
  pht->eval = eval;
  pht->depth = depth;
}
