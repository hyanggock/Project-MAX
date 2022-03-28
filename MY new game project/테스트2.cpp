#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include<MMSystem.h>
#include<string.h>
#pragma comment(lib,"winmm")
#define START_X 2
#define START_Y 3
#define FINAL_X 48
#define FINAL_Y 37
#define BLK_XCNT 6
#define BLK_YCNT 5
#define ITEM1SIZE1 -6
#define ITEM1SIZE2 -8
#define BAR_SIZE1 8

MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;
int dwID;
int dwID1;
int dwID2;
int dwID3;
int dwID4;

POINT mouse;
// 마우스 이동 값
int gamespeed = 5;

int ball_x;
int ball_y;
// 볼변화량
int heartp = 0, heart = 3;
int time_s = 0;
int time_f = 0;

int gtm = 0;
int gt = 0;
int gta = 0;
int gtb = 0;
int gtc = 0;
//시간 저장 값 

int change_x;
int change_y;
// 블록X, Y 좌표

int stage = 1;
//스테이지

int blocks_x[BLK_XCNT] = { 2, 10, 18, 26, 34, 42 };
int blocks_y[BLK_YCNT] = { 4, 6, 8, 10, 12 };
int blocks_status[BLK_YCNT][BLK_XCNT] =
{
	{ 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 4, 0, 1 },
	{ 1, 0, 1, 0, 1, 0 },
	{ 0, 2, 0, 1, 0, 1 },
	{ 5, 3, 5, 5, 2, 5 },
};


int a;
int itemcount = 0;
int   item1_x, item1_y;//아이템 1  바길게
int item2_x, item2_y;//아이템 2  목숨
int item3_x, item3_y;//아이템 3  바줄이기 
int item4_x, item4_y;//아이템 4 죽음 
int item5_x, item5_y;//아이템 5 통과 
int item1 = 0;// 바 길게 
int item2 = 0;// 생명
int item3 = 0;// 바 줄이기
int item4 = 0;// 죽음 
int item5 = 0;// 통과 
int bar_x;
int bar_y;
int temsize = 0;
int it = 42;// 바 증가시 변화를 줌 



void PlayMusic(char *fileName)
{
	mciOpen.lpstrDeviceType = L"mpenvideo";
	mciOpen.lpstrElementName = L"터짐.mp3";//연주할 파일 이름
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); //MCI_OPEN 명령을 준다.
	dwID = mciOpen.wDeviceID;    //열린 디바이스 아이디를 받는다.
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlay);//연주시작
}
void PlayMusic1(char *fileName)
{
	mciOpen.lpstrElementName = L"배경음악.mp3";//연주할 파일 이름
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); //MCI_OPEN 명령을 준다.
	dwID1 = mciOpen.wDeviceID;    //열린 디바이스 아이디를 받는다.
	mciSendCommand(dwID1, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlay);//연주시작
}
void PlayMusic2(char *fileName)
{
	mciOpen.lpstrElementName = L"끝.mp3";//연주할 파일 이름
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); //MCI_OPEN 명령을 준다.
	dwID2 = mciOpen.wDeviceID;    //열린 디바이스 아이디를 받는다.
	mciSendCommand(dwID2, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlay);//연주시작
}
void PlayMusic3(char *fileName)
{
	mciOpen.lpstrElementName = L"클리어.mp3";//연주할 파일 이름
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); //MCI_OPEN 명령을 준다.
	dwID3 = mciOpen.wDeviceID;    //열린 디바이스 아이디를 받는다.
	mciSendCommand(dwID3, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlay);//연주시작
}
void PlayMusic4(char *fileName)
{
	mciOpen.lpstrElementName = L"진짜끝.mp3";//연주할 파일 이름
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); //MCI_OPEN 명령을 준다.
	dwID4 = mciOpen.wDeviceID;    //열린 디바이스 아이디를 받느다.
	mciSendCommand(dwID4, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlay);//연주시작
}

