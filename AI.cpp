#include "AI.h"

void AI::init(Chess* chess)
{
	this->chess = chess;
	int size = chess->getGradeSize();
	for (int i = 0; i < size;i++) {
		vector<int> row;
		for (int j = 0; j < size;j++) {
			row.push_back(0);
		}
		this->scoreMap.push_back(row);
	}
}
void AI::go()
{
	ChessPos pos = think();
	Sleep(1000);
	chess->chessDown(&pos, CHESS_WHITE);
}
/*
*		黑棋         白棋
连2      10			  10
死3		 30			  25
活3		 40			  50
死4		 60			  55
活4      200		  10000
连5		 20000		  30000
*/
void AI::calculateScore()
{
	int personNum = 0;//棋手方有多少个连续的棋子
	int aiNum = 0;//AI方
	int emptyNum = 0;//该方向上空白位的个数
	//评分向量数组清零
	for (int i = 0; i < scoreMap.size();i++) {
		for (int j = 0; j < scoreMap[i].size();j++) {
			scoreMap[i][j] = 0;
		}
	}
	int size = chess->getGradeSize();
	for (int row = 0; row < size;row++) {
		for (int col = 0; col < size;col++) {
			//对每个点进行计算
			if (chess->getChessData(row, col)) continue;
			//8个方向，每个方向再计算反向会有重复计算  指计算四个就够了
			//int y = -1; y <= 1;y++   int x = -1; x <= 1;x++
			/*
				左上，顺时针到右
				（-1，-1） （0，-1） （1，-1） （1，0）
			*/
			for (int y = -1; y <= 0;y++) {
				for (int x = -1; x <= 1;x++) {
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue;//6个方向 再过滤掉两个
					//每个方向都需要重新计算
					personNum = 0;
					aiNum = 0;
					emptyNum = 0;
					//假设黑棋在该位置落子，会构成什么棋型
					for (int i = 1; i <= 4;i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;
						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size&&
							chess->getChessData(curRow,curCol)==1) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					//反向继续计算
					for (int i = 1; i <= 4;i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;
						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					//没有跳3，跳4的情况
					if (personNum == 1) {//连2
						scoreMap[row][col] += 10;
					}
					else if (personNum==2) {//连3
						if (emptyNum == 1) {//死3
							scoreMap[row][col] += 30;
						}
						else if (emptyNum==2) {//活3
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum==3) {
						if (emptyNum==1) {//死4
							scoreMap[row][col] += 60;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 200;
						}
					}
					else if (personNum==4) {
						scoreMap[row][col] += 20000;
					}

					//假设白棋在该位置落子，会构成什么棋型
					emptyNum = 0;

					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;
						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					//反向继续计算
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;
						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					if (aiNum==0) {
						scoreMap[row][col] += 5;
					}else if (aiNum == 1) {//连2
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {//连3
						if (emptyNum == 1) {//死3
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) {//活3
							scoreMap[row][col] += 50;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) {//死4
							scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 10000;
						}
					}
					else if (aiNum == 4) {
						scoreMap[row][col] += 30000;
					}
					
				}
			}
		}
	}
}

ChessPos AI::think()
{
	calculateScore();
	int size = scoreMap.size(); //chess->getGradeSize();
	int maxScore = 0;
	vector<ChessPos> maxPoints;
	for (int row = 0; row < size;row++) {
		for (int col = 0; col < size;col++) {
			if (chess->getChessData(row, col) == 0) {
				if (scoreMap[row][col] > maxScore) {
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row,col));
				}
				else if (scoreMap[row][col]==maxScore) {
					maxPoints.push_back(ChessPos(row,col));
				}
			}
		}
	}
	int index = rand() % maxPoints.size();
	return maxPoints[index];
}
