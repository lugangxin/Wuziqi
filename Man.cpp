#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;	
	while (1) {
		//获取鼠标点击消息
		msg = GetMouseMsg();

		//通过chess对象，调用判断落子是否有效  以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN && this->chess->clickBoard(msg.x, msg.y, &pos))//左键单击，并且有效
		{
			break;
		}
	}
	//printf("%d,%d\n", pos.row, pos.col);测试
	//落子
	this->chess->chessDown(&pos,CHESS_BLACK);
}
/*void Man::go1()
{
	MOUSEMSG msg;
	ChessPos pos;
	while (1) {
		//获取鼠标点击消息
		msg = GetMouseMsg();

		//通过chess对象，调用判断落子是否有效  以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN && this->chess->clickBoard(msg.x, msg.y, &pos))//左键单击，并且有效
		{
			break;
		}
	}
	//printf("%d,%d\n", pos.row, pos.col);测试
	//落子
	this->chess->chessDown(&pos, CHESS_WHITE);
}*/

