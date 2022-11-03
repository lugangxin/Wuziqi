#include<iostream>
using namespace std;
#include "ChessGame.h"
int main() {
	Man man;
	AI ai;
	//Man man1;
	Chess chess(13,44,43,67.3);
	ChessGame game(&man,&ai,&chess);
	game.play();
	return 0;
}