#pragma once
#include<graphics.h>//easyx图形库头文件
#include<vector>
using namespace std;
//落子位置
struct ChessPos {
	int row;
	int col;
	ChessPos(int r=0,int c=0):row(r),col(c) {}//有默认参数，代表默认构造函数
};
typedef enum {
	CHESS_WHITE=-1,//白棋
	CHESS_BLACK=1
}chess_kind_t;
class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	//棋盘初始化，加载棋盘图片资源，初始化相关数据
	void init();
	//判断（x,y）是否是有效点击 ，是则保存在参数pos中
	bool clickBoard(int x, int y, ChessPos* pos);
	//在棋盘指定位置落子
	void chessDown(ChessPos *pos, chess_kind_t kind);
	//获取棋盘大小（13，15，19）
	int getGradeSize();
	//获取指定位置是黑棋还是白棋，还是空白
	int getChessData(ChessPos *pos);
	int getChessData(int row,int col);
	//检查棋局是否结束
	bool checkOver();

	//使用图形库进行显示 easyx
private:
	IMAGE chessBlackImg;//黑棋
	IMAGE chessWhiteImg;//白棋
	int gradeSize;//棋盘大小
	int margin_x;//棋盘左侧边界
	int margin_y;//棋盘顶部边界
	float chessSize;//棋盘小方格大小（与棋子大小相同）
	//存储当前棋局的棋子分布  0：空白  1：黑子  -1：白子
	vector<vector<int>> chessMap; 
	//表示现在该谁下棋
	bool playerFlag;//true: 黑  false:白

	//防止外部乱调用
	void updateGameMap(ChessPos* pos);

	bool checkWin();//检查是否胜利，胜负已分就返回true

	ChessPos lastPos;//最近落子位置
};

