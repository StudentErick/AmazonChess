#include "EvaluateEngine.h"
#include "utils.h"
#include <queue>
#include <cmath>
#include <cstring>
using namespace DataStructure;
CEvaluateEngine::CEvaluateEngine()
{
    //需要改进的参数
    K = 0.11;/////////////////////////////////////////////////////////////////////
    //定义8个偏移方向
    /*
    offset[0][0] = -1; offset[0][1] = -1;
    offset[1][0] = -1; offset[1][1] = 0;
    offset[2][0] = -1; offset[2][1] = 1;
    offset[3][0] = 0; offset[3][1] = -1;
    offset[4][0] = 0; offset[4][1] = 1;
    offset[5][0] = 1; offset[5][1] = -1;
    offset[6][0] = 1; offset[6][1] = 0;
    offset[7][0] = 1; offset[7][1] = 1;
    */
}


CEvaluateEngine::~CEvaluateEngine()
{
}


void CEvaluateEngine::calculate_kingMove()
{
    struct P
    {
        int x, y;   //x y坐标
        int dis;    //距离
    };
    int visit[10][10];   //是否访问的标记
    //////////这里是计算黑子的kingmove的/////////////////////////////////////
    memcpy(kingMoveBlack, Board, sizeof(Board));  //复制信息
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (kingMoveBlack[i][j] == BLACK)
            {
                memset(visit, 0, sizeof(visit));    //初始化访问标记
                visit[i][j] = 1;
                std::queue<P>que;
                P pos, pos1;
                pos.x = i; pos.y = j;
                pos.dis = 0;
                que.push(pos);
                visit[i][j] = 1;
                while (!que.empty())
                {
                    int m, n;
                    pos1 = que.front();
                    que.pop();
                    for (int k = 0; k < 8; k++)
                    {
                        m = pos1.x + offset[k][0];
                        n = pos1.y + offset[k][1];
                        if (m>=0&&n>=0&&m<10&&n<10&&visit[m][n] == 0 && kingMoveBlack[m][n] >= 0)
                        {
                            visit[m][n] = 1;  //标记为访问过的
                            pos.x = m; pos.y = n;
                            pos.dis = pos1.dis + 1;
                            que.push(pos);
                            if (kingMoveBlack[m][n] == EMPTY || (kingMoveBlack[m][n]>pos.dis))
                                kingMoveBlack[m][n] = pos.dis;
                        }
                    }
                }
            }
        }
    }
    /////从这里开始是计算白子的kingmove的/////////////////////////////////////////////////////
    memcpy(kingMoveWhite, Board, sizeof(Board));  //复制信息
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (kingMoveWhite[i][j] == WHITE)
            {
                memset(visit, 0, sizeof(visit));    //初始化访问标记
                visit[i][j] = 1;
                std::queue<P>que;
                P pos, pos1;
                pos.x = i; pos.y = j;
                pos.dis = 0;
                que.push(pos);
                visit[i][j] = 1;
                while (!que.empty())
                {
                    int m, n;
                    pos1 = que.front();
                    que.pop();
                    for (int k = 0; k < 8; k++)
                    {
                        m = pos1.x + offset[k][0];
                        n = pos1.y + offset[k][1];
                        if (m>=0&&m<10&&n>=0&&n<10&&visit[m][n] == 0 && kingMoveWhite[m][n] >= 0)
                        {
                            visit[m][n] = 1;  //标记为访问过的
                            pos.x = m; pos.y = n;
                            pos.dis = pos1.dis + 1;
                            que.push(pos);
                            if (kingMoveWhite[m][n] == EMPTY || (kingMoveWhite[m][n]>pos.dis))
                                kingMoveWhite[m][n] = pos.dis;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (kingMoveBlack[i][j] == EMPTY)
            {
                kingMoveBlack[i][j] = INF;
            }
            if (kingMoveWhite[i][j] == EMPTY)
            {
                kingMoveWhite[i][j] == INF;
            }
        }
    }
}


void CEvaluateEngine::calculate_queenMove()
{
    struct P
    {
        int x, y;   //x y坐标
        int dis;    //距离
    };
    //从这里向下是计算queenBlackMove//////////////////////////////////////
    memcpy(queenMoveBlack, Board, sizeof(Board));
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == BLACK)
            {
                std::queue<P>que;
                P pos, pos1;
                pos.x = i;
                pos.y = j;
                pos.dis = 0;
                que.push(pos);
                while (!que.empty())
                {
                    pos1 = que.front();
                    pos1.dis++;
                    que.pop();
                    for (int k = 0; k < 8; k++)
                    {
                        int m = pos1.x + offset[k][0];
                        int n = pos1.y + offset[k][1];
                        while (m>=0&&m<10&&n>=0&&n<10&&queenMoveBlack[m][n] >= 0)
                        {
                            if (queenMoveBlack[m][n] == 0 || queenMoveBlack[m][n] > pos1.dis)
                            {
                                queenMoveBlack[m][n] = pos1.dis;
                                pos.x = m;
                                pos.y = n;
                                pos.dis = pos1.dis;
                                que.push(pos);
                            }
                            m += offset[k][0];
                            n += offset[k][1];
                        }
                    }
                }
            }
        }
    }
    //从这里向下是计算queenWhiteMove//////////////////////////////////////
    memcpy(queenMoveWhite, Board, sizeof(Board));
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == WHITE)
            {
                std::queue<P>que;
                P pos, pos1;
                pos.x = i;
                pos.y = j;
                pos.dis = 0;
                que.push(pos);
                while (!que.empty())
                {
                    pos1 = que.front();
                    pos1.dis++;
                    que.pop();
                    for (int k = 0; k < 8; k++)
                    {
                        int m = pos1.x + offset[k][0];
                        int n = pos1.y + offset[k][1];
                        while (m>=0&&m<10&&n>=0&&n<10&&queenMoveWhite[m][n] >= 0)
                        {
                            if (queenMoveBlack[m][n] == 0 || queenMoveBlack[m][n] > pos1.dis)
                            {
                                queenMoveBlack[m][n] = pos1.dis;
                                pos.x = m;
                                pos.y = n;
                                pos.dis = pos1.dis;
                                que.push(pos);
                            }
                            m += offset[k][0];
                            n += offset[k][1];
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (queenMoveBlack[i][j] == EMPTY)
            {
                queenMoveBlack[i][j] = INF;
            }
            else if (queenMoveWhite[i][j] == EMPTY)
            {
                queenMoveWhite[i][j] = INF;
            }
        }
    }
}


void CEvaluateEngine::calculate_t1()
{
    t1 = 0;
    double q1 = 0, q2 = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == EMPTY)
            {
                q1 = queenMoveBlack[i][j];
                q2 = queenMoveBlack[i][j];
                if (q1 == INF&&q2 == INF)
                {
                    t1 += 0;
                }
                else if (q1 < INF&&q1 == q2)
                {
                    t1 += K;
                }
                else if (q1 < q2)
                {
                    t1 += 1;
                }
                else if (q1 > q2)
                {
                    t1 -= 1;
                }
            }
        }
    }
}


