#pragma once
#include<graphics.h>//easyxͼ�ο�ͷ�ļ�
#include<vector>
using namespace std;
//����λ��
struct ChessPos {
	int row;
	int col;
	ChessPos(int r=0,int c=0):row(r),col(c) {}//��Ĭ�ϲ���������Ĭ�Ϲ��캯��
};
typedef enum {
	CHESS_WHITE=-1,//����
	CHESS_BLACK=1
}chess_kind_t;
class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	//���̳�ʼ������������ͼƬ��Դ����ʼ���������
	void init();
	//�жϣ�x,y���Ƿ�����Ч��� �����򱣴��ڲ���pos��
	bool clickBoard(int x, int y, ChessPos* pos);
	//������ָ��λ������
	void chessDown(ChessPos *pos, chess_kind_t kind);
	//��ȡ���̴�С��13��15��19��
	int getGradeSize();
	//��ȡָ��λ���Ǻ��廹�ǰ��壬���ǿհ�
	int getChessData(ChessPos *pos);
	int getChessData(int row,int col);
	//�������Ƿ����
	bool checkOver();

	//ʹ��ͼ�ο������ʾ easyx
private:
	IMAGE chessBlackImg;//����
	IMAGE chessWhiteImg;//����
	int gradeSize;//���̴�С
	int margin_x;//�������߽�
	int margin_y;//���̶����߽�
	float chessSize;//����С�����С�������Ӵ�С��ͬ��
	//�洢��ǰ��ֵ����ӷֲ�  0���հ�  1������  -1������
	vector<vector<int>> chessMap; 
	//��ʾ���ڸ�˭����
	bool playerFlag;//true: ��  false:��

	//��ֹ�ⲿ�ҵ���
	void updateGameMap(ChessPos* pos);

	bool checkWin();//����Ƿ�ʤ����ʤ���ѷ־ͷ���true

	ChessPos lastPos;//�������λ��
};

