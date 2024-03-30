#include "CASINO_FUNCTION.h"

#define LIST_START 3
#define LIST_SPACE 2

unsigned char _currentgame;

void drawgamelist_(unsigned char _selectedgame) {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	changeconsolecolor_(black, brightwhite);
	clearall_();
	gotoXY_(1, LIST_START);
	unsigned char _final_count = 0;
	if (_debug_enable == FALSE) {
		_final_count = GAMECOUNT - 1;
	}
	else if (_debug_enable == TRUE) {
		_final_count = GAMECOUNT;
	}
	for (unsigned char _countgame = 0; _countgame < _final_count; _countgame++) {
		printf("%s", _gamename[_countgame]);
		gotoXY_(1, LIST_START + LIST_SPACE * (_countgame + 1));
	}
	gotoXY_(1, LIST_START + LIST_SPACE * _selectedgame);
	clearline_();
	changeconsolecolor_(brightwhite, black);
	printf("%s", _gamename[_selectedgame]);
	gotoXY_(1, LIST_START + LIST_SPACE * _selectedgame);
	changeconsolecolor_(black, brightwhite);
}
void movegamelst_(unsigned char _direction) {
	COORD _curpos = getcurrentcurpos_();
	switch (_direction) {
	case 72://up
	case 'w':
	case 'W':
		if (_curpos.Y == LIST_START) {
			break;
		}
		gotoXY_(1, LIST_START + _currentgame * LIST_SPACE);
		changeconsolecolor_(black, brightwhite);
		clearline_();
		printf("%s", _gamename[_currentgame]);
		_currentgame--;
		PlaySound(TEXT("movecur.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case 80://down
	case 's':
	case 'S':
		if (_curpos.Y == LIST_START + LIST_SPACE * (GAMECOUNT - 1) && _debug_enable == TRUE) {
			break;
		}
		else if (_curpos.Y == LIST_START + LIST_SPACE * (GAMECOUNT - 2) && _debug_enable == FALSE) {
			break;
		}
		gotoXY_(1, LIST_START + _currentgame * LIST_SPACE);
		changeconsolecolor_(black, brightwhite);
		clearline_();
		printf("%s", _gamename[_currentgame]);
		_currentgame++;
		PlaySound(TEXT("movecur.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	}
	gotoXY_(1, LIST_START + LIST_SPACE * _currentgame);
	changeconsolecolor_(brightwhite, black);
	printf("%s", _gamename[_currentgame]);
	gotoXY_(1, LIST_START + LIST_SPACE * _currentgame);
	changeconsolecolor_(black, brightwhite);
}
void isbankrupt_(void) {
	CONSOLE_SCREEN_BUFFER_INFO _current;
	if (_player_money <= 0) {
		changeconsolecolor_(red, brightwhite);
		for (unsigned char _eraserpos = 0; _eraserpos < 30; _eraserpos++) {
			gotoXY_(0, _eraserpos);
			COORD _eraser = getcurrentcurpos_();
			for (unsigned char _eraserpos = 0; _eraserpos < 120; _eraserpos++) {
				gotoXY_(_eraserpos, _eraser.Y);
				putchar(' ');
			}
			gotoXY_(_eraser.X, _eraser.Y);
		}
		gotoXY_(1, LIST_START);
		puts("GAME OVER");
		gotoXY_(1, LIST_START + LIST_SPACE);
		puts("you are bankrupt");
		gotoXY_(1, (LIST_START + LIST_SPACE) + 3);
		changeconsolecolor_(brightwhite, red);
		printf("reset data");
		gotoXY_(1, ((LIST_START + LIST_SPACE) + 3) + LIST_SPACE);
		changeconsolecolor_(red, brightwhite);
		printf("exit");
		gotoXY_(1, (LIST_START + LIST_SPACE) + 3);
		while (1) {
			char k_input = _getch();
			switch (k_input) {
			case '`':
			case '~':
				exit(1);
				break;
			case 'w':
			case 'W':
			case 72://alias up
				gotoXY_(1, ((LIST_START + LIST_SPACE) + 3) + LIST_SPACE);
				changeconsolecolor_(red, brightwhite);
				printf("exit");
				gotoXY_(1, (LIST_START + LIST_SPACE) + 3);
				changeconsolecolor_(brightwhite, red);
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_current);
				printf("reset data");
				break;
			case 80://alias down
			case 's':
			case 'S':
				gotoXY_(1, (LIST_START + LIST_SPACE) + 3);
				changeconsolecolor_(red, brightwhite);
				printf("reset data");
				gotoXY_(1, ((LIST_START + LIST_SPACE) + 3) + LIST_SPACE);
				changeconsolecolor_(brightwhite, red);
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_current);
				printf("exit");
				break;
			case ' ':
			case 13://alias enter
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_current);
				if (_current.dwCursorPosition.Y == (LIST_START + LIST_SPACE) + 3) {
					_player_money = 10000;
					savemoneytofile_(_player_money);
					return 1;
				}
				else if (_current.dwCursorPosition.Y == ((LIST_START + LIST_SPACE) + 3) + LIST_SPACE) {
					exit(1);
					return;
				}
				break;
			}
		}
	}
	else
		return 0;
}
void gamelauncher_(unsigned char _gamecode) {
	unsigned char _reopen = 0;
	PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (1) {
		clearall_();
		switch (_gamecode) {
		case BLACKJACK:
			_reopen = main_blackjack();
			break;
		case VENTTI:
			_reopen = main_ventti();
			break;
		case SLOTMACINE:
			_reopen = main_slotmachine();
			break;
		case DEBUG:
			_reopen = main_debug();
			break;
		}
		isbankrupt_();
		if (_reopen == 1) {
			continue;
		}
		else {
			break;
		}
	}
	isbankrupt_();
	drawgamelist_(_gamecode);
	gotoXY_(1, 0);
	printf("YOU HAVE %lld", _player_money);
}

void main(void) {
	_player_money = readmoneyfromfile_();
	isbankrupt_();
	SetConsoleTitleA("CASINO");
	drawgamelist_(_currentgame);
	COORD _posbackup = getcurrentcurpos_();
	gotoXY_(1, 0);
	printf("YOU HAVE %lld", _player_money);
	gotoXY_(_posbackup.X, _posbackup.Y);
	while (1) {
		SetConsoleTitleA("CASINO");
		char _k_input = _getch();
		if (__debug_code[_debug_code_cur] == _k_input) {
			_debug_code_cur++;
			if (_debug_code_cur == strnlen_s(__debug_code, 64)) {
				_debug_code_cur = 0;
				_debug_enable = TRUE;
				clearall_();
				gotoXY_(1, 0);
				printf("ENABLE DEBUG MODE");
				hold_();
				clearall_();
				drawgamelist_(_currentgame);
				COORD _posbackup = getcurrentcurpos_();
				gotoXY_(1, 0);
				printf("YOU HAVE %lld", _player_money);
				gotoXY_(_posbackup.X, _posbackup.Y);
				continue;
			}
		}
		else {
			_debug_code_cur = 0;
		}
		switch (_k_input) {
		case '`':
		case '~':
			exit(1);
			break;
		case 72://up
		case 80://down
			movegamelst_(_k_input);
			break;
		case 'w':
		case 'W':
			movegamelst_(72);
			break;
		case 's':
		case 'S':
			movegamelst_(80);
			break;
		case ' ':
		case 13://enter
			gamelauncher_(_currentgame);
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (_k_input == (DEBUG + 1) + '0' && _debug_enable == FALSE) {
				break;
			}
			_currentgame = _k_input - 49;
			gamelauncher_(_currentgame);
			break;
		}
	}
}