#include"basicstd.h"

void draw_note(int x, int y,char note[]);
void move_note(int x, int y);
void draw(int vac[4][21]);
void del(int vac[4][21]);
void note_to_note(int *i,int notelen,char f_note[][5], int note[4][21]);//불러온 노트 배열을 게임 으로 출력

char file_note[20000][5];//
int game_note[20000][4];//

int main(void)
{
	int num=0,i, j,n_of_file;//n_of_file 노트 줄의 갯수(0.1초 기준)
	int note[4][21] = { 0, 0, };
	char temp[4] = { 0, };
	//
	FILE *notefile;
 	notefile = fopen("sound/songs/black_swan.txt", "r");
	if (notefile == NULL)
		printf("파일 불러오기 실패\n");
	//파일 길이 측정
	//**********************************
	fseek(notefile, 0L, SEEK_END);
	n_of_file = ftell(notefile);
	n_of_file = (n_of_file/6) +1;
	//********************************
	//파일을 가져온 후, 분석하여 배열에 저장
	//**********************************************
	fseek(notefile, 0, SEEK_SET);
	for (i = 0; i < n_of_file; i++)
	{
		for (j = 0; j < 5; j++)
		{
			file_note[i][j] = fgetc(notefile);
			fseek(notefile, 0L, SEEK_CUR);
		}
	}
	//**********************************************//
	//
	for (;;)
	{
		note_to_note(&num,n_of_file,file_note, note);
		draw(note);
		Sleep(90);
		for (i = 0; i < 4; i++)
		{
			for (j = 20; j > 0; j--)
			{
				note[i][j] = note[i][j-1];
				note[i][j-1] = 0;
			}
			

		}
		note[i][20] = 0;
	}
	
	
	
	
}
void draw(int vac[4][21])
{
	int x=0, y=0;
	int i, j;
	gotoxy(x, y);
	for (i = 0; i < 21; i++)
	{
		gotoxy(x, y);
		for (j = 0; j < 4; j++)
		{
			if (vac[j][i] == 1)
			{
				printf("●");
			}
			else if (vac[j][i] == 0)
			{
				printf("○");
			}
		}
		y += 1;
	}
}
void del(int vac[4][21])
{
	int x = 0, y = 0;
	int i, j;
	gotoxy(x, y);
	for (i = 0; i < 21; i++)
	{
		gotoxy(x, y);
		for (j = 0; j < 4; j++)
		{
			if (vac[j][i] == 1)
			{
				printf("  ");
			}
			else if (vac[j][i] == 0)
			{
				printf("  ");
			}
		}
		y += 1;
	}
}
void note_to_note(int *num,int notelen, char f_note[][5], int note[4][21])
{
	int j,c;
	for (j = 0; j < 4; j++)
	{
		if (f_note[(*num)][j] == '0')
			note[j][0] = 0;
		else if (f_note[(*num)][j] == '1')
			note[j][0] = 1;
	}
	*num +=1;
}