void CEvaluateEngine::calculate_t2()
{
    double q1, q2;
    t2 = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == EMPTY)
            {
                q1 = kingMoveBlack[i][j];
                q2 = kingMoveWhite[i][j];
                if (q1 == INF&&q2 == INF)
                {
                    t2 += 0;
                }
                else if (q1 == q2&&q1 < INF)
                {
                    t2 += K;
                }
                else if (q1 < q2)
                {
                    t2 += 1;
                }
                else if (q1 > q2)
                {
                    t2 -= 1;
                }
            }
        }
    }
}


void CEvaluateEngine::calculate_c1()
{
    c1 = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == EMPTY)
            {
                double q1 = queenMoveBlack[i][j];
                double q2 = queenMoveWhite[i][j];
                c1 += pow(2, -q1) - pow(2, -q2);
            }
        }
    }
    c1 /= 2;
}


void CEvaluateEngine::calculate_c2()
{
    c2 = 0;
    double q1, q2;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == EMPTY)
            {
                q1 = kingMoveBlack[i][j];
                q2 = kingMoveWhite[i][j];
                double x = (q2 - q1) / 6;
                double y = x > -1 ? x : -1;
                c2 += (1 < y ? 1 : y);
            }
        }
    }
}


void CEvaluateEngine::calculate_w()
{
    w = 0;
    double q1, q2;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            q1 = queenMoveBlack[i][j];
            q2 = kingMoveBlack[i][j];
            w += pow(2, -fabs(q1 - q2));
        }
    }
}


