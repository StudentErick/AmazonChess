#ifndef UTILS_H
#define UTILS_H

// 定义了常用的数据结构
namespace DataStructure {
// 棋子等的数据标示
const int BLACK=-1;
const int WHITE=-2;
const int BARRIER=-3;
const int EMPTY=0;
const int INF=10000000;
const int NOT_HIT_TARGET=66666;

// 8个偏移方向
const int offset[8][2]={-1,-1,-1,0,-1,1,
                  0,-1,0,0,
                  1,-1,1,0,1,1};
// 位置坐标
struct Pos{
  int x,y  ;
};
// 步法信息
struct ChessMove{
    Pos From;    // 起点坐标
    Pos To;      // 终点坐标
    Pos Bar;     // 障碍坐标
    int score;   // 历史得分
    int chessID; // 行棋方
    ChessMove(){
        score=0;
        chessID=BLACK;  // 默认方走
    }
};
}

#endif // UTILS_H
