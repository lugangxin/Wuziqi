#pragma once
#include "Man.h"
#include "AI.h"
class ChessGame
{
public:
	ChessGame(Man *man,AI *ai,Chess *chess);
	//��ʼ�Ծ�
	void play();
	//������ݳ�Ա
private:
	Man* man;
	AI* ai;
	Chess* chess;
	//Man* man1;
};

