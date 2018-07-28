#ifndef EVALUATEENGINE_H
#define EVALUATEENGINE_H

class CEvaluateEngine
{
public:
	CEvaluateEngine();
	virtual ~CEvaluateEngine();
    double evaluate(int Board[10][10]);
	double getMobility()const{ return m; }
private:   //以下所有参数的意义，请全部参照英文原版论文
    int Board[10][10];    //存储传入的棋盘
    int kingMoveBlack[10][10];
    int kingMoveWhite[10][10];
    int queenMoveBlack[10][10];
    int queenMoveWhite[10][10];
    int mobility[10][10];

	double t1;
	double t2;
	double c1;
	double c2;
	double w;
	double s1;
	double s2;
    double s;
	double t;
	double m; 
	double K;           //浮动参数，用于
    double value;       //最终评估值

	void calculate_kingMove();
	void calculate_queenMove();
	void calculate_t1();
	void calculate_t2();
	void calculate_c1();
	void calculate_c2();
	void calculate_w();
	void calculate_m();
    void calculate_s();

//	int offset[8][2];  //偏移方向
};

#endif