void StopMusic()
{
	mciSendCommand(dwID, MCI_CLOSE, 0, NULL);//연주 종
}
void StopMusic1()
{
	mciSendCommand(dwID1, MCI_CLOSE, 0, NULL);//연주 종료
}
struct member//랭킹값
{
	char name[10];
	int seconds;
	int minute;

};

//블록값 리셋
void back1()
{
	int i, j;
	for (i = 0; i<BLK_YCNT; i++)
	{
		for (j = 0; j<BLK_XCNT; j++)
		{
			blocks_status[i][j] = 1;
			blocks_status[2][0] = 5;
			blocks_status[2][1] = 5;
			blocks_status[2][2] = 5;
			blocks_status[2][3] = 5;
			blocks_status[2][4] = 5;
			blocks_status[2][5] = 5;
			blocks_status[3][3] = 7;
			blocks_status[0][0] = 6;
			blocks_status[3][5] = 2;
		}

	}
}
void back2()
{
	int i, j;
	for (i = 0; i<BLK_YCNT; i++)
	{
		for (j = 0; j<BLK_XCNT; j++)
		{
			blocks_status[i][j] = 1;
			blocks_status[4][0] = 5;
			blocks_status[4][1] = 5;
			blocks_status[4][2] = 5;
			blocks_status[4][3] = 5;
			blocks_status[4][4] = 5;
			blocks_status[4][5] = 5;
			blocks_status[0][2] = 0;
			blocks_status[0][3] = 0;
			blocks_status[1][0] = 0;
			blocks_status[1][1] = 0;
			blocks_status[2][0] = 0;
			blocks_status[2][1] = 0;
			blocks_status[3][2] = 0;
			blocks_status[3][3] = 0;
			blocks_status[1][4] = 0;
			blocks_status[1][5] = 0;
			blocks_status[2][4] = 0;
			blocks_status[2][5] = 0;
			blocks_status[3][1] = 6;
			blocks_status[2][3] = 4;
		}
	}
}




// 위치이동함수
void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
//색을 지정하는 함수
void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


