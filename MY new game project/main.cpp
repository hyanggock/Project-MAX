//헤더 선언 부분
//***************************#
#include "basicstd.h"//gotoxy,move_cursor,기본적인 함수들 포함.
#include<fmod.hpp>
#include<fmod.h>
#include<fmod_errors.h>
#pragma comment(lib,"fmodex_vc.lib")


//정의 부분
//******************************
FMOD::System *fmodsystem = 0;
FMOD::Sound *sounds[3] = { 0, };
FMOD::Channel *channel[3] = { 0, };

FMOD_RESULT result;

//FMOD 함수 선언 부분
//********************************
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}
//*********************************
//서브 함수 선언 부분
//***********************
int  draw_title(int x, int y);//         타이틀화면
int draw_maker();//					     제작자 소개
void draw_loading_screen();//            로딩 화면(애니메이션)
int draw_select_song(int x_m, int y_m);//곡 선택 부분
void draw_equalizer(int *equalizer_count,char pic[][8]);// 곡 선택 화면-이퀄라이져
void draw_song(int *sel,char key,struct song *);//곡 선택 화면-곡 목록 그리기
void draw_setting();//			     곡 선택 화면-설정화면
void auto_sync();//                      설정화면 - 자동 싱크 모듈
void draw_sync_check_note(int vac[][11]);//자동 싱크 모듈 테스트 함수
void wirte_somnail(struct song *);//     곡 선택 화면-곡정보 작성
void move_draw_song(char key,struct song *);//곡 선택 화면-곡 목록 옮기기
int draw_game_screen(int note_set,int spd,int song,int diff);//게임 화면
void draw_key_effect(char *key);//       게임화면-키를 누를 시 기어의 반응
void delete_key_FX(char *key);//         게임화면-기어 반응 후 초기화
void draw_note(int set,int vac[4][24]); //게임화면-노트 그리기
void note_to_note(int *num, int notelen, char f_note[][5], int note[4][24]);//게임화면-텍스트로 불러온 노트를 게임으로 이식
void draw_offset();//					 게임화면-음악과 노트 싱크를 조절하는 값
int judge_acc(char *key, int note[4][24]);//게임화면-정확도 판단 함수
void draw_judge(int *jud);//			 게임화면-정확도에 따라서 이펙트.
void erease_judge();//					 게임화면-이펙트를 삭제
void draw_score_and_combo();//           게임화면-점수와 콤보를 표현
int stopmenu();//						 게임화면-정지메뉴
int draw_result();//                     게임화면-결과화면
void move_set_cursor(char key, int *y, int y_m);//셋팅 커서 움직이기
//****************************

//*************************

//구조체 선언
//*********************************
struct song
{
	char songname[20];
	char artist[20];
	int level;
};
//*********************************
//전역 변수 사용 부분
//****************************
int score;
int combo;
int max_combo;
int offset= 75;
double acc;
int notecount;
char g_key[4] = { 'd', 'f', 'j', 'k' };
int note_set = 1;
//****************************

//메인 함수
int main(void)
{
	ALL_START:
	FMOD_SYSTEM      *system;
	FMOD_SOUND       *sound1, *sound2;
	FMOD_CHANNEL     *channel = 0;
	FMOD_RESULT       result;

	unsigned int      version;

	result = FMOD_System_Create(&system);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(system, &version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}
	result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(system, "sound/bgm/Menu_sound.mp3", FMOD_LOOP_NORMAL, 0, &sound1);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(system, "sound/SFX/select_song_sound.mp3", FMOD_SOFTWARE, 0, &sound2);
	ERRCHECK(result);

	//***********************************************FMOD
	START:
	int i, x, y, select,main_error=0;
	x = 69; y = 2;

	result = FMOD_System_CreateSound(system, "sound/bgm/Menu_sound.mp3", FMOD_LOOP_NORMAL, 0, &sound1);
	ERRCHECK(result);
	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound1, 0, &channel);
	ERRCHECK(result);
	select = draw_title(x, y);
	
	for (;;)
	{
		if (select == 0)
		{
			if (sound1 != NULL)
			{
				result = FMOD_Sound_Release(sound1);
				ERRCHECK(result);
			}
			result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound2, 0, &channel);
			ERRCHECK(result);
			for (i = 0; i < 2; i++)
			{
				draw_loading_screen();
				Sleep(200);
			}
			main_error = draw_select_song(X_MAX, Y_MAX);
		}
		else if (select == 1)
		{
			return 0;
			break;
		}
		else if (select == 2)
		{
			result = FMOD_Sound_Release(sound1);
			ERRCHECK(result);
			draw_maker();
			
			goto START;
		}
		if (main_error == 1)
			return 0;
		else if (main_error == 2)
			goto ALL_START;

	}
	
	
}

