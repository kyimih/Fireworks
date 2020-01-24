/********************************************************

*	程序名称：生日祝福

*	编译环境：VS2019 && easyx20190529

*	作者相关：yimi

*	最后修改：2019.12.10

*   更新日志：（2019/12/10）对结尾End增加炫彩效果

********************************************************/

/**********************   头文件   ************************/
#include<graphics.h>
#include<windows.h>
#include<stdlib.h>
#include<ctime>
#include<cmath>
#include<conio.h>
#include <mmsystem.h>

/*********************   库文件   *************************/
#pragma comment(lib, "winmm.lib")

/*********************   宏定义   *************************/
#define NUM 12
#define PI 3.14159

/*********************   结构体   *************************/

/************   烟花弹   ************/
struct Jet {
	int h_max;					//烟花弹最高点
	int x, y;					//烟花弹的初始坐标
	int height;					//烟花弹的发射高度
	DWORD t1, t2, dt;
	bool shoot;					//是否处为发射状态
	IMAGE ima[2];				//储存烟花弹亮色、暗色素材
	byte n : 1;					//烟花弹闪烁图片下标
}Jet[NUM];

/*************   烟花   *************/
struct Fire {
	int x_cen, y_cen;			//爆炸中心的坐标
	int x, y;					//相对于屏幕左上角的坐标
	DWORD t1, t2, dt;
	int width;					//烟花的宽
	int height;					//烟花的高
	int xy[200][200];			//储存像素点
	int r, r_max;				//爆炸当前半径、最大半径
	bool show, draw;			//是否爆炸、绽放
}Fire[NUM];

/*********************   主函数   *************************/