// 시작전 로딩화면
void start()
{
	int num = 0;

	for (;;)
	{
		SetColor(15);
		gotoxy(0, 0);
		printf("-------------------------------------------------------------------------------");
		gotoxy(0, 1);
		SetColor(9);
		printf("--  ■  --  ■  ---------------------------------------------------------------");
		gotoxy(0, 2);
		SetColor(10);
		printf("--  ■■■■■  ---------------------------------------------------------------");
		gotoxy(0, 3);
		SetColor(11);
		printf("--  ■      ■  ---  ■■■■  ------------------------------------------------");
		gotoxy(0, 4);
		SetColor(12);
		printf("--  ■■■■■  ---------  ■  -  ■  -----------------------------------------");
		gotoxy(0, 5);
		SetColor(13);
		printf("-------------------  ■■■■  -  ■  -----------------------------------------");
		gotoxy(0, 6);
		SetColor(14);
		printf(" ■■■■■■■■  - ■  ---  ■■■  -----------------------------------------");
		gotoxy(0, 7);
		SetColor(15);
		printf("-------------------  ■■■■  -  ■  -----------------------------------------");
		gotoxy(0, 8);
		SetColor(9);
		printf("--  ■■■■■  ----------------  ▼  -----------------------------------------");
		gotoxy(0, 9);
		SetColor(10);
		printf("----------  ■  ------------------  -------------------------------------------");
		gotoxy(0, 10);
		SetColor(11);
		printf("--  ■■■■■  --------  ■■■■■■  ---------------------------------------");
		gotoxy(0, 11);
		SetColor(12);
		printf("--  ■  --------------------------  ■  ---------------------------------------");
		gotoxy(0, 12);
		SetColor(13);
		printf("--  ■■■■■■■■  ------------  ■  ---------------------------------------");
		gotoxy(0, 13);
		SetColor(14);
		printf("----------------------------------  ■  ---------------------------------------");
		gotoxy(0, 14);
		SetColor(15);
		printf("----------------------------------  ■  ---------------------------------------");
		gotoxy(0, 15);
		SetColor(9);
		printf("----------  ■    ■    ■  ------  ▼  ---------------------------------------");
		gotoxy(0, 16);
		SetColor(10);
		printf("----------  ■■■■■■■  --------  -----------------------------------------");
		gotoxy(0, 17);
		SetColor(11);
		printf("----------  ■    ■    ■  ---------------------------------------------------");
		gotoxy(0, 18);
		SetColor(12);
		printf("----------  ■■■■■■■  -------  ■  -------  ■  -------------------------");
		gotoxy(0, 19);
		SetColor(13);
		printf("---------                    ------  ■  -------  ■  -------------------------");
		gotoxy(0, 20);
		SetColor(14);
		printf("------  ■■■■■■■■■■■  --  ■■  ------  ■  -------------------------");
		gotoxy(0, 21);
		SetColor(15);
		printf("----------------  ■  -----------  ■ - ■  ----  ■  ----------------  ■  ---");
		gotoxy(0, 22);
		SetColor(9);
		printf("---------------   ■  ----------  ■  -  ■  ---  ■  -  ■■■■■■   ■  ---");
		gotoxy(0, 23);
		SetColor(10);
		printf("---------------   ■  ---------  ■  ---  ■  --  ■  -----------  ■   ■  ---");
		gotoxy(0, 24);
		SetColor(11);
		printf("----------------  ■  --------  ■  -----  ■  -- ■  -----------  ■   ■  ---");
		gotoxy(0, 25);
		SetColor(12);
		printf("----------------  ■  -------  ▼  -------- ▼    ▼  -----------  ■   ■  ---");
		gotoxy(0, 26);
		SetColor(13);
		printf("----------------  ▼  --------  ------------  ----  -------------  ■   ■  ---");
		gotoxy(0, 27);
		SetColor(14);
		printf("------------------  ---------------------------------------------  ■   ■  ---");
		gotoxy(0, 28);
		SetColor(15);
		printf("-----------------------------------------------------------------  ▼   ■  ---");
		gotoxy(0, 29);
		SetColor(9);
		printf("-------------------------------------------------------------------  -  ■  ---");
		gotoxy(0, 30);
		SetColor(10);
		printf("----------------------------------------------------------------------  ■  ---");
		gotoxy(0, 31);
		SetColor(11);
		printf("----------------------------------------------------------------------  ■  ---");
		gotoxy(0, 32);
		SetColor(12);
		printf("----------------------------------------------------------------------  ■  ---");
		gotoxy(0, 33);
		SetColor(13);
		printf("----------------------------------------------------------------------  ■  ---");
		gotoxy(0, 34);
		SetColor(14);
		printf("----------------------------------------------------------------------  ■  ---");
		gotoxy(0, 35);
		SetColor(15);
		printf("----------------------------------------------------------------------  ▼  ---");
		gotoxy(0, 36);
		SetColor(9);
		printf("                                                                               ");
		gotoxy(0, 37);
		SetColor(10);
		printf("                                                                               ");
		gotoxy(0, 38);
		SetColor(11);
		printf("                                                                               ");
		gotoxy(0, 41);
		SetColor(12);
		printf("                                                                               ");
		gotoxy(0, 40);
		SetColor(13);
		printf("                                                                               ");
		gotoxy(0, 39);
		SetColor(15);
		printf("▶▶▶▶▶▶▶▶▶▶▶▶▶▶ 아무키나 입력하시오! ◀◀◀◀◀◀◀◀◀◀◀◀◀◀◀");

		Sleep(10);



		if (_kbhit())
		{
			int num = _getch();

			if (num != 0)
			{
				break;
			}

		}

	}
	SetColor(15);
	gotoxy(27, 41);
	printf("□□□□□□□□□□0%%");
	Sleep(500);          //0.5초 대기
	gotoxy(27, 41);
	printf("■□□□□□□□□□10%%");
	Sleep(1000);          //1초대기
	gotoxy(27, 41);
	printf("■■□□□□□□□□20%%");
	Sleep(100);             //0.1초대기
	gotoxy(27, 41);
	printf("■■■□□□□□□□30%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■□□□□□□40%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■■□□□□□50%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■■■□□□□60%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■■■■□□□70%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■■■■■□□80%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■■■■■■□90%%");
	Sleep(100);
	gotoxy(27, 41);
	printf("■■■■■■■■■■100%%");
	Sleep(500);
	gotoxy(27, 41);
	printf("■■■■■■■■■■100%%");

}


