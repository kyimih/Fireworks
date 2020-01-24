/********************************************************

*	�������ƣ�����ף��

*	���뻷����VS2019 && easyx20190529

*	������أ�yimi

*	����޸ģ�2019.12.10

*   ������־����2019/12/10���Խ�βEnd�����Ų�Ч��

********************************************************/

/**********************   ͷ�ļ�   ************************/
#include<graphics.h>
#include<windows.h>
#include<stdlib.h>
#include<ctime>
#include<cmath>
#include<conio.h>
#include <mmsystem.h>

/*********************   ���ļ�   *************************/
#pragma comment(lib, "winmm.lib")

/*********************   �궨��   *************************/
#define NUM 12
#define PI 3.14159

/*********************   �ṹ��   *************************/

/************   �̻���   ************/
struct Jet {
	int h_max;					//�̻�����ߵ�
	int x, y;					//�̻����ĳ�ʼ����
	int height;					//�̻����ķ���߶�
	DWORD t1, t2, dt;
	bool shoot;					//�Ƿ�Ϊ����״̬
	IMAGE ima[2];				//�����̻�����ɫ����ɫ�ز�
	byte n : 1;					//�̻�����˸ͼƬ�±�
}Jet[NUM];

/*************   �̻�   *************/
struct Fire {
	int x_cen, y_cen;			//��ը���ĵ�����
	int x, y;					//�������Ļ���Ͻǵ�����
	DWORD t1, t2, dt;
	int width;					//�̻��Ŀ�
	int height;					//�̻��ĸ�
	int xy[200][200];			//�������ص�
	int r, r_max;				//��ը��ǰ�뾶�����뾶
	bool show, draw;			//�Ƿ�ը������
}Fire[NUM];

/*********************   ������   *************************/

