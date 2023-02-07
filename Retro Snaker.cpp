#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

void hideCursor(bool isShow)
{
	HANDLE _hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(_hOut, &cursor_info);
	cursor_info.bVisible = isShow ? TRUE : FALSE;
	SetConsoleCursorInfo(_hOut, &cursor_info);}

void setcolor(WORD colorNum)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, colorNum);
}

void setx_y(short x, short y)
{
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

#define wi 60
#define hei 25
#define x_p 10
#define y_p 0

void show()
{
	for (int i = x_p; i <= wi + x_p; i++)
	{
		for (int j = y_p; j <= hei + y_p; j++)
		{
			setx_y(i, j);
			if (i == x_p || i == wi + x_p)
				cout << '|';
			else if (j == y_p || j == hei + y_p)
				cout << '=';
			else
				cout << ' ';
		}
	}
}

void showMenu()
{
	show();
	setx_y(wi / 2 + x_p - 12, hei / 2 + y_p);
	cout << "开始:回车";
	setx_y(wi / 2 + x_p - 12, hei / 2 + y_p + 1);
	cout << "控制:w s a d 方向键";
	setx_y(wi / 2 + x_p - 12, hei / 2 + y_p + 2);
	cout << "暂停:空格";
	setx_y(wi / 2 + x_p - 12, hei / 2 + y_p + 3);
	cout << "退出:ESC";
	setx_y(0, hei + 1);
}


typedef struct _point
{
	short x;
	short y;
}_point;

typedef struct _position
{
	_point xy;
	struct _position* next;
	_position() :next(nullptr), xy({ 0,0 }) {}
	_position(_point temp) :next(nullptr), xy({ temp.x,temp.y }) {}
}_pos;

typedef struct _snackhead
{
	_pos* SH;
	_pos* ST;
	_snackhead() :SH(nullptr), ST(nullptr){}
}_Head;

#define firstHx wi / 2 + x_p
#define firstHy hei / 2 + y_p

void initQue(_Head& H)
{
	((H.ST = new _pos())->xy) = { firstHx - 2,firstHy };
	((H.SH = new _pos())->xy) = { firstHx,firstHy };
	((H.ST->next = new _pos())->xy) = { firstHx - 1,firstHy };
	H.ST->next->next = H.SH;
}

void destroy(_Head& H)
{
	while (H.ST != nullptr)
	{
		H.SH = H.ST;
		H.ST = H.ST->next;
		delete H.SH;
		H.SH = nullptr;
	}
}

bool sPd(_Head& H, short LR, short UD)
{
	H.SH->next = H.ST;
	_pos*& p = H.SH->next;
	while (p != H.SH)
	{
		if ((p->xy).x == (H.SH->xy).x + LR && (p->xy).y == (H.SH->xy).y + UD)
		{
			p = nullptr;
			return false;
		}
		p = p->next;
	}
	p = nullptr;
	return true;
}

int snack()
{
	show();
	_point fruit = { firstHx + 5,firstHy + 5 };
	_point temp;
	_Head H;
	short LR = 1;
	short UD = 0;
	char com;
	bool ispause = false;
	bool noquit = true;
	int score = 0;
	char sHead = '@';
	char sBody = 'O';
	char sFruit = '*';
	initQue(H);
	setx_y(firstHx, firstHy);
	cout << sHead;
	setx_y(firstHx - 1, firstHy);
	cout << sBody;
	setx_y(firstHx - 2, firstHy);
	cout << sBody;
	setx_y(fruit.x, fruit.y);
	cout << sFruit;
	while (noquit)
	{
		setx_y(wi + x_p + 1, hei / 2 + y_p);
		cout << "分数: " << score;
		setx_y(wi + x_p + 1, hei / 2 + y_p+1);
		cout << "长度: " << score + 3;
		if (_kbhit())
		{
			com = _getch();
			switch (com)
			{
				//UP
			case 119:
			case 72:
				if (UD == 0 && !ispause)
				{
					UD = -1;
					LR = 0;
				}
				break;
				//DOWN
			case 115:
			case 80:
				if (UD == 0 && !ispause)
				{
					UD = 1;
					LR = 0;
				}
				break;
				//LEFT
			case 97:
			case 75:
				if (LR == 0 && !ispause)
				{
					LR = -1;
					UD = 0;
				}
				break;
				//RIGHT
			case 100:
			case 77:
				if (LR == 0 && !ispause)
				{
					LR = 1;
					UD = 0;
				}
				break;
			case 32:
				ispause = !ispause;
				break;
			case 27:
				noquit = false;
				break;
			}
		}
		if (!ispause)
		{
			if (((H.SH->xy).x + LR) <= x_p || ((H.SH->xy).x + LR) >= wi + x_p || ((H.SH->xy).y + UD) <= y_p || ((H.SH->xy).y + UD) >= hei + y_p)
			{
				noquit = false;
			}
			else if (!sPd(H, LR, UD))
			{
				noquit = false;
			}
			else if (((H.SH->xy).x + LR) == fruit.x && ((H.SH->xy).y + UD) == fruit.y)
			{
				H.SH->next = new _pos(H.ST->xy);
				H.SH->next->xy = { 0,hei + y_p + 2 };
				H.SH->next->next = H.ST;
				H.ST = H.SH->next;
				H.SH->next = nullptr;
				srand((unsigned)time(NULL));
				fruit.x = rand() % (wi - 1) + x_p + 1;
				fruit.y = rand() % (hei - 1) + y_p + 1;
				setx_y(fruit.x, fruit.y);
				cout << sFruit;
				++score;
			}
			if(noquit)
			{
				setx_y((H.SH->xy).x, (H.SH->xy).y);
				cout << sBody;
				temp.x = (H.SH->xy).x + LR;
				temp.y = (H.SH->xy).y + UD;
				H.SH = (H.SH->next = new _pos(temp));
				setx_y((H.SH->xy).x, (H.SH->xy).y);
				cout << sHead;
				setx_y((H.ST->xy).x, (H.ST->xy).y);
				cout << ' ';
				H.ST = (H.SH->next = H.ST)->next;
				delete H.SH->next;
				H.SH->next = nullptr;
			}
		}
		Sleep(50);
	}
	destroy(H);
	return score;
}

bool control()
{
	char flag;
	while (1)
	{
		if (_kbhit())
		{
			flag = _getch();
			switch (flag)
			{
			case 13:
				snack();
				return true;
				break;
			case 27:
				return false;
			}
		}
	}
}

int main()
{
	bool flag = true;
	//system("pause");
	hideCursor(false);
	while (flag)
	{
		showMenu();
		flag = control();
	}
	return 0;
}