void CEvaluateEngine::calculate_m()
{
    m = 0;
    memcpy(mobility, Board, sizeof(Board));
    //计算空格的灵活度
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Board[i][j] == EMPTY)
            {
                for (int k = 0; k < 8; k++)
                {
                    int a = i + offset[k][0], b = j + offset[k][1];
                    //	if (Board[i][j] == EMPTY)   //任然有一些问题
                    if(a>=0&&a<10&&b>=0&&b<10&&Board[a][b]==EMPTY)
                    {
                        mobility[i][j]++;
                    }
                }
            }
        }
    }
    //计算棋子的灵活度
    double m_black = 0, m_white = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            //黑棋的灵活度
            if (Board[i][j]==BLACK)
            {
                for (int k = 0; k < 8; k++)  //8个方向
                {
                    int a = i + offset[k][0], b = offset[k][1];
                    while (a>=0&&a<10&&b>=0&&b<10&&Board[a][b] == EMPTY)
                    {
                        m_black += queenMoveBlack[a][b] / kingMoveBlack[a][b];
                        a += offset[k][0];
                        b += offset[k][1];
                    }
                }

            }
            //白棋的灵活度
            else if (Board[i][j] == WHITE)
            {
                for (int k = 0; k < 8; k++)
                {
                    int a = i + offset[k][0], b = offset[k][1];
                    while (a>=0&&a<10&&b>=0&&b<10&&Board[a][b] == EMPTY)
                    {
                        if(kingMoveWhite[a][b]==0)continue; //处理浮点数异常，暂时没有解决
                        m_white += queenMoveWhite[a][b] / kingMoveWhite[a][b];
                        a += offset[k][0];
                        b += offset[k][1];
                    }
                }
            }
        }
    }
    m = m_black - m_white;
}


void CEvaluateEngine::calculate_s()
   {
        int N1, N2, M1,M2,i,j;
        N1 = N2 = 0;
        for (i = 1; i <=10; i++)
        {
            for (j = 1; j <=10; j++)
            {
                if (Board[i][j] == EMPTY)
                {
                    if (queenMoveBlack[i][j] < queenMoveWhite[i][j])
                    {
                        if (queenMoveBlack[i][j] == EMPTY)
                        {
                            M2++;
                            N2 += mobility[i][j];
                        }
                        else
                        {
                            M1++;
                            N1 += mobility[i][j];
                        }
                    }
                    if (queenMoveBlack[i][j] > queenMoveWhite[i][j])
                    {
                        if (queenMoveWhite[i][j] == EMPTY)
                        {
                            M1++;
                            N1 += mobility[i][j];
                        }
                        else
                        {
                            M2++;
                            N2 += mobility[i][j];
                        }
                    }
                }
            }
        }
        double s1 = M2 - M1;
        double s2 = N1 - N2;
        s = s1 / 1000 + s2 / 10000;
   }


double CEvaluateEngine::evaluate(int positioin[10][10])
{
    memcpy(Board,positioin, 100*sizeof(int));
    calculate_kingMove();
    calculate_queenMove();
    calculate_w();
    calculate_t1();
    calculate_t2();
    calculate_c1();
    calculate_c2();
    double f1 = (17 / (w + 17));
    double f2 = (w / (w + 49.03));
    double f34 = (1 - (f1 + f2)) / 2;
    calculate_m();
    calculate_s();
    value = f1*t1 + f2*t2 / 2 + f34*((c1 + c2) / 2)+s;
    return value;
}