//서브 함수 정리부분
int draw_maker()
{
	FMOD_SYSTEM      *sys;
	FMOD_SOUND       *sound1, *sound2;
	FMOD_CHANNEL     *channel = 0;
	FMOD_RESULT       result;

	unsigned int      version;

	result = FMOD_System_Create(&sys);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(sys, &version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}
	result = FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/bgm/setting_bgm.mp3", FMOD_LOOP_NORMAL, 0, &sound1);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/SFX/return_sound.mp3", FMOD_SOFTWARE, 0, &sound2);
	ERRCHECK(result);

	//***********************************************FMOD

	char key;
	result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound1, 0, &channel);
	ERRCHECK(result);
	for (;;)
	{
		

		system("cls");
		printf("제작자 : 우성원\n");
		printf("학번 : 20141339\n");
		printf("버젼 : 0.0.1.2\n");
		printf("\n변경사항\n\n");
		printf("Experiment(0.0.0.0) : 테스트용. \n");
		printf("0.0.0.1 : 릴리즈 및 싱크 조절 기능 추가\n");
		printf("0.0.0.2 : 호환성 문제 해결, 점수 기능 및 기타 기능 추가\n");
		printf("0.0.0.4 : (Naver : '소원'님 제보) 판정선 추가 및 결과화면 후 플레이 가능\n");
		printf("0.0.0.5 : magnolia 곡 추가, 커스터마이징 추가\n");
		printf("0.0.0.6 : 알고리즘 개선(느린 cpu에서의 속도 개선)\n");
		printf("0.0.0.8 : 이퀄라이져 개선,first kiss 곡 추가,더욱 편리한 싱크 맞추기\n");
		printf("0.0.1.0 : 오토싱크 기능 추가, 도움말 기능\n");
		printf("0.0.1.1 : 오토 싱크 정확도 개선, 기능 일부 개선\n");
		printf("0.0.1.2 : windows10 및 Visusl studio 2019 지원(LTS)");
		printf("\n\n dj max 제작자분들을 존경합니다.");
		printf("\n\n\n\n돌아가기 : space");

		key = _getch();

		if (key == 32)
		{
			result = FMOD_Sound_Release(sound1);
			ERRCHECK(result);
			result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound2, 0, &channel);
			ERRCHECK(result);
			Sleep(500);
			ERRCHECK(result);
			system("cls");
			break;
		}
		else
			system("cls"); continue;
	}

}
int draw_title(int x, int y)
{
	FMOD_SYSTEM      *sys;
	FMOD_SOUND       *sound1;
	FMOD_CHANNEL     *channel = 0;
	FMOD_RESULT       result;

	unsigned int      version;

	result = FMOD_System_Create(&sys);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(sys, &version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}
	result = FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/SFX/menu_button_sound.mp3", FMOD_HARDWARE, 0, &sound1);
	ERRCHECK(result);

	//***********************************************FMOD

	char key;
	for (;;)
	{

		/*63칸  |*/  /*73 |*/
		printf("                        ######                                        \n");
		printf("                        #######                          ##           \n");
		printf("        ,#######         ######                     ### ###           1.시작\n");
		printf("   ################      #######              ####   #####            2.나가기\n");
		printf("   ##################     ######        ###   #####    ####           3.제작자\n");
		printf("    ##################    ####### ###  ####   ## ###  #######      \n");
		printf("    #######    #######     ###### ##########  ####### ##    ##     \n");
		printf("     ######     #######    ######  ###### ### ##   ###             \n");
		printf("     #######    ####### #########  ##  ##  ## #                    \n");
		printf("      ######   ####### ,#########  ##                              \n");
		printf("      ################ #########                                   \n");
		printf("       ##############                최종 버젼 + LTS               \n");
		printf("       ############                                                \n");
		printf("        ######                       ver 0.0.1.2                   \n");
		printf(" \n");
		printf("↑,↓ : 커서이동     space : 선택                                   \n");
		for (;;)
		{
			gotoxy(x, y); printf("\b\b"); printf("▶");
			key = _getch();
			if (y >= 2 && y <= 4)//메뉴 선택
			{
				if (key == 72 || key == 80)
				{
					result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound1, 0, &channel);
					ERRCHECK(result);
					printf("\b\b"); printf("  ");
					move_cursor(key, &x, &y, X_MAX, Y_MAX);
					if (y>4) y -= 3;
					if (y<2) y += 3;
				}
				else if (key == 32)//스페이스로 선택
				{
					if (y == 2)
					{
						system("cls");
						return 0;
					}
					else if (y == 3)
					{
						system("cls");
						return 1;
					}
					else if (y == 4)
					{
						system("cls");
						return 2;
					}
				}
				else
				{
					continue;
				}
			}
		}

	}

}

