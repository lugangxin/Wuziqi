#pragma once
#include "Chess.h"
class Man
{
public:
	void init(Chess* chess);
	void go();
	//void go1();
private:
	Chess *chess;
};