// 배경그리기함수
void draw_background()
{
	int i = 0;
	SetColor(15);
	gotoxy(0, 0);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━┓Level %d \n", stage);
	printf("┃             벽   돌  뿌  시  기                  ┃목숨 ");
	SetColor(12);
	for (i = 0; i<heart; i++)
	{
		printf("♥");
	}
	printf("\n");
	SetColor(15);
	printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━┫시간:\n");
	for (i = 0; i < (FINAL_Y); i++)
	{
		printf("┃                                                  ┃\n");
	}
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}



// 볼이블록을부쉈는지확인
int update_blocks(void)
{
	int i, j;

	for (i = 0; i < BLK_YCNT; i++)
	{
		for (j = 0; j < BLK_XCNT; j++)
		{
			if (blocks_status[i][j] == 0)
			{
				continue;
			}
			else if ((ball_x >= blocks_x[j] && ball_x <= (blocks_x[j] + 6)) && (ball_y >= blocks_y[i] && ball_y <= (blocks_y[i] + 1)))
			{
				if (blocks_status[i][j] == 1)// 그냥 블럭
				{
					blocks_status[i][j] = 0;
				}
				else if (blocks_status[i][j] == 5)// 2번  블럭
				{
					blocks_status[i][j] = 1;
				}
				else if (blocks_status[i][j] == 2)// 바 길이 길게 
				{
					blocks_status[i][j] = 0;
					if (itemcount == 0)
					{
						itemcount = 1;
						item1_x = blocks_x[j] + 2;
						item1_y = blocks_y[i];
					}

				}
				else if (blocks_status[i][j] == 3)// 하트
				{
					blocks_status[i][j] = 0;
					if (itemcount == 0)
					{
						itemcount = 2;
						item2_x = blocks_x[j] + 2;
						item2_y = blocks_y[i];
					}

				}
				else if (blocks_status[i][j] == 4)// 바 길이 짧게 
				{
					blocks_status[i][j] = 0;
					if (itemcount == 0)
					{
						itemcount = 3;
						item3_x = blocks_x[j] + 2;
						item3_y = blocks_y[i];
					}

				}
				else if (blocks_status[i][j] == 6)// 죽음 
				{
					blocks_status[i][j] = 0;
					if (itemcount == 0)
					{
						itemcount = 4;
						item4_x = blocks_x[j] + 2;
						item4_y = blocks_y[i];
					}

				}
				else if (blocks_status[i][j] == 7)// 통과 
				{
					blocks_status[i][j] = 0;
					if (itemcount == 0)
					{
						itemcount = 5;
						item5_x = blocks_x[j] + 2;
						item5_y = blocks_y[i];
					}

				}
				StopMusic();
				PlayMusic("sub.mp3");

				gotoxy(blocks_x[j], blocks_y[i]);
				printf("       ");
				gotoxy(blocks_x[j], blocks_y[i] + 1);
				printf("       ");
				return 1;
			}
		}
	}
	return 0;
}

//게임의 끝남 유무
int check_gameover(void)
{
	int i, j;
	int is_over = 1;
	for (i = 0; i < BLK_YCNT; i++)
	{
		for (j = 0; j < BLK_XCNT; j++)
		{
			if (blocks_status[i][j] == 1 || blocks_status[i][j] == 2 || blocks_status[i][j] == 3 || blocks_status[i][j] == 4 ||
				blocks_status[i][j] == 5 || blocks_status[i][j] == 6 || blocks_status[i][j] == 7)
			{
				is_over = 0;
			}
		}
	}
	return is_over;
}


