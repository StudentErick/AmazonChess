#ifndef UTILS_H
#define UTILS_H

// 棋子等的数据标示
const int BLACK = -1;
const int WHITE = -2;
const int BARRIER = -3;
const int EMPTY = 0;
const double D_INF = 10000000.0;
const int INF = 10000000;
const double NOT_HIT_TARGET = 666666.0;
const double eps = 0.00000001;

// 8个偏移方向
const int offset[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};

// 精确，下边界，上边界
enum entry_type { exact, lower_bound, upper_bound };

struct HashItem {
  unsigned long long checksum;  // 64位校验码
  int depth;                    // 取得该值时的搜索深度
  double eval;                  // 结点的值
  entry_type EntryType;
};
// 位置坐标
struct Pos {
  int x, y;
};
// 步法信息
struct ChessMove {
  Pos From;           // 起点坐标
  Pos To;             // 终点坐标
  Pos Bar;            // 障碍坐标
  double eval_score;  // 评估分数
  int his_score;      // 历史启发得分
  int chessID;        // 棋子的颜色
  double win_prob;    // 蒙特卡洛的胜率
  double Qsa;         // 论文中的Q(s,a)
  int Nsa;            // 论文中的N(s,a)
  int Ns;             // 论文中的Ns
  ChessMove() {
    his_score = 0;
    win_prob = 0.0;
    eval_score = -D_INF;
    Qsa = 0.0;
    Nsa = 0.0;
    chessID = BLACK;  // 默认黑方走
  }
};

#endif  // UTILS_H
