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
		//��ȡ�������Ϣ
		msg = GetMouseMsg();

		//ͨ��chess���󣬵����ж������Ƿ���Ч  �Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN && this->chess->clickBoard(msg.x, msg.y, &pos))//���������������Ч
		{
			break;
		}
	}
	//printf("%d,%d\n", pos.row, pos.col);����
	//����
	this->chess->chessDown(&pos,CHESS_BLACK);
}
/*void Man::go1()
{
	MOUSEMSG msg;
	ChessPos pos;
	while (1) {
		//��ȡ�������Ϣ
		msg = GetMouseMsg();

		//ͨ��chess���󣬵����ж������Ƿ���Ч  �Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN && this->chess->clickBoard(msg.x, msg.y, &pos))//���������������Ч
		{
			break;
		}
	}
	//printf("%d,%d\n", pos.row, pos.col);����
	//����
	this->chess->chessDown(&pos, CHESS_WHITE);
}*/