void draw_loading_screen()
{
	//로딩스크린 수정 가능
	system("cls");
	printf("                        ######                                      \n");
	printf("                        #######                          ##         \n");
	printf("                         ######                     ### ###         \n");
	printf("                         #######              ####   #####          \n");
	printf("                          ######        ###   #####    ####         \n");
	printf("                          ####### ###  ####   ## ###  #######      \n");
	printf("                           ###### ##########  ####### ##    ##     \n");
	printf("                           ######  ###### ### ##   ###             \n");
	printf("                        #########  ##  ##  ## #                    \n");
	printf("                       ,#########  ##                              \n");
	printf("                       #########                                   \n");
	printf("                                                                   \n");
	printf("                                                                   \n");
	printf("                                                                   \n");
	Sleep(150);
	system("cls");
	printf("                                                                    \n");
	printf("                                                         ##         \n");
	printf("        ,#######                                    ### ###         \n");
	printf("   ################                           ####   #####          \n");
	printf("   ##################                   ###   #####    ####         \n");
	printf("    ##################            ###  ####   ## ###  #######      \n");
	printf("    #######    #######            ##########  ####### ##    ##     \n");
	printf("     ######     #######            ###### ### ##   ###             \n");
	printf("     #######    #######            ##  ##  ## #                    \n");
	printf("      ######   #######             ##                              \n");
	printf("      ################                                             \n");
	printf("       ##############                                              \n");
	printf("       ############                                               \n");
	printf("        ######                                                     \n");
	Sleep(150);
	system("cls");
	printf("                        ######                                      \n");
	printf("                        #######                          ##         \n");
	printf("        ,#######         ######                     ### ###         \n");
	printf("   ################      #######              ####   #####          \n");
	printf("   ##################     ######              #####    ####         \n");
	printf("    ##################    #######             ## ###  #######      \n");
	printf("    #######    #######     ######             ####### ##    ##     \n");
	printf("     ######     #######    ######             ##   ###             \n");
	printf("     #######    ####### #########             #                    \n");
	printf("      ######   ####### ,#########                                  \n");
	printf("      ################ #########                                   \n");
	printf("       ##############                                              \n");
	printf("       ############                                                \n");
	printf("        ######                                                     \n");
	Sleep(150);
	system("cls");
	printf("                        ######                                      \n");
	printf("                        #######                          ##         \n");
	printf("        ,#######         ######                     ### ###         \n");
	printf("   ################      #######                     #####          \n");
	printf("   ##################     ######        ###            ####         \n");
	printf("    ##################    ####### ###  ####           #######      \n");
	printf("    #######    #######     ###### ##########          ##    ##     \n");
	printf("     ######     #######    ######  ###### ###                      \n");
	printf("     #######    ####### #########  ##  ##  ##                      \n");
	printf("      ######   ####### ,#########  ##                              \n");
	printf("      ################ #########                                   \n");
	printf("       ##############                                              \n");
	printf("       ############                                                \n");
	printf("        ######                                                     \n");
	Sleep(150);
	system("cls");
	printf("                        ######                                      \n");
	printf("                        #######                                     \n");
	printf("        ,#######         ######                                     \n");
	printf("   ################      #######              ####                  \n");
	printf("   ##################     ######        ###   #####                 \n");
	printf("    ##################    ####### ###  ####   ## ###               \n");
	printf("    #######    #######     ###### ##########  #######              \n");
	printf("     ######     #######    ######  ###### ### ##   ###             \n");
	printf("     #######    ####### #########  ##  ##  ## #                    \n");
	printf("      ######   ####### ,#########  ##                              \n");
	printf("      ################ #########                                   \n");
	printf("       ##############                                              \n");
	printf("       ############                                                \n");
	printf("        ######                                                     \n");
	Sleep(150);
	system("cls");
	printf("                        ######                                      \n");
	printf("                        #######                          ##         \n");
	printf("        ,#######         ######                     ### ###         \n");
	printf("   ################      #######              ####   #####          \n");
	printf("   ##################     ######        ###   #####    ####         \n");
	printf("    ##################    ####### ###  ####   ## ###  #######      \n");
	printf("    #######    #######     ###### ##########  ####### ##    ##     \n");
	printf("     ######     #######    ######  ###### ### ##   ###             \n");
	printf("     #######    ####### #########  ##  ##  ## #                    \n");
	printf("      ######   ####### ,#########  ##                              \n");
	printf("      ################ #########                                   \n");
	printf("       ##############                                              \n");
	printf("       ############                                                \n");
	printf("        ######                                                     \n");
	Sleep(200);
	system("cls");
	printf("                        ######                                      \n");
	printf("                        #######                          ##         \n");
	printf("        ,#######         ######                     ### ###         \n");
	printf("   ################      #######              ####   #####          \n");
	printf("   ##################     ######        ###   #####    ####         \n");
	printf("    ##################    ####### ###  ####   ## ###  #######      \n");
	printf("    #######    #######     ###### ##########  ####### ##    ##     \n");
	printf("     ######     #######    ######  ###### ### ##   ###             \n");
	printf("     #######    ####### #########  ##  ##  ## #                    \n");
	printf("      ######   ####### ,#########  ##                              \n");
	printf("      ################ #########                                   \n");
	printf("       ##############                                              \n");
	printf("       ############                                                \n");
	printf("        ######                                                     \n");
	Sleep(200);
}
int draw_select_song(int x_m, int y_m)
{
	SELECT_RESTART:
	FMOD_SYSTEM      *sys;
	FMOD_SOUND       *sound1, *sound2, *sound3, *move_sel;
	FMOD_CHANNEL     *g_channel[1] = { NULL };
	FMOD_RESULT       result;
	bool playing = 0;
	bool paused = 0;
	unsigned int ms = 0;
	unsigned int lenms = 0;

	FMOD_BOOL IsPlaying;

	int channelisplaying = 0;

	unsigned int      version;

	result = FMOD_System_Create(&sys);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(sys, &version);
	ERRCHECK(result);

	result = fmodsystem->init(32, FMOD_INIT_NORMAL, 0);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 1;
	}
	result = FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/somnail/first_kiss_somnail.mp3", FMOD_LOOP_NORMAL, 0, &sound1);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/somnail/black_swan_somnail.mp3", FMOD_LOOP_NORMAL, 0, &sound2);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/somnail/magnolia_somnail.mp3", FMOD_LOOP_NORMAL, 0, &sound3);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/SFX/djing_FX.mp3", FMOD_HARDWARE, 0, &move_sel);
	ERRCHECK(result);

	//***********************************************FMOD
	char key=0,offkey=NULL;
	char pic[8][8] = { 0, };
	int speed = 50, song = 1, difficulty = 1;//속도, 음악,난이도(1=NM 2=HD)
	int selected_music = 2, playing_music = 2, sw = 0, first = 1, l_r = 0;
	int cur_choice=4;
	struct song music[3];
	struct song temp;
	int i=0;
	int equalizer_count = 0;
	wirte_somnail(music);
	srand(time(NULL));
	system("cls");
	//10칸 기준
	//0123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|MAX=80         
	printf("  싱크 (노트속도) 조정 : + (느리게) - (빠르게)         s:게임 설정"); printf("\n");
	printf("   "); printf("\n");
	printf("  싱크   :          (ms)                               q:메인 화면으로"); printf("\n");
	printf("                              "); printf("\n");
	printf("  "); printf("\n");//***
	printf("    "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");//***
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");//***
	printf("                              ▼▼▼▼▼▼▼▼ "); printf("\n");
	printf("    곡명 :                   곡명 :                   곡명 :                   "); printf("\n");//곡명 : 15칸
	printf("    아티스트 :               아티스트 :               아티스트 :               "); printf("\n");//아티스트 : 11칸
	printf("    난이도 :                 난이도 :                 난이도 :                 "); printf("\n");//난이도 : 13칸
	printf("                              ▲▲▲▲▲▲▲▲ "); printf("\n");//***
	printf("     "); printf("\n");
	printf("   곡 이동 : ←,→    선택 : space");
	//END OF DOS Y_AXIS

	Sleep(800);

	for (;;)
	{
		
		if (first == 1)//첫 실행 시 사운드 재생
		{
			result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound2, 0, &g_channel[0]);
			first = 0;
		}

		if (sw == 1)//사운드를 풀어주는 함수
		{

			if (l_r == 1)
			{

				switch (playing_music)
				{
				case 1:
					FMOD_Sound_Release(sound1);
					break;
				case 2:
					FMOD_Sound_Release(sound2);
					break;
				case 3:
					FMOD_Sound_Release(sound3);
					break;
				}
				playing_music--;
				if (playing_music < 1)
					playing_music = 3;
			}
			else if (l_r == 2)
			{

				switch (playing_music)
				{
				case 1:
					
					ERRCHECK(result);
					
					FMOD_Sound_Release(sound1);
					break;
				case 2:
					
					ERRCHECK(result);
					
					FMOD_Sound_Release(sound2);
					break;
				case 3:
					
					ERRCHECK(result);
					FMOD_Sound_Release(sound3);
					break;

				}
				playing_music++;
				if (playing_music > 3)
					playing_music = 1;
			}

		}
		if (sw == 1)//곡을 변환하기위한 스위치
		{
			if (selected_music == 1)
			{
				result = FMOD_System_CreateSound(sys, "sound/somnail/first_kiss_somnail.mp3", FMOD_LOOP_NORMAL, 0, &sound1);
				result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound1, 0, &g_channel[0]);
				ERRCHECK(result);
				sw = 0;
			}
			else if (selected_music == 2)
			{
				result = FMOD_System_CreateSound(sys, "sound/somnail/black_swan_somnail.mp3", FMOD_LOOP_NORMAL, 0, &sound2);
				FMOD_Channel_Stop(g_channel[0]);
				result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound2, 0, &g_channel[0]);
				ERRCHECK(result);
				sw = 0;
			}
			else if (selected_music == 3)
			{
				result = FMOD_System_CreateSound(sys, "sound/somnail/magnolia_somnail.mp3", FMOD_LOOP_NORMAL, 0, &sound3);
				FMOD_Channel_Stop(g_channel[0]);
				result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound3, 0, &g_channel[0]);
				ERRCHECK(result);
				sw = 0;
			}
		}
		for (;;)
		{

			//잡 효과
			Sleep(100);
			if (_kbhit() == 1)
			{
				key = _getch();
				offkey = key;
			}
			draw_equalizer(&equalizer_count, pic);
			
			draw_song(&selected_music, key, music);
			draw_offset();

			if (offkey == '+')
			{
				offset++;
				offkey = 0;
			}
			else if (offkey == '-')
			{
				offset--;
				offkey = 0;
			}
			if (key == 's')
			{
				switch (playing_music)
				{
				case 1:
					FMOD_Sound_Release(sound1);
					break;
				case 2:
					FMOD_Sound_Release(sound2);
					break;
				case 3:
					FMOD_Sound_Release(sound3);
					break;
				}
				draw_setting();
				goto SELECT_RESTART;
			}
			if (key == 'q')
			{
				switch (playing_music)
				{
				case 1:
					FMOD_Sound_Release(sound1);
					break;
				case 2:
					FMOD_Sound_Release(sound2);
					break;
				case 3:
					FMOD_Sound_Release(sound3);
					break;
				}
				system("cls");
				return 2;
			}
			if (key == 32)
			{
				if (playing_music == 1)
				{
					result = FMOD_Sound_Release(sound1);
					ERRCHECK(result);
				}
				else if (playing_music == 2)
				{
					result = FMOD_Sound_Release(sound2);
					ERRCHECK(result);
				}
				else if (playing_music == 3)
				{
					result = FMOD_Sound_Release(sound3);
					ERRCHECK(result);
				}
				system("cls");
				cur_choice=draw_game_screen(note_set,speed, playing_music, difficulty);
				if (cur_choice == 8)
					goto SELECT_RESTART;
				else if (cur_choice == 5)
					return 2;
			}
			else if (key == 75 || key == 77)
			{
				move_draw_song(key, music);
				result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, move_sel, 0, &g_channel[0]);
				Sleep(100);
				ERRCHECK(result);
				if (key == 77)
				{

					selected_music--;
					if (selected_music < 1)
						selected_music = 3;
					l_r = 1;
					sw = 1;
					key = 0;
					break;
				}
				else if (key == 75)
				{

					selected_music++;
					if (selected_music > 3)
						selected_music = 1;
					l_r = 2;
					sw = 1;
					key = 0;
					break;
				}

			}
			if (cur_choice == 0)
				return 0;
			else if (cur_choice == 1)
				return 1;
			else if (cur_choice == 2)
				break;
		}
	}
}
void draw_equalizer(int *equalizer_count,char pic[][8])
{
	int x = 30, y = 5, i, j,k;

	
	if (*equalizer_count == 3)
	{
		for (i = 0; i < 8; i++)
		{
			k=rand() % 5;
			pic[i][k] = 1;
		}
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 7; j++)
			{
				if (pic[i][j] == 1)
				{
					pic[i][j + 1] = 1;
				}
			}
		}
		*equalizer_count = 0;
	}
	for (i = 0; i < 8; i++)
	{
		for (j = 7; j > 0; j--)
		{
			pic[i][j] = pic[i][j - 1];
			pic[i][j - 1] = 0;
		}


	}
	for (i = 0; i < 8; i++)
	{
		gotoxy(x, y);
		for (j = 0; j < 8; j++)
		{
			if (pic[j][i] == 1)
				printf("■");
			else if (pic[j][i]==0)
				printf("  ");
		}
		y += 1;
	}
	*equalizer_count=*equalizer_count+1;

	return;


}
void wirte_somnail(struct song *sn)
{

	strcpy_s(sn->songname, 20, "first kiss  ");
	strcpy_s(sn->artist, 20, "BJJ     ");
	sn->level = 3;

	strcpy_s((sn+1)->songname, 20, "black swan  ");
	strcpy_s((sn+1)->artist, 20, "Tak        ");
	(sn+1)->level = 6;

	strcpy_s((sn+2)->songname, 20, "magnolia    ");
	strcpy_s((sn+2)->artist, 20, "M2U        ");
	(sn+2)->level = 5;
	
}
void move_draw_song(char key,struct song *sn)
{
	struct song temp;
	int i;
	switch (key)
	{
	case 75:
		strcpy_s(temp.songname, _TRUNCATE, (sn)->songname);
		strcpy_s(temp.artist, _TRUNCATE, (sn)->artist);
		temp.level = (sn)->level;
		for (i = 0; i < 2; i++)
		{
			strcpy_s((sn + i)->songname, _TRUNCATE, (sn + i + 1)->songname);
			strcpy_s((sn + i)->artist, _TRUNCATE, (sn + i + 1)->artist);
			(sn + i)->level = (sn + i + 1)->level;
		}
		strcpy_s((sn + 2)->artist, _TRUNCATE, temp.artist);
		strcpy_s((sn + 2)->songname, _TRUNCATE, temp.songname);
		(sn + 2)->level= temp.level; 
		break;
	case 77:
		strcpy_s(temp.songname, _TRUNCATE, (sn+2)->songname);
		strcpy_s(temp.artist, _TRUNCATE, (sn+2)->artist);
		temp.level = (sn+2)->level;
		for (i = 2; i > 0; i--)
		{
			strcpy_s((sn + i)->songname, _TRUNCATE, (sn + i - 1)->songname);
			strcpy_s((sn + i)->artist, _TRUNCATE, (sn + i - 1)->artist);
			(sn + i)->level = (sn + i - 1)->level;
		}
		strcpy_s((sn)->artist, _TRUNCATE, temp.artist);
		strcpy_s((sn)->songname, _TRUNCATE, temp.songname);
		(sn)->level = temp.level;
		break;
	}
}
void draw_song(int *sel, char key,struct song *sn)
{
	int  y=16;
	gotoxy(11, y);
	printf("%s", sn->songname);
	gotoxy(16, y+1);
	printf("%s", sn->artist);
	gotoxy(14, y+2);
	printf("%d", sn->level);
	gotoxy(36, y);
	printf("%s", (sn + 1)->songname);
	gotoxy(41, y+1);
	printf("%s", (sn + 1)->artist);
	gotoxy(39, y+2);
	printf("%d", (sn + 1)->level);
	gotoxy(60, y);
	printf("%s", (sn + 2)->songname);
	gotoxy(65, y+1);
	printf("%s", (sn + 2)->artist);
	gotoxy(63, y+2);
	printf("%d", (sn + 2)->level);
}
int draw_game_screen(int note_set,int spd, int song, int diff)
{
	GAME_RESTART:
	FMOD_SYSTEM      *sys;
	FMOD_SOUND       *sound1, *sound2, *sound3, *move_sel;
	FMOD_CHANNEL     *g_channel[1] = { NULL };
	FMOD_RESULT       result;
	bool playing = 0;
	bool paused = 0;
	unsigned int ms = 0;
	unsigned int lenms = 0;

	FMOD_BOOL IsPlaying;

	int channelisplaying = 0;

	unsigned int      version;

	result = FMOD_System_Create(&sys);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(sys, &version);
	ERRCHECK(result);

	result = fmodsystem->init(32, FMOD_INIT_NORMAL, 0);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}
	result = FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/songs/first_kiss.mp3", FMOD_HARDWARE, 0, &sound1);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/songs/black_swan.mp3", FMOD_HARDWARE, 0, &sound2);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/songs/Magnolia.mp3", FMOD_HARDWARE, 0, &sound3);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/SFX/menu_button_sound.mp3", FMOD_HARDWARE, 0, &move_sel);
	ERRCHECK(result);
	//변수 선언
	//---------------------------------------------------------------
	int x = 10, y = 0;
	int i, j, k;
	int playlen = 0;
	char key = 0;
	char file_note[20000][5];
	int note[4][24] = { 0, 0, };
	int n_of_file = 0, num = 0;
	int judge = 0;
	int stop_select=0;
	double acc2=0.0;
	int play_count = 0;
	//----------------------------------------------------------------

	//초기값 설정
	//----------------------------------------------------------------
	score = 0;
	combo = 0;
	acc = 1;
	notecount = 0;
	//-------------------------------------------------------------------

	//텍스트 파일 불러오기
	//------------------------------------------------------------------
	FILE *notefile = NULL;
	if (song == 1)
	{
		notefile = fopen("sound/songs/first_kiss.txt", "r");
	}
	else if (song == 2)
	{
		notefile = fopen("sound/songs/black_swan.txt", "r");
	}
	else if (song == 3)
	{
		notefile = fopen("sound/songs/Magnolia.txt", "r");
	}
	if (notefile == NULL)
		printf("파일 불러오기 실패\n");
	//파일 길이 측정
	//**********************************
	fseek(notefile, 0L, SEEK_END);
	n_of_file = ftell(notefile);
	n_of_file = (n_of_file / 6) + 1;
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
	fclose(notefile);
	//**********************************************
	//---------------------------------------------------------------------


	for (i = 0; i < 21; i++)
	{
		gotoxy(x, y);
		for (j = 0; j < 4; j++)
		{
			printf("│      ");
		}
		printf("│");
		y += 1;
	}
	gotoxy(x, y);
	printf("├───┼───┼───┼───┤");
	y += 1;
	gotoxy(x, y);
	for (j = 0; j < 5; j++)
	{
		printf("│      ");
	}
	y += 1;
	gotoxy(x, y);
	printf("└───┴───┴───┴───┘");
	gotoxy(60, 2);
	printf("q:정지메뉴");
	gotoxy(44, 20);
	printf("--perfect");
	gotoxy(44, 18);
	printf("--Good");
	
	for (playlen = 0; playlen < n_of_file + 22; playlen++)
	{
		if (note[3][20]==1||note[2][20]==1||note[1][20]==1||note[0][20] == 1||score!=0)
		{
			if (play_count == 0)
			{
				if (song == 1)
				{
					result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound1, 0, &g_channel[0]);
					play_count++;
				}
				else if (song == 2)
				{
					result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound2, 0, &g_channel[0]);
					play_count++;
				}
				else if (song == 3)
				{
					result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound3, 0, &g_channel[0]);
					play_count++;
				}
			}
			ERRCHECK(result);
		}
		if (_kbhit())
			key = _getch();

		if (key == 'q')
		{
			if (song == 1)
			{
				FMOD_Sound_Release(sound1);
				ERRCHECK(result);
			}
			else if (song == 2)
			{
				FMOD_Sound_Release(sound2);
				ERRCHECK(result);
			}
			else if (song == 3)
			{
				FMOD_Sound_Release(sound3);
				ERRCHECK(result);
			}
			stop_select=stopmenu();
			if (stop_select == 8)
				return 8;
			else if (stop_select == 4)
				goto GAME_RESTART;
			else if (stop_select == 5)
				return 5;

		}
		if (key == 'd' || key == 'f' || key == 'j' || key == 'k')
		{
			draw_key_effect(&key);
			judge = judge_acc(&key, note);
		}
		if (note[0][23] == 1 || note[1][23] == 1 || note[2][23] == 1 || note[3][23] == 1)
			judge = 4;



		if (judge == 2)
		{
			combo++;
			score += 300;
			score += (6 * combo);
			notecount++;
			acc2 += 100.0;
			if (max_combo < combo)
				max_combo = combo;
		}
		else if (judge == 1)
		{
			combo++;
			score += 100;
			score += (2 * combo);
			notecount++;
			acc2 += 70.0;
			if (max_combo < combo)
				max_combo = combo;
		}
		else if (judge == 4)
		{
			combo = 0;
			notecount++;
			acc2 += 0.0;
		}





		acc = acc2/notecount;
		draw_score_and_combo();
		draw_judge(&judge);

		note_to_note(&num, n_of_file, file_note, note);
		draw_note(1, note);
		Sleep(offset);
		for (i = 0; i < 4; i++)
		{
			for (j = 23; j > 0; j--)
			{
				note[i][j] = note[i][j - 1];
				note[i][j - 1] = 0;
			}


		}
		note[i][20] = 0;
		delete_key_FX(&key);
		erease_judge();
	
	}
	draw_result();
}
void draw_key_effect(char *key)
{
	if (*key == g_key[0])
	{
		gotoxy(12, 22);
		printf("■■■");
	}
	else if (*key == g_key[1])
	{
		gotoxy(20, 22);
		printf("■■■");
	}
	else if (*key == g_key[2])
	{
		gotoxy(28, 22);
		printf("■■■");
	}
	else if (*key == g_key[3])
	{
		gotoxy(36, 22);
		printf("■■■");
	}
}
void delete_key_FX(char *key)
{
	if (*key == g_key[0])
	{
		gotoxy(12, 22);
		printf("      ");
	}
	else if (*key == g_key[1])
	{
		gotoxy(20, 22);
		printf("      ");
	}
	else if (*key == g_key[2])
	{
		gotoxy(28, 22);
		printf("      ");
	}
	else if (*key == g_key[3])
	{
		gotoxy(36, 22);
		printf("      ");
	}
	*key = 0;
}
void draw_note(int set, int vac[4][24])
{
	int x = 12, y = 0;
	int i, j;
	int clean_count=0;
	gotoxy(x, y);
	for (i = 0; i < 21; i++)
	{
		
		for (j = 0; j < 4; j++)
		{
			
			if (i == 20 || i == 19 || i == 18||i==17)
			{
				gotoxy(x, i);
				printf("	  ");
			}
			gotoxy(x, y);
			if (vac[j][i] == 1)
			{
				switch (note_set)
				{
				case 1:
					printf("●●●");
					break;
				case 2:
					printf("■■■");
					break;
				case 3:
					printf("★★★");
					break;
				}
				if (y > 0)
				{
					gotoxy(x, y - 1);
					printf("	  ");
				}
			}
			
			x += 8;
		}
		x = 12;
		y += 1;
	}
}
void note_to_note(int *num, int notelen, char f_note[][5], int note[4][24])
{
	int j, c;
	for (j = 0; j < 4; j++)
	{
		if (f_note[(*num)][j] == '0')
			note[j][0] = 0;
		else if (f_note[(*num)][j] == '1')
			note[j][0] = 1;
	}
	*num += 1;
}
void draw_offset()
{
	int x=16, y=2;
	gotoxy(x, y);
	printf("%d", offset);
}
int judge_acc(char *key,int note[4][24])
{
	
	if (*key == g_key[0])
	{
		if (note[0][20] == 1 || note[0][21] == 1)
		{
			if (note[0][20] == 1)
			{
				note[0][20] = 0;
			}
			else if (note[0][21] == 1)
			{
				note[0][21] = 0;
			}
			return 2;
		}
		else if (note[0][19] == 1 || note[0][18] == 1)
		{
			if (note[0][19] == 1)
			{
				note[0][19] = 0;
			}
			else if (note[0][18] == 1)
			{
				note[0][18] = 0;
			}
			return 1;
		}
	}
	else if (*key == g_key[1])
	{
		if (note[1][20] == 1 || note[1][21] == 1)
		{
			if (note[1][20] == 1)
			{
				note[1][20] = 0;
			}
			else if (note[1][21] == 1)
			{
				note[1][21] = 0;
			}
			return 2;
		}
		else if (note[1][19] == 1 || note[1][18] == 1)
		{
			if (note[1][19] == 1)
			{
				note[1][19] = 0;
			}
			else if (note[1][18] == 1)
			{
				note[1][18] = 0;
			}
			return 1;
		}
	}
	else if (*key == g_key[2])
	{
		if (note[2][20] == 1 || note[2][21] == 1)
		{
			if (note[2][20] == 1)
			{
				note[2][20] = 0;
			}
			else if (note[2][21] == 1)
			{
				note[2][21] = 0;
			}
			return 2;
		}
		else if (note[2][19] == 1 || note[2][18] == 1)
		{
			if (note[2][19] == 1)
			{
				note[2][19] = 0;
			}
			else if (note[2][18] == 1)
			{
				note[2][18] = 0;
			}
			return 1;
		}
	}
	else if (*key == g_key[3])
	{
		if (note[3][20] == 1 || note[3][21] == 1)
		{
			if (note[3][20] == 1)
			{
				note[3][20] = 0;
			}
			else if (note[3][21] == 1)
			{
				note[3][21] = 0;
			}
			return 2;
		}
		else if (note[3][19] == 1 || note[3][18] == 1)
		{
			if (note[3][19] == 1)
			{
				note[3][19] = 0;
			}
			else if (note[3][18] == 1)
			{
				note[3][18] = 0;
			}
			return 1;
		}
	}
	
	return 0;

}
void draw_judge(int *jud)
{
	int x = 45, y = 15;
	gotoxy(x, y);
	if (*jud == 1)
		printf("●GOOD!●    ");
	else if (*jud == 2)
		printf("★PERFECT!!★");
	else if (*jud == 4)
		printf("miss...      ");
	else
		return;
	*jud = 0;
}
void erease_judge()
{
	int x = 45, y = 15;
	gotoxy(x, y);
	printf("              ");
}
void draw_score_and_combo()
{
	gotoxy(2, 10);
	printf("콤보");
	gotoxy(2, 11);
	printf("%3d", combo);
	gotoxy(2, 13);
	printf("점수");
	gotoxy(2, 14);
	printf("%d", score);
	gotoxy(2,17);
	printf("정확도");
	gotoxy(2, 18);
	printf("%.2lf", acc);
}
int stopmenu()
{
	char key;
	system("cls");
	for (;;)
	{
		gotoxy(0, 0);
		printf("정지메뉴");
		gotoxy(0, 1);
		printf("1.곡 선택 화면");
		gotoxy(0, 2);
		printf("2.재시작");
		gotoxy(0,3);
		printf("3.메인 메뉴로");
		gotoxy(0, 4);
		printf("이어하기 기능은 지원하지 않습니다.");
		gotoxy(20, 1);
		printf("싱크 : %d", offset);
		key=_getch();
		system("cls");
		if (key == '+')
			offset++;
		else if (key == '-')
			offset--;
		else if (key == '1')
			return 8;
		else if (key == '2')
			return 4;
		else if (key == '3')
			return 5;
	}
}
int draw_result()
{
	system("cls");

	printf("점수 : %d\n", score);
	printf("정확도 : %.2lf\n", acc);
	printf("콤보 : %d\n", max_combo);
	printf("아무 키나 누르면 선곡화면으로 이동");
	_getch();
	return 8;
}
void draw_setting()
{
	FMOD_SYSTEM      *sys;
	FMOD_SOUND       *sound1, *bgm;
	FMOD_CHANNEL     *channel = 0;
	FMOD_RESULT       result;

	unsigned int      version;

	result = FMOD_System_Create(&sys);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(sys, &version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
	}
	result = FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/SFX/menu_button_sound.mp3", FMOD_HARDWARE, 0, &sound1);
	ERRCHECK(result);
	result = FMOD_System_CreateSound(sys, "sound/bgm/setting_bgm.mp3", FMOD_LOOP_NORMAL, 0, &bgm);
	ERRCHECK(result);

	//***********************************************FMOD

	int select_x=2, select_y = 2;
	char key;

	result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, bgm, 0, &channel);
	ERRCHECK(result);

	system("cls");
	//10칸 기준
	//		0123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|MAX=80         
	printf("                      게임 설정 "); printf("\n");
	printf("  "); printf("\n");
	printf("       싱크(노트 속도) :         "); printf("\n");
	printf("  "); printf("\n");
	printf("       노트 모양 :       (1.● 2.■ 3.★)"); printf("\n");
	printf(" "); printf("\n");//***
	printf("       오토싱크(Beta)"); printf("\n");
	printf(" "); printf("\n");
	printf("       선택화면으로"); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");//***
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");
	printf("  "); printf("\n");//***
	printf("    "); printf("\n");
	printf("    "); printf("\n");//곡명 : 15칸
	printf("           "); printf("\n");//아티스트 : 11칸
	printf("    커서 이동 : ↑,↓  조절 : ←,→  선택 : space   "); printf("\n");//난이도 : 13칸
	printf("   "); printf("\n");//***
	//END OF DOS Y_AXIS

	for (;;)
	{
		gotoxy(select_x, select_y);
		printf("▶");
		gotoxy(25, 2);
		printf("%d", offset);
		gotoxy(20, 4);
		printf("%d", note_set);
		key = _getch();

		if (key == 72 || key == 80)
		{
			gotoxy(select_x, select_y);
			printf("  ");
			result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound1, 0, &channel);
			ERRCHECK(result);
			move_set_cursor(key, &select_y, 8);
		}

		if (select_y == 2)
		{
			if (key == 75)
				offset--;
			else if (key == 77)
				offset++;
		}
		else if (select_y == 4)
		{
			if (key == 75)
			{
				note_set--;
				if (note_set<1)
					note_set++;
			}
			else if (key == 77)
			{
				note_set++;
				if (note_set > 3)
					note_set--;
			}
		}
		else if (select_y == 6)
		{
			if (key == 32)
			{
				result = FMOD_Sound_Release(bgm);
				ERRCHECK(result);
				auto_sync();
				return;
			}
		}
		else if (select_y == 8)
		{
			if (key == 32)
			{
				result = FMOD_Sound_Release(bgm);
				ERRCHECK(result);
				return;
			}
		}
	}
}
void move_set_cursor(char key, int *y, int y_m)
{
	switch (key)
	{
	case 72:
		*y = *y - 2;
		if (*y<2) *y += 2;
		break;
	case 80:
		*y += 2;
		if (*y>y_m) *y -= 2;
		break;
	}
}
void auto_sync()
{
	int i, j, tic,tik;
	int x = 0, y = 0;
	double sy_time;
	clock_t start, finish;
	int note[4][11] = { 0, };
	int sy_avg_time = 0;
	char key;
	system("cls");
	printf("자동 싱크 맞추기(Test ver.)\n");
	printf("컴퓨터의 속도를 대략적으로 계산하여 싱크를 맞춥니다.\n");
	printf("계속 하려면 아무키나 누르세요\n");
	_getch();
	system("cls");


	for (i = 0; i < 10; i++)
	{
		gotoxy(x, y);
		for (j = 0; j < 4; j++)
		{
			printf("│      ");
		}
		printf("│");
		y += 1;
	}
	gotoxy(x, y);
	printf("├───┼───┼───┼───┤");
	y += 1;
	gotoxy(x, y);
	for (j = 0; j < 5; j++)
	{
		printf("│      ");
	}
	y += 1;
	gotoxy(x, y);
	printf("└───┴───┴───┴───┘");
	for (tic = 0; tic < 5; tic++)
	{
		for (j = 0; j < 6; j += 3)
		{

			for (i = 0; i < 4; i++)
			{

				note[i][j] = 1;
			}
		}

		start = clock();
		for (tik=0;tik<10;tik++)
		{
			draw_sync_check_note(note);
			Sleep(100);
			for (i = 0; i < 4; i++)
			{
				for (j = 10; j > 0; j--)
				{
					note[i][j] = note[i][j - 1];
					note[i][j - 1] = 0;
				}


			}
			note[i][10] = 0;
		}
		finish = clock();

		sy_time = (double)(finish - start) / CLOCKS_PER_SEC;
		sy_time = sy_time - 1;
		sy_time *= 100;
		sy_avg_time += (int)(sy_time);
	}
	sy_avg_time /= 10;

	

	gotoxy(2, 13);
	printf("감지된 싱크 : %d", 100-sy_avg_time);
	gotoxy(2, 14);
	printf("적용하시겠습니까? (y=예 n=아니요)");
	gotoxy(2, 15);
	printf("선택 후 바로 곡 선택화면으로 돌아갑니다.");
	for (;;)
	{
		key = _getch();
		if (key == 'y')
		{
			offset = 100;
			offset -= sy_avg_time;
			return;
		}
		else if (key == 'n')
		{
			return;
		}
		else
			continue;
	}

	

}
void draw_sync_check_note(int vac[][11])
{
	int x = 2, y = 0;
	int al, i, j;
	int clean_count = 0;
	int note_set = 1;
	gotoxy(x, y);
	for (al = 0; al < 2; al++)
	{
		y = 0;
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (i == 9)
				{
					gotoxy(x, y);
					printf("  ");
				}
				gotoxy(x, i);
				printf("      ");

				gotoxy(x, y);
				if (vac[j][i] == 1)
				{
					switch (note_set)
					{
					case 1:
						printf("●●●");
						break;
					}
					if (y > 0)
					{
						gotoxy(x, y - 1);
						printf("      ");
					}
				}

				x += 8;
			}
			x = 2;
			y += 1;
		}
	}
}