#include "ChessGame.h"

ChessGame::ChessGame(Man* man,AI *ai, Chess* chess)
{
	this->man = man;
	this->ai = ai;
	this->chess = chess;
	//this->man1 = man1;
	man->init(chess);
	ai->init(chess);
	//man1->init(chess);
}

void ChessGame::play()
{
	chess->init();
	while (1) {
		//先由棋手走
		man->go();
		if (chess->checkOver()) {
			chess->init();
			continue;
		}
		//AI走
		//man1->go1();
		ai->go();
		if (chess->checkOver()) {
			chess->init();
			continue;
		}
	}
}