//블럭 그리기
void draw_all_blocks(void)
{

	int i, j;
	for (i = 0; i < BLK_YCNT; i++)
	{
		for (j = 0; j < BLK_XCNT; j++)
		{
			if (blocks_status[i][j] != 0 && blocks_status[i][j] != 5)
			{
				gotoxy(blocks_x[j], blocks_y[i]);
				SetColor(11);
				printf("┌──┐");
				gotoxy(blocks_x[j], blocks_y[i] + 1);
				printf("└──┘");
			}
			if (blocks_status[i][j] == 5)
			{
				gotoxy(blocks_x[j], blocks_y[i]);
				SetColor(12);
				printf("┌──┐");
				gotoxy(blocks_x[j], blocks_y[i] + 1);
				printf("└──┘");

			}
			if (blocks_status[4][0] == 1)
			{
				blocks_status[4][0] = 3;
			}


		}
	}
}


// 바그리기함수
void draw_bar(int bar_x, int bar_y)
{
	SetColor(15);
	if (item1 == 0)
	{
		gotoxy(bar_x, bar_y);
		printf("■■■■■");
		it = 42;
	}
	if (item1 == 1)
	{
		SetColor(15);
		gotoxy(bar_x, bar_y);
		printf("■■■■■■");
		it = 40;
	}
	if (item1 == 3)
	{
		SetColor(15);
		gotoxy(bar_x, bar_y);
		printf("■■■■");
		it = 44;
	}
}
//아이템 관련 함수
void Item(void)
{
	if (itemcount == 1)
	{
		SetColor(15);
		gotoxy(item1_x, item1_y + a);
		printf(" ");
		a++;
		gotoxy(item1_x, item1_y + a);
		printf("■");

		if (item1_y + a == FINAL_Y - 1 && item1_x >= bar_x && item1_x <= bar_x + 10)
		{
			if (item1 == 0)
			{
				item1 = 1;
				temsize = 2;
			}
			else if (item1 == 3)
			{
				item1 = 0;
				temsize = 2;
			}
		}
		if (item1_y + a == FINAL_Y)
		{
			a = 0;
			gotoxy(item1_x, item1_y + a);
			printf(" ");
			itemcount = 0;
		}
	}

	if (itemcount == 2)
	{

		gotoxy(item2_x, item2_y + a);
		printf(" ");
		a++;
		SetColor(12);
		gotoxy(item2_x, item2_y + a);
		printf("♥");

		if (item2_y + a == FINAL_Y - 1 && item2_x >= bar_x && item2_x <= bar_x + 10)
		{
			item2 = 1;
		}
		if (item2_y + a == FINAL_Y)
		{
			a = 0;
			gotoxy(item2_x, item2_y + a);
			printf(" ");
			itemcount = 0;
		}


	}
	if (itemcount == 3)
	{
		SetColor(15);
		gotoxy(item3_x, item3_y + a);
		printf(" ");
		a++;
		gotoxy(item3_x, item3_y + a);
		printf("□");

		if (item3_y + a == FINAL_Y - 1 && item3_x >= bar_x && item3_x <= bar_x + 10)
		{
			if (item1 == 1)
			{
				item1 = 0;
				temsize = 2;
			}
			else if (item1 == 0)
			{
				item1 = 3;
				temsize = 2;
			}
		}
		if (item3_y + a == FINAL_Y)
		{
			a = 0;
			gotoxy(item3_x, item3_y + a);
			printf(" ");
			itemcount = 0;
		}

	}
	if (itemcount == 4)
	{

		gotoxy(item4_x, item4_y + a);
		printf(" ");
		a++;
		SetColor(9);
		gotoxy(item4_x, item4_y + a);
		printf("▣");

		if (item4_y + a == FINAL_Y - 1 && item4_x >= bar_x && item4_x <= bar_x + 10)
		{
			item4 = 1;
			heart = 0;
		}
		if (item4_y + a == FINAL_Y)
		{
			a = 0;
			gotoxy(item4_x, item4_y + a);
			printf(" ");
			itemcount = 0;
		}


	}
	if (itemcount == 5)
	{
		time_s = clock() / 1000;
		SetColor(12);
		gotoxy(item5_x, item5_y + a);
		printf(" ");
		a++;
		gotoxy(item5_x, item5_y + a);
		printf("●");
		if (item5_y + a == FINAL_Y - 1 && item5_x >= bar_x && item5_x <= bar_x + 10)
		{

			item5 = 1;

		}
		if (item5_y + a == FINAL_Y)
		{
			a = 0;
			gotoxy(item5_x, item5_y + a);
			printf(" ");
			itemcount = 0;
		}
	}


}

