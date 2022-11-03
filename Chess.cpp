#include "Chess.h"
//提供音乐播放功能
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<math.h>
#include<conio.h>//_getch();//暂停
//easyx图形库默认不支持背景透明的png格式图片，把透明部分直接渲染为黑色了。解决方案，使用自定义的图形渲染接口
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}
Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
	this->gradeSize = gradeSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;
	for (int i = 0; i < gradeSize;i++) {
		vector<int> row;
		for (int j = 0; j < gradeSize;j++) {
			row.push_back(0);
		}
		this->chessMap.push_back(row);
	}
}

void Chess::init()
{
	//创建游戏窗口
	initgraph(897,895);//,EW_SHOWCONSOLE显示控制台
	//显示棋盘图片
	loadimage(0,"res/棋盘2.jpg");//项目-》属性-》高级-》多字节字符集  或  L"res/棋盘2.jpg"
	//播放开始提示音
	mciSendString("play res/start.wav",0,0,0);

	//加载黑棋和白棋棋子的图片
	loadimage(&chessBlackImg,"res/black.png",chessSize,chessSize,true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

	//棋盘清零
	for (int i = 0; i < gradeSize;i++) {
		for (int j = 0; j < gradeSize;j++) {
			this->chessMap[i][j] = 0;
		}
	}
	playerFlag = true;//黑子先走
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col=(x - this->margin_x) / this->chessSize;
	int row = (y - this->margin_y) / this->chessSize;
	int leftTopX = margin_x + chessSize * col;
	int leftTopY = margin_y + chessSize * row;
	int offset = chessSize * 0.4;

	int len;
	bool ret = false;
	do {
		//左上角
		len = sqrt((x - leftTopX) * (x - leftTopX) + (y - leftTopY) * (y - leftTopY));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
		//右上角
		int rightTopX = leftTopX + chessSize;
		int rightTopY = leftTopY;
		len = sqrt((x-rightTopX)* (x - rightTopX)+(y-rightTopY)* (y - rightTopY));
		if (len < offset) {
			pos->row = row;
			pos->col = col+1;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
		//左下角
		int leftDownX = leftTopX;
		int leftDownY = leftTopY + chessSize;
		len = sqrt((x-leftDownX)* (x - leftDownX)+(y-leftDownY)* (y - leftDownY));
		if (len < offset) {
			pos->row = row+1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
		//右下角
		int rightDownX = leftTopX + chessSize;
		int rightDownY = leftTopY + chessSize;
		len = sqrt((x - rightDownX) * (x - rightDownX) + (y - rightDownY) * (y - rightDownY));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
	} while (0);
	return ret;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	mciSendString("play res/down7.wav", 0, 0, 0);
	//正常图片左边位置在左上角，应该放在正中间
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize;;
	if (kind == CHESS_WHITE) {
		//putimage(x,y,&chessWhiteImg);
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		//putimage(x,y,&chessBlackImg);
		putimagePNG(x, y, &chessBlackImg);
	}
	updateGameMap(pos);
}

int Chess::getGradeSize()
{
	return this->gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

bool Chess::checkOver()
{
	if (checkWin()) {
		Sleep(1500);
		if (playerFlag==false) {
			//刚才下棋的是人，棋手胜利
			mciSendString("play res/不错.mp3", 0, 0, 0);
			loadimage(0,"res/胜利.jpg");
		}
		else {
			mciSendString("play res/失败.mp3", 0, 0, 0);
			loadimage(0, "res/失败.jpg");
		}
		_getch();//暂停
		return true;
	}	
	return false;
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//黑白交替出手
}

bool Chess::checkWin()
{
	//最近落子点位置
	int row = lastPos.row;
	int col = lastPos.col;
	//水平方向
	for (int i = 0; i < 5;i++) {
		//最左侧位置 row
		//i=0 col-0 col+1 col+2 ....
		//i=1 col-1 col col+1....
		if (col-i>=0&&col-i+4<gradeSize&&chessMap[row][col-i]==chessMap[row][col-i+1]&&
			chessMap[row][col - i] == chessMap[row][col - i + 2]&&
			chessMap[row][col - i] == chessMap[row][col - i + 3]&&
			chessMap[row][col - i] == chessMap[row][col - i + 4]) {
			return true;
		}
	}
	//竖直方向
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize && chessMap[row-i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col]) {
			return true;
		}
	}
	// "\"方向
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize && col - i >= 0 && col - i + 4 < gradeSize 
			&& chessMap[row - i][col-i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col-i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col-i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col-i] == chessMap[row - i + 4][col - i + 4]) {
			return true;
		}
	}

	// "/"方向
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize && col + i < gradeSize && col + i - 4 >=0 && 
			chessMap[row - i][col + i] == chessMap[row - i + 1][col+i-1] &&
			chessMap[row - i][col + i] == chessMap[row - i + 2][col + i - 2] &&
			chessMap[row - i][col + i] == chessMap[row - i + 3][col + i - 3] &&
			chessMap[row - i][col + i] == chessMap[row - i + 4][col + i - 4]) {
			return true;
		}
	}
	/*
	for (int i = 0; i < 5; i++) {
		if (row + i < gradeSize && row + i - 4 >=0 && col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4]) {
			return true;
		}
	}*/
	return false;
}