int main() {
	/***************    ����������    *************/
	void Welcome();
	void Load();
	void Init(int);
	void Chose(DWORD&);
	void Shoot();
	void Style();
	void Show(DWORD*);
	void HappyBirthday();
	void Congratulations();
	void End();
	void Style(DWORD& t1);
	/**********************************************/

	initgraph(1200, 800);
	srand((unsigned)time(NULL) * 1000);

	Welcome();
	DWORD* pMem = GetImageBuffer();
	DWORD t1 = timeGetTime();

	/***************   ��������   *****************/
	mciSendString(TEXT("open C:/���ϻ��ѿ�.mp3 alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);

	for (int i = 0; i < NUM; i++)
		Init(i);
	Load();
	BeginBatchDraw();																//��ʼ��ͼ

	while (!_kbhit()) {
		Sleep(10);
		/**** ���ѡ�� 3200 �����ص���� ****/
		for (int clr = 0; clr < 800; clr++) {
			for (int j = 0; j < 2; j++) {
				int px1 = rand() % 1200;
				int py1 = rand() % 800;
				if (py1 < 799)														// ��ֹԽ��
					pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK;	// ���Դ渳ֵ�������ص�
			}
		}
		Style(t1);
		Chose(t1);
		Shoot();
		Show(pMem);
		FlushBatchDraw();
	}
	EndBatchDraw();
	HappyBirthday();
	Congratulations();
	End();
	while (1);
}

/*********************   �Ӻ���   *************************/

/************   ��ӭ����   **********/
void Welcome() {
	char welcome[] = { '3','2','1' };
	settextstyle(250, 0, _T("����"));
	settextcolor(RGB(50, 720, 720));
	for (int i = 0; i < 3; i++) {
		settextcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		outtextxy(530, 260, welcome[i]);
		Sleep(1000);
	}
	cleardevice();
}

/************   �����ز�   **********/
void Load() {
	/*****************     �����̻�ͼƬ    ****************/
	IMAGE fm[12], sm;
	loadimage(&fm[0], _T("D:/�ز�/C++ �̻�/�̻�/fire1.jpg"), 200, 200);
	loadimage(&fm[1], _T("D:/�ز�/C++ �̻�/�̻�/fire2.jpg"), 200, 200);
	loadimage(&fm[2], _T("D:/�ز�/C++ �̻�/�̻�/fire3.jpg"), 200, 200);
	loadimage(&fm[3], _T("D:/�ز�/C++ �̻�/�̻�/fire4.jpg"), 200, 200);
	loadimage(&fm[4], _T("D:/�ز�/C++ �̻�/�̻�/fire5.jpg"), 200, 200);
	loadimage(&fm[5], _T("D:/�ز�/C++ �̻�/�̻�/fire6.jpg"), 200, 200);
	loadimage(&fm[6], _T("D:/�ز�/C++ �̻�/�̻�/fire7.jpg"), 200, 200);
	loadimage(&fm[7], _T("D:/�ز�/C++ �̻�/�̻�/fire8.jpg"), 200, 200);
	loadimage(&fm[8], _T("D:/�ز�/C++ �̻�/�̻�/fire9.jpg"), 200, 200);
	loadimage(&fm[9], _T("D:/�ز�/C++ �̻�/�̻�/fire10.jpg"), 200, 200);
	loadimage(&fm[10], _T("D:/�ز�/C++ �̻�/�̻�/fire11.jpg"), 200, 200);
	loadimage(&fm[11], _T("D:/�ز�/C++ �̻�/�̻�/fire12.jpg"), 200, 200);

	for (int i = 0; i < 12; i++) {
		SetWorkingImage(&fm[i]);
		getimage(&sm, 0, 0, 200, 200);
		SetWorkingImage(&sm);

		for (int i1 = 0; i1 < 200; i1++)
			for (int i2 = 0; i2 < 200; i2++)
				Fire[i].xy[i1][i2] = getpixel(i1, i2);
	}
	/**************************************************/

	/**************    �����̻���ͼƬ   ***************/
	IMAGE gm;
	loadimage(&gm, _T("D:/�ز�/C++ �̻�/�̻�/shoot.jpg"), 200, 50);
	SetWorkingImage(&gm);
	for (int i = 0; i < NUM; i++) {
		int n = rand() % 5;
		getimage(&Jet[i].ima[0], n * 20, 0, 20, 50);		//��
		getimage(&Jet[i].ima[1], (n + 5) * 20, 0, 20, 50);	//��
	}
	/**************************************************/
	SetWorkingImage(NULL);
}

/*************   ��ʼ��   ***********/
void Init(int i) {
	/********   ��ʼ���̻���   ********/
	Jet[i].x = 0;
	Jet[i].y = 0;
	Jet[i].n = 0;
	Jet[i].dt = rand() % 6 + 4;
	Jet[i].height = 0;
	Jet[i].h_max = 0;
	Jet[i].shoot = false;
	Jet[i].t1 = timeGetTime();

	/**********   ��ʼ���̻�   ********/
	Fire[i].r_max = 150;
	Fire[i].r = 0;
	Fire[i].x = 0;
	Fire[i].y = 0;
	Fire[i].r = 0;
	Fire[i].x_cen = 100;
	Fire[i].y_cen = 100;
	Fire[i].width = 200;
	Fire[i].height = 200;
	Fire[i].dt = 1;
	Fire[i].t1 = timeGetTime();
	Fire[i].show = false;
	//Fire[i].draw = false;
}

/************   ѡ���̻�   **********/
void Chose(DWORD& t1) {

	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100) {
		int n = rand() % 20;
		if (n < 12 && Jet[n].shoot == false && Fire[n].show == false) {
			/*******  �����̻�����׼������  *******/
			Jet[n].x = rand() % 1100 + 50;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].h_max = rand() % 300;

			Jet[n].height = Jet[n].y - Jet[n].h_max;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].ima[Jet[n].n], SRCCOPY);
		}
		t1 = t2;
	}
}

/************   �����̻�   **********/
void Shoot() {
	for (int i = 0; i < NUM; i++) {

		Jet[i].t2 = timeGetTime();

		if (Jet[i].shoot == true && Jet[i].t2 - Jet[i].t1 > Jet[i].dt) {
			/*********   �̻���������   ********/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);
			if (Jet[i].y > Jet[i].h_max) {
				Jet[i].n++;
				Jet[i].y -= 7;
			}
			putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);

			/***********   ��������   **********/
			if (Jet[i].y - Jet[i].h_max < 1 / 4 * Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/********   ���������߶�   *******/
			if (Jet[i].y <= Jet[i].h_max) {
				putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);
				Fire[i].x = Jet[i].x;
				Fire[i].y = Jet[i].h_max;
				Fire[i].show = true;
				Jet[i].shoot = false;
			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}

/************   �̻���ը   **********/
void Show(DWORD* pMem) {

	int drt[15] = { 5, 5, 5, 5, 5, 25, 25, 25, 25, 55, 55, 55, 55, 55 };

	for (int i = 0; i < NUM; i++) {

		Fire[i].t2 = timeGetTime();

		if (Fire[i].show == true && Fire[i].t2 - Fire[i].t1 > Fire[i].dt) {
			if (Fire[i].r < Fire[i].r_max) {
				Fire[i].r++;
				Fire[i].dt = drt[Fire[i].r % 10];
				Fire[i].draw = true;
			}
			if (Fire[i].r >= Fire[i].r_max - 1) {
				Fire[i].draw = false;
				Init(i);
			}
			Fire[i].t1 = Fire[i].t2;
		}

		if (Fire[i].draw == true) {
			for (double m = 0; m <= 2 * PI; m += 0.01) {
				/*****   ������̻�ͼƬ���Ͻǵ�����   *****/
				int x = (int)(Fire[i].x_cen + Fire[i].r * cos(m));
				int y = (int)(Fire[i].y_cen - Fire[i].r * sin(m));

				if (x > 0 && x < Fire[i].width && y>0 && y < Fire[i].height) {
					int r = (Fire[i].xy[x][y] >> 16);
					int g = (Fire[i].xy[x][y] >> 8) & 0xff;
					int b = (Fire[i].xy[x][y]) & 0xff;

					/********   �������Ļ���Ͻǵ�����   *********/
					int xx = (int)(Fire[i].x + Fire[i].r * cos(m));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(m));

					/********   ��������ĵ㡢��ֹԽ��   *********/
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800)
						pMem[yy * 1200 + xx] = BGR(Fire[i].xy[x][y]);
				}
			}
		}
		Fire[i].draw = false;
	}
}