// 메인함수
int main(void)
{

	PlayMusic1("sub.mp3");


	struct member m[3];
	char an[10];
	// 랭킹 이름
	int min = 0;
	int sec = 0;
	int i, j = 0, k;

	srand((int)time(NULL));
	ball_x = START_X + rand() % (((50 - 2) + 1) + 0);
	ball_y = START_Y + 20;
	change_x = 1;
	change_y = 1;

	start();
	gta = clock() / 1000;
x:      /////스테이지 이동
	system("cls");
	draw_bar(bar_x, bar_y);
	// 배경화면만들기
	draw_background();
	// 벽돌그리기
	draw_all_blocks();
	// 바그리기

	for (i = 0; 1; i++)
	{
		if (clock() % 10 == 0)
		{
			draw_all_blocks();
		}
		if (heart == 3)
		{
			SetColor(12);
			gotoxy(65, 1);
			printf("  ");
		}
		if (heart == 2)
		{
			SetColor(12);
			gotoxy(63, 1);
			printf("  ");
		}
		if (heart == 1)
		{
			SetColor(12);
			gotoxy(61, 1);
			printf("  ");
		}

		if (item2 == 1)
		{
			for (heartp = 0; heartp<heart; heartp++)
			{
				printf("  ");
			}
			gotoxy(61, 1);
			heart = heart + 1;
			if (heart >= 4)
			{
				heart = 4;
			}

			for (heartp = 0; heartp<heart - 1; heartp++)
			{

				SetColor(12);
				printf("♥");
				item2 = 0;
			}
		}


		if (_kbhit())
		{
			char ch = _getch();
			// 게임종료
			if (ch == 'p' || ch == 'P')
			{
				SetColor(7);
				gtb = clock() / 1000;
				gotoxy(20, 10);
				printf("일시정지!!!!");
				gotoxy(15, 11);
				printf("정지해지시 'P' 를 누르세요");
				for (;;)
				{
					char ch = _getch();
					if (ch == 'p' || ch == 'P')
					{
						gtc = clock() / 1000 - gtb;
						gotoxy(20, 10);
						printf("               ");
						gotoxy(15, 11);
						printf("                           ");
						ch = 0;
						break;
					}

				}

			}
			// 바그리기   
		}
		if (clock() % 5 == 0)
		{
			SetColor(15);
			gotoxy(60, 2);
			{
				gt = clock() / 1000;
				if (stage == 1)
				{
					gt = gt - gta - gtc;
				}
				if (stage == 2)
				{
					gt = gt = gt - gta - gtc - 5;
				}
				if (stage == 3)
				{
					gt = gt = gt - gta - gtc - 10;
				}
				gtm = gt / 60;
				gt = gt % 60;
				printf("%d 분 %d 초", gtm, gt);
			}
		}
		GetCursorPos(&mouse);
		bar_x = mouse.x / 10 - 16;
		bar_y = mouse.y / 10 - 28;
		if (bar_x<2)
		{
			bar_x = 2;
		}
		else if (bar_x>it)
		{
			bar_x = it;
		}
		if (bar_y <= 36)
		{
			bar_y = 36;
		}
		else if (bar_y>36)
		{
			bar_y = 37;
		}
		SetColor(15);
		gotoxy(2, 36);
		printf("                                                  ┃");
		gotoxy(2, 37);
		printf("                                                  ┃");
		draw_bar(bar_x, bar_y);

		Sleep(10);
		// 볼위치갱신해서그리기
		if (heart == 0)
		{
			StopMusic1();
			PlayMusic2("sub.mp3");
			gotoxy(60, 1);
			printf("              ");
			SetColor(12);
			gotoxy(58, 1);
			printf("  ");
			gotoxy(15, 8);
			printf("GAME OVER!!\n");
			FILE *f;
			f = fopen("a.txt", "r");

			for (k = 0; k<3; k++)
				fscanf(f, "%s %d %d", &m[k].name, &m[k].minute, &m[k].seconds);
			f = fopen("a.txt", "w");


			for (k = 0; k<3; k++)

				fprintf(f, "%s %5d %5d\n", m[k].name, m[k].minute, m[k].seconds);


			f = fopen("a.txt", "r");
			for (k = 0; k<3; k++)
			{
				gotoxy(5, k + 12);
				printf("%d등  ", k + 1);
				printf("player %s 시간 %d 분  %d초 \n", m[k].name, m[k].minute, m[k].seconds);
			}

			fclose(f);
			for (;;)
			{
				if (_kbhit())
				{
					int num = _getch();

					if (num != 0)
					{
						break;
					}

				}
			}

			return -1;
		}

		if (i%gamespeed == 0)            //속도조절
		{
			gotoxy(ball_x, ball_y);
			printf("  ");
			if (ball_x <= START_X && change_x < 0)
			{
				change_x *= -1;
			}
			if (ball_x >= FINAL_X && change_x > 0)
			{
				change_x *= -1;
			}
			if (ball_y <= START_Y && change_y < 0)
			{
				change_y *= -1;
			}

			if (ball_y >= (bar_y))
			{

				if (ball_x >= bar_x && ball_x <= (bar_x + BAR_SIZE1 + temsize))
				{
					change_y *= -1;
				}

				else
				{
					if (item2 == 0)
					{
						if (heart == 1)
						{
							StopMusic1();
							PlayMusic2("sub.mp3");
							gotoxy(60, 1);
							printf("              ");
							SetColor(12);
							gotoxy(58, 1);
							printf("  ");
							gotoxy(15, 8);
							printf("GAME OVER!!\n");
							FILE *f;
							f = fopen("a.txt", "r");

							for (k = 0; k<3; k++)
								fscanf(f, "%s %d %d", &m[k].name, &m[k].minute, &m[k].seconds);
							f = fopen("a.txt", "w");


							for (k = 0; k<3; k++)

								fprintf(f, "%s %5d %5d\n", m[k].name, m[k].minute, m[k].seconds);


							f = fopen("a.txt", "r");
							for (k = 0; k<3; k++)
							{
								gotoxy(5, k + 12);
								printf("%d등  ", k + 1);
								printf("player %s 시간 %d 분  %d초 \n", m[k].name, m[k].minute, m[k].seconds);
							}
							fclose(f);
							for (;;)
							{
								if (_kbhit())
								{
									int num = _getch();

									if (num != 0)
									{
										break;
									}

								}
							}

							return -1;
						}
						else
						{
							heart = heart - 1;
							ball_x = START_X + rand() % (((50 - 2) + 1) + 0);
							ball_y = START_Y + 20;
							continue;
						}
					}
				}
			}
			ball_x += change_x;
			ball_y += change_y;
			if (i % 2 == 0)
			{
				Item();
			}
			if (update_blocks() == 1)
			{
				if (item5 == 0)
				{
					change_y *= -1;
				}
				else if (item5 == 1)
				{
					change_y *= 1;
					time_f = clock() / 1000;
					if (time_f - time_s >= 5)
					{
						item5 = 0;
					}
				}
			}
			SetColor(15);
			gotoxy(ball_x, ball_y);
			if (item5 == 1)
			{
				SetColor(12);
			}
			else if (item5 == 0)
			{
				SetColor(15);
			}
			printf("●");
			// 게임종료확인
			if (check_gameover() == 1)
			{
				if (stage == 1)
				{
					StopMusic1();
					PlayMusic3("sub.mp3");
					gotoxy(15, 8);
					printf("Next stage!");
					time_s = clock() / 1000;
					for (;;)
					{
						time_f = clock() / 1000;
						if (time_f - time_s >= 5)
						{
							break;
						}
					}
					PlayMusic1("sub.mp3");
					system("cls");
					stage = stage + 1;
					ball_x = START_X + rand() % (((50 - 2) + 1) + 0);
					ball_y = START_Y + 20;
					back1();
					goto x;
				}
				if (stage == 2)
				{
					StopMusic1();
					PlayMusic3("sub.mp3");
					gotoxy(15, 8);
					printf("Next stage!");
					time_s = clock() / 1000;
					for (;;)
					{
						time_f = clock() / 1000;
						if (time_f - time_s >= 5)
						{
							break;
						}
					}
					PlayMusic1("sub.mp3");
					system("cls");
					stage = stage + 1;
					ball_x = START_X + rand() % (((50 - 2) + 1) + 0);
					ball_y = START_Y + 20;
					back2();
					goto x;
				}
				StopMusic1();
				PlayMusic("sub.mp3");
				gotoxy(15, 8);
				printf("STAGE CLEAR!");
				gotoxy(3, 10);
				min = gtm;
				sec = gt;
				FILE *f;
				f = fopen("a.txt", "r");

				for (k = 0; k<3; k++)
					fscanf(f, "%s %d %d", &m[k].name, &m[k].minute, &m[k].seconds);


				if (min * 60 + sec <= m[0].minute * 60 + m[0].seconds)
				{
					m[2].minute = m[1].minute;
					m[2].seconds = m[1].seconds;
					strcpy(m[2].name, m[1].name);
					m[1].minute = m[0].minute;
					m[1].seconds = m[0].seconds;
					strcpy(m[1].name, m[0].name);
					printf("랭킹에 들었습니다 player 이름을 입력해주세요 : ");
					scanf("%s", an);
					m[0].minute = min;
					m[0].seconds = sec;
					strcpy(m[0].name, an);
				}
				else if (min * 60 + sec <= m[1].minute * 60 + m[1].seconds)
				{
					m[1].minute = m[0].minute;
					m[1].seconds = m[0].seconds;
					strcpy(m[2].name, m[1].name);
					printf("랭킹에 들었습니다 player 이름을 입력해주세요 : ");
					scanf("%s", an);
					m[1].minute = min;
					m[1].seconds = sec;
					strcpy(m[1].name, an);
				}
				else if (min * 60 + sec <= m[2].minute * 60 + m[2].seconds)
				{
					printf("랭킹에 들었습니다 player 이름을 입력해주세요 : ");
					scanf("%s", an);
					m[2].minute = min;
					m[2].seconds = sec;
					strcpy(m[2].name, an);
				}
				f = fopen("a.txt", "w");


				for (k = 0; k<3; k++)

					fprintf(f, "%s %5d %5d\n", m[k].name, m[k].minute, m[k].seconds);


				f = fopen("a.txt", "r");
				for (k = 0; k<3; k++)
				{
					gotoxy(5, k + 12);
					printf("%d등  ", k + 1);
					printf("player %s 시간 %d 분  %d초 \n", m[k].name, m[k].minute, m[k].seconds);
				}

				fclose(f);
				for (;;)
				{
					if (_kbhit())
					{
						int num = _getch();

						if (num != 0)
						{
							break;
						}

					}
				}
				return 0;
			}

		}

	}
	return 0;
}