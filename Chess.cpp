#include "Chess.h"
//�ṩ���ֲ��Ź���
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<math.h>
#include<conio.h>//_getch();//��ͣ
//easyxͼ�ο�Ĭ�ϲ�֧�ֱ���͸����png��ʽͼƬ����͸������ֱ����ȾΪ��ɫ�ˡ����������ʹ���Զ����ͼ����Ⱦ�ӿ�
void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
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
	//������Ϸ����
	initgraph(897,895);//,EW_SHOWCONSOLE��ʾ����̨
	//��ʾ����ͼƬ
	loadimage(0,"res/����2.jpg");//��Ŀ-������-���߼�-�����ֽ��ַ���  ��  L"res/����2.jpg"
	//���ſ�ʼ��ʾ��
	mciSendString("play res/start.wav",0,0,0);

	//���غ���Ͱ������ӵ�ͼƬ
	loadimage(&chessBlackImg,"res/black.png",chessSize,chessSize,true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

	//��������
	for (int i = 0; i < gradeSize;i++) {
		for (int j = 0; j < gradeSize;j++) {
			this->chessMap[i][j] = 0;
		}
	}
	playerFlag = true;//��������
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
		//���Ͻ�
		len = sqrt((x - leftTopX) * (x - leftTopX) + (y - leftTopY) * (y - leftTopY));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
		//���Ͻ�
		int rightTopX = leftTopX + chessSize;
		int rightTopY = leftTopY;
		len = sqrt((x-rightTopX)* (x - rightTopX)+(y-rightTopY)* (y - rightTopY));
		if (len < offset) {
			pos->row = row;
			pos->col = col+1;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
		//���½�
		int leftDownX = leftTopX;
		int leftDownY = leftTopY + chessSize;
		len = sqrt((x-leftDownX)* (x - leftDownX)+(y-leftDownY)* (y - leftDownY));
		if (len < offset) {
			pos->row = row+1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) ret = true;
			break;
		}
		//���½�
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
	//����ͼƬ���λ�������Ͻǣ�Ӧ�÷������м�
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
			//�ղ���������ˣ�����ʤ��
			mciSendString("play res/����.mp3", 0, 0, 0);
			loadimage(0,"res/ʤ��.jpg");
		}
		else {
			mciSendString("play res/ʧ��.mp3", 0, 0, 0);
			loadimage(0, "res/ʧ��.jpg");
		}
		_getch();//��ͣ
		return true;
	}	
	return false;
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//�ڰ׽������
}

bool Chess::checkWin()
{
	//������ӵ�λ��
	int row = lastPos.row;
	int col = lastPos.col;
	//ˮƽ����
	for (int i = 0; i < 5;i++) {
		//�����λ�� row
		//i=0 col-0 col+1 col+2 ....
		//i=1 col-1 col col+1....
		if (col-i>=0&&col-i+4<gradeSize&&chessMap[row][col-i]==chessMap[row][col-i+1]&&
			chessMap[row][col - i] == chessMap[row][col - i + 2]&&
			chessMap[row][col - i] == chessMap[row][col - i + 3]&&
			chessMap[row][col - i] == chessMap[row][col - i + 4]) {
			return true;
		}
	}
	//��ֱ����
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize && chessMap[row-i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col]) {
			return true;
		}
	}
	// "\"����
	for (int i = 0; i < 5; i++) {
		if (row - i >= 0 && row - i + 4 < gradeSize && col - i >= 0 && col - i + 4 < gradeSize 
			&& chessMap[row - i][col-i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col-i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col-i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col-i] == chessMap[row - i + 4][col - i + 4]) {
			return true;
		}
	}

	// "/"����
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