/*************   ����HB   ***********/
void HappyBirthday() {
	char HB[] = "HappyBirthday!";
	settextstyle(100, 0, _T("FreeStyle Script"));
	settextcolor(RGB(50, 720, 720));
	for (int i = 0; i < 15; i++) {
		outtextxy(280 + i * 50, 350, HB[i]);
		Sleep(100);
	}
	char aim[] = "----for KTY";
	settextstyle(40, 0, _T("FreeStyle Script"));
	for (int i = 0; i < 12; i++) {
		outtextxy(600 + i * 30, 500, aim[i]);
		Sleep(100);
	}
	Sleep(3000);
	cleardevice();
}

/************   ��������   **********/
void Congratulations() {
	settextstyle(25, 0, _T("����"));

	outtextxy(400, 250, _T("Ը���ڱ����ʱ��"));
	outtextxy(400, 300, _T("���������󣬵ֿ����⣻"));
	outtextxy(400, 350, _T("����ãʱ������������"));
	outtextxy(400, 400, _T("�����������������У�"));
	outtextxy(400, 450, _T("�������ģ�����������"));
	outtextxy(600, 500, _T("----������������"));
	Sleep(3500);
	cleardevice();

	outtextxy(400, 250, _T("����������"));
	outtextxy(400, 300, _T("ֻ�����ɣ�ֻ��ʢ�ţ�"));
	outtextxy(400, 350, _T("ֻ�����飬"));
	outtextxy(400, 400, _T("ֻ�ʳ��ģ�ֻ�ʸ��£�"));
	outtextxy(400, 450, _T("����������"));
	outtextxy(600, 500, _T("----������������"));
	Sleep(3500);
	cleardevice();

	outtextxy(400, 150, _T("�����ĺ������ģ�"));
	outtextxy(400, 200, _T("�����������Ǿ�ɥ��"));
	outtextxy(400, 250, _T("����������ǿ��"));
	outtextxy(400, 300, _T("ǿ���������ı����ǵ���ͷ��"));
	outtextxy(400, 350, _T("��������л���"));
	outtextxy(400, 400, _T("��ǰ�˽������ǵ�������"));
	outtextxy(400, 450, _T("֪���ഺҲ����ֻ����Щ���ӣ�"));
	outtextxy(400, 500, _T("��֪�����Ƿ񻹻�����"));
	outtextxy(400, 550, _T("��Щ����ϣ��������������顣"));
	outtextxy(600, 600, _T("----������������"));
	Sleep(6000);
	cleardevice();
}

/************   ����End   ***********/
void End() {
	char welcome[] = "End";
	settextstyle(250, 0, _T("Javanese Text"));
	while (true) {
		for (int i = 0; i < 3; i++) {
			settextcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
			outtextxy(480 + i * 80, 300, welcome[i]);
		}
	}
}

// ��ʾ����
void Style(DWORD& st1) {
	DWORD st2 = timeGetTime();
	// һ�׸��ʱ��
	if (st2 - st1 > 10000) {
		// ��������
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
		for (int i = 0; i < NUM; i++) {
			cleardevice();
			/**** ���ɷֲ��̻��� ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].height = Jet[i].y - y[i] * 10;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);			// ��ʾ�̻���

			/**** �����̻����� ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = y[i] * 10;
			Fire[i].show = false;
			Fire[i].r = 0;
		}
		st1 = st2;
	}
}
/**********************************************************/