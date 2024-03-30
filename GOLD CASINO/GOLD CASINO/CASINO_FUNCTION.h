#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

enum color_kinds {
	black,
	blue,
	green,
	aqua,
	red,
	purple,
	yellow,
	white,
	gray,
	lightblue,
	lightgreen,
	lightaqua,
	lightred, 
	lightpurple,
	lightyellow,
	brightwhite,
};
enum game_list {
	BLACKJACK,
	VENTTI,
	SLOTMACINE,
	DEBUG,
	GAMECOUNT,
};



extern char _gamename[GAMECOUNT][50];
extern time_t _random_seed;
extern long long _player_money;
extern long long _previous_bet_money;
extern unsigned long long _string_buffer_malloc_size;
extern char* __string_buffer;
extern unsigned char _debug_enable;
extern unsigned char __debug_code[64];
extern unsigned char _debug_code_cur;

COORD getcurrentcurpos_(void);
void gotoXY_(short _x, short _y);
void moveXY_(short _x, short _y);
void changeconsolecolor_(unsigned char _backColor, unsigned char _textColor);
void clearline_(void);
void clearall_(void);
void hold_(void);
void printleft_(const char* __line);
void printcenter_(const char* __line);
void printright_(const char* __line);
void savemoneytofile_(long long _money_value);
long long readmoneyfromfile_(void);
void updaterandomseed_(void);
char* lltoa__(long long value, char* __str, int len);
long long atollbin_(const char* __str);
void removeright_(void);
long long setstake_(unsigned char _current_game);
char* stringautomatch__(char* __input, char* __list[], unsigned short _list_count);
unsigned char randompercent_(long double _percent);
char* getstringbetween__(char* __string, unsigned short _start, unsigned short _end);
char ishavestringinside_(char* __target, char* __filed);
void mallocstringbuffer_(unsigned long long _malloc_size);