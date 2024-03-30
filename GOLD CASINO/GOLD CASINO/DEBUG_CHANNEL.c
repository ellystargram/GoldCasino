#include "CASINO_FUNCTION.h"

#define DEBUGLIST_START 5
#define DEBUGLIST_SPACE 2

enum debuglist {
	getascii,
	stringautomatch__functiontest,
	drawslot,
	editmoney,
	getstring,
	hasin,
	countdebuglist,
};
void drawdebuglist_(unsigned char _debugsellect, char* __debug_list[]);
void getasciifunction_(void);
void stringautomatch__function_(void);
void drawslotfunction_(void);
void editmoneyfunction_(void);
void getstrbtw_(void);
void hasinside_(void);

unsigned char main_debug(void) {
	SetConsoleTitleA("DEBUG AREA - YOU CAN'T ENTER THIS PLACE ON RELEASE. YOU THIEF!");
	char* __debug_list[] = { "GETASCII","stringautomatch__FUNCTIONTEST","DRAW SLOT","EDIT MONEY","GETSTRING","HAVE INSIDE" };
	unsigned char _debug_option = 0;
	while (1) {
		drawdebuglist_(_debug_option, __debug_list);
		char _k_input = _getch();
		switch (_k_input) {
		case '`':
		case '~':
			exit(1);
			break;
		case 27://escape
			return 0;
			break;
		case 72://up
		case 'w':
		case 'W':
			if (_debug_option != 0) {
				_debug_option--;
			}
			break;
		case 80://down
		case 's':
		case 'S':
			if (_debug_option != countdebuglist - 1) {
				_debug_option++;
			}
			break;
		case ' ':
		case 13://enter
			switch (_debug_option) {
			case getascii:
				getasciifunction_();
				break;
			case stringautomatch__functiontest:
				stringautomatch__function_();
				break;
			case drawslot:
				drawslotfunction_();
				break;
			case editmoney:
				editmoneyfunction_();
				break;
			case getstring:
				getstrbtw_();
				break;
			case hasin:
				hasinside_();
				break;
			}
		}
	}
}
void drawdebuglist_(unsigned char _debugsellect, char* __debug_list[]) {
	changeconsolecolor_(black, brightwhite);
	gotoXY_(1, 0);
	printf("welcome to debug menu!");
	gotoXY_(1, DEBUGLIST_START);
	for (unsigned char _debuglist_print = 0; _debuglist_print < countdebuglist; _debuglist_print++) {
		gotoXY_(1, DEBUGLIST_START + (DEBUGLIST_SPACE * _debuglist_print));
		if (_debuglist_print == _debugsellect) {
			changeconsolecolor_(brightwhite, black);
		}
		printf(__debug_list[_debuglist_print]);
		changeconsolecolor_(black, brightwhite);
	}
}
void getasciifunction_(void) {
	clearall_();
	gotoXY_(0, 0);
	printcenter_("press any key which you check");
	gotoXY_(60, 5);
	unsigned char _k_input = _getche();
	gotoXY_(55, 8);
	printf("%hhu ", _k_input);
	if (_k_input == 224) {
		printf("%hhu", _getch());
	}
	gotoXY_(60, 11);
	hold_();
	clearall_();
}
void stringautomatch__function_(void) {
	clearall_();
	gotoXY_(1, 0);
	printf("enter search sentence: ");
	char _input[64];
	gets(_input);
	char* _list[] = { "rere", "re", "cherry" };
	gotoXY_(0, 10);
	printcenter_(stringautomatch__(_input, _list, 3));
	gotoXY_(60, 11);
	hold_();
	clearall_();
}
void drawslotfunction_(void) {
	clearall_();
	gotoXY_(1, 0);
	printf("slotmachine debug");
	drawslot_slotmachine_();
	changeconsolecolor_(black, brightwhite);
	gotoXY_(0, 30);
	hold_();
	clearall_();
}
void editmoneyfunction_(void) {
	clearall_();
	gotoXY_(1, 0);
	printf("edit money");
	printcenter_("current money: ");
	printf("%lld", _player_money);
	gotoXY_(1, 1);
	long long _changemoney;
	scanf_s("%lld", &_changemoney);
	_player_money = _changemoney;
	savemoneytofile_(_changemoney);
	clearall_();
}
void getstrbtw_(void) {
	clearall_();
	gotoXY_(1, 0);
	printf("original str is hello");
	char* original = "hello";
	gotoXY_(1, 1);
	printf("(%s)", getstringbetween__(original, 1, 3));
	gotoXY_(1, 2);
	for (unsigned char _count = 0; _count < 5; _count++) {
		printf("%hhu\n", _count + 1);
		Sleep(1000);
	}
	/*
	for (unsigned long long int _ramcheck = 1; _ramcheck < ULLONG_MAX; _ramcheck++) {
		mallocstringbuffer_(_ramcheck);
		printf("%llu\n", _ramcheck);
	}
	*/
	/*while (1) {
		malloc(sizeof(unsigned long long int));
	}*/
	gotoXY_(1, 20);
	hold_();
	clearall_();
}
void hasinside_(void) {
	clearall_();
	gotoXY_(1, 0);
	printf("apple has pp");
	char* original = "pp";
	gotoXY_(1, 1);
	printf("%hhd", ishavestringinside_(original,"apple"));
	gotoXY_(1, 20);
	hold_();
	clearall_();
}