int main() {
	/***************    函数声明区    *************/
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

	/***************   播放音乐   *****************/
	mciSendString(TEXT("open C:/城南花已开.mp3 alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);

	for (int i = 0; i < NUM; i++)
		Init(i);
	Load();
	BeginBatchDraw();																//开始绘图

	while (!_kbhit()) {
		Sleep(10);
		/**** 随机选择 3200 个像素点擦除 ****/
		for (int clr = 0; clr < 800; clr++) {
			for (int j = 0; j < 2; j++) {
				int px1 = rand() % 1200;
				int py1 = rand() % 800;
				if (py1 < 799)														// 防止越界
					pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK;	// 对显存赋值擦出像素点
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

/*********************   子函数   *************************/

/************   欢迎界面   **********/
void Welcome() {
	char welcome[] = { '3','2','1' };
	settextstyle(250, 0, _T("楷体"));
	settextcolor(RGB(50, 720, 720));
	for (int i = 0; i < 3; i++) {
		settextcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		outtextxy(530, 260, welcome[i]);
		Sleep(1000);
	}
	cleardevice();
}

/************   加载素材   **********/
void Load() {
	/*****************     加载烟花图片    ****************/
	IMAGE fm[12], sm;
	loadimage(&fm[0], _T("D:/素材/C++ 烟花/烟花/fire1.jpg"), 200, 200);
	loadimage(&fm[1], _T("D:/素材/C++ 烟花/烟花/fire2.jpg"), 200, 200);
	loadimage(&fm[2], _T("D:/素材/C++ 烟花/烟花/fire3.jpg"), 200, 200);
	loadimage(&fm[3], _T("D:/素材/C++ 烟花/烟花/fire4.jpg"), 200, 200);
	loadimage(&fm[4], _T("D:/素材/C++ 烟花/烟花/fire5.jpg"), 200, 200);
	loadimage(&fm[5], _T("D:/素材/C++ 烟花/烟花/fire6.jpg"), 200, 200);
	loadimage(&fm[6], _T("D:/素材/C++ 烟花/烟花/fire7.jpg"), 200, 200);
	loadimage(&fm[7], _T("D:/素材/C++ 烟花/烟花/fire8.jpg"), 200, 200);
	loadimage(&fm[8], _T("D:/素材/C++ 烟花/烟花/fire9.jpg"), 200, 200);
	loadimage(&fm[9], _T("D:/素材/C++ 烟花/烟花/fire10.jpg"), 200, 200);
	loadimage(&fm[10], _T("D:/素材/C++ 烟花/烟花/fire11.jpg"), 200, 200);
	loadimage(&fm[11], _T("D:/素材/C++ 烟花/烟花/fire12.jpg"), 200, 200);

	for (int i = 0; i < 12; i++) {
		SetWorkingImage(&fm[i]);
		getimage(&sm, 0, 0, 200, 200);
		SetWorkingImage(&sm);

		for (int i1 = 0; i1 < 200; i1++)
			for (int i2 = 0; i2 < 200; i2++)
				Fire[i].xy[i1][i2] = getpixel(i1, i2);
	}
	/**************************************************/

	/**************    加载烟花弹图片   ***************/
	IMAGE gm;
	loadimage(&gm, _T("D:/素材/C++ 烟花/烟花/shoot.jpg"), 200, 50);
	SetWorkingImage(&gm);
	for (int i = 0; i < NUM; i++) {
		int n = rand() % 5;
		getimage(&Jet[i].ima[0], n * 20, 0, 20, 50);		//暗
		getimage(&Jet[i].ima[1], (n + 5) * 20, 0, 20, 50);	//亮
	}
	/**************************************************/
	SetWorkingImage(NULL);
}

/*************   初始化   ***********/
void Init(int i) {
	/********   初始化烟花弹   ********/
	Jet[i].x = 0;
	Jet[i].y = 0;
	Jet[i].n = 0;
	Jet[i].dt = rand() % 6 + 4;
	Jet[i].height = 0;
	Jet[i].h_max = 0;
	Jet[i].shoot = false;
	Jet[i].t1 = timeGetTime();

	/**********   初始化烟花   ********/
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

/************   选择烟花   **********/
void Chose(DWORD& t1) {

	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100) {
		int n = rand() % 20;
		if (n < 12 && Jet[n].shoot == false && Fire[n].show == false) {
			/*******  重置烟花弹，准备发射  *******/
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

/************   发射烟花   **********/
void Shoot() {
	for (int i = 0; i < NUM; i++) {

		Jet[i].t2 = timeGetTime();

		if (Jet[i].shoot == true && Jet[i].t2 - Jet[i].t1 > Jet[i].dt) {
			/*********   烟花弹的上升   ********/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);
			if (Jet[i].y > Jet[i].h_max) {
				Jet[i].n++;
				Jet[i].y -= 7;
			}
			putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);

			/***********   减速上升   **********/
			if (Jet[i].y - Jet[i].h_max < 1 / 4 * Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/********   上升到最大高度   *******/
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

/************   烟花爆炸   **********/
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
				/*****   相对于烟花图片左上角的坐标   *****/
				int x = (int)(Fire[i].x_cen + Fire[i].r * cos(m));
				int y = (int)(Fire[i].y_cen - Fire[i].r * sin(m));

				if (x > 0 && x < Fire[i].width && y>0 && y < Fire[i].height) {
					int r = (Fire[i].xy[x][y] >> 16);
					int g = (Fire[i].xy[x][y] >> 8) & 0xff;
					int b = (Fire[i].xy[x][y]) & 0xff;

					/********   相对于屏幕左上角的坐标   *********/
					int xx = (int)(Fire[i].x + Fire[i].r * cos(m));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(m));

					/********   输出较亮的点、防止越界   *********/
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800)
						pMem[yy * 1200 + xx] = BGR(Fire[i].xy[x][y]);
				}
			}
		}
		Fire[i].draw = false;
	}
}

/*************   加载HB   ***********/
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

/************   加载箴言   **********/
void Congratulations() {
	settextstyle(25, 0, _T("楷体"));

	outtextxy(400, 250, _T("愿你在被打击时，"));
	outtextxy(400, 300, _T("记起你的珍贵，抵抗恶意；"));
	outtextxy(400, 350, _T("在迷茫时，坚信你的珍贵。"));
	outtextxy(400, 400, _T("爱你所爱，行你所行，"));
	outtextxy(400, 450, _T("听从你心，无问西东。"));
	outtextxy(600, 500, _T("----《无问西东》"));
	Sleep(3500);
	cleardevice();

	outtextxy(400, 250, _T("无问西东，"));
	outtextxy(400, 300, _T("只问自由，只问盛放，"));
	outtextxy(400, 350, _T("只问深情，"));
	outtextxy(400, 400, _T("只问初心，只问敢勇，"));
	outtextxy(400, 450, _T("无问西东。"));
	outtextxy(600, 500, _T("----《无问西东》"));
	Sleep(3500);
	cleardevice();

	outtextxy(400, 150, _T("看见的和听到的，"));
	outtextxy(400, 200, _T("经常会令你们沮丧。"));
	outtextxy(400, 250, _T("世俗是这样强大，"));
	outtextxy(400, 300, _T("强大到生不出改变他们的念头。"));
	outtextxy(400, 350, _T("可是如果有机会"));
	outtextxy(400, 400, _T("提前了解了你们的人生，"));
	outtextxy(400, 450, _T("知道青春也不过只有这些日子，"));
	outtextxy(400, 500, _T("不知你们是否还会在意"));
	outtextxy(400, 550, _T("那些世俗希望你们在意的事情。"));
	outtextxy(600, 600, _T("----《无问西东》"));
	Sleep(6000);
	cleardevice();
}

/************   加载End   ***********/
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

// 显示花样
void Style(DWORD& st1) {
	DWORD st2 = timeGetTime();
	// 一首歌的时间
	if (st2 - st1 > 10000) {
		// 心形坐标
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
		for (int i = 0; i < NUM; i++) {
			cleardevice();
			/**** 规律分布烟花弹 ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].height = Jet[i].y - y[i] * 10;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].ima[Jet[i].n], SRCINVERT);			// 显示烟花弹

			/**** 设置烟花参数 ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = y[i] * 10;
			Fire[i].show = false;
			Fire[i].r = 0;
		}
		st1 = st2;
	}
}
/**********************************************************/