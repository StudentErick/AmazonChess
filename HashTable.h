#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "utils.h"

class HashTable {
 public:
  HashTable(int TableSize = 1024);
  ~HashTable();
  // 计算当前状态的棋盘哈希值
  void CalculateInitHashKey(int Board[10][10]);
  // 根据步法生成新的哈希值
  void Hash_MakeMove(const ChessMove &move);
  // 撤销所给走法的哈希值，还原成之前的状态
  void Hash_UnMakeMove(const ChessMove &move);
  //查询评估值
  double LookUpHashTable(double alpha, double beta, double depth);
  // 当前结点数据存入哈希表
  void EnterHashTalbe(entry_type EType, double value, int depth);
  // 初始化哈希表
  void InitializeHashKey();

 private:
  unsigned long long int m_HashKey32;               // 32位定位数据
  unsigned long long int m_HashKey64;               // 64位哈希校验值
  unsigned long long int m_HashBoard32[3][10][10];  // 32位数据表
  unsigned long long int m_HashBoard64[3][10][10];  // 64位数据表
  int m_nTableSize;

  HashItem *m_pHashTable;
};

#endif  // HASHTABLE_H
