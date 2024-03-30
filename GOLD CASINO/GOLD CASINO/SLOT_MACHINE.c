#include "CASINO_FUNCTION.h"

unsigned char welcome_slotmachine_(void);
void drawslot_slotmachine_(void);
void display_slotmachine_(unsigned char* __display_array);
unsigned char* rollslot__(unsigned char* __slot);
long long calculatereward_(unsigned char* __slot, long long _bet_money);

unsigned char main_slotmachine(void) {
	SetConsoleTitleA(_gamename[SLOTMACINE]);
	if (welcome_slotmachine_() == 1) {
		return 0;
	}
	clearall_();
	unsigned char _slot[3] = { 0,0,0 };
	long long _slotmachine_bet_money = setstake_(SLOTMACINE);
	clearall_();
	gotoXY_(0, 0);
	printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
	drawslot_slotmachine_();
	srand(time(NULL));
	unsigned char _while_leave_flag = 0;
	while (_while_leave_flag==0) {
		char _k_input = _getch();
		switch (_k_input) {
		case '`':
		case '~':
			exit(1);
			break;
		case 27://escape
			return 0;
			break;
		case 8://backspace
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			return 1;
			break;
		case ' ':
		case 13://enter
			while (1) {
				for (unsigned char _adjust = 0; _adjust < 3; _adjust++) {
					_slot[_adjust] = (_slot[_adjust] + 1) % 12;
				}
				display_slotmachine_(_slot);
				if (_kbhit() == 1) {
					break;
				}
			}
			memcpy_s(_slot, 2, rollslot__(_slot), 2);
			_while_leave_flag = 1;
			display_slotmachine_(_slot);
			break;
		}
	}
	while (_kbhit()) {
		char _junk = _getch();
	}
	changeconsolecolor_(black, brightwhite);
	long long _return_money = calculatereward_(_slot, _slotmachine_bet_money);
	gotoXY_(0, 28);
	printcenter_("reward: ");
	printf("%lld", _return_money - _slotmachine_bet_money);
	_player_money = _player_money + _return_money;
	savemoneytofile_(_player_money);
	gotoXY_(40, 29);
	hold_();
}
unsigned char welcome_slotmachine_(void) {
	gotoXY_(0, 10);
	printcenter_(_gamename[SLOTMACINE]);
	gotoXY_(0, 13);
	printcenter_("press any key to enter bet money");
	moveXY_(0, 1);
	printcenter_("press `, escape to exit app");
	moveXY_(0, 1);
	printcenter_("press backspace to back to menu");
	while (1) {
		switch (_getch()) {
		case '`':
		case '~':
		case 27://escape
			exit(1);
			break;
		case 8://backspace
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			return 1;
			break;
		default:
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			return 0;
			break;
		}
	}
}
void drawslot_slotmachine_(void) {
	changeconsolecolor_(black, brightwhite);
	gotoXY_(45, 10);
	changeconsolecolor_(brightwhite, black);
	for (unsigned char _drawx = 0; _drawx < 31; _drawx++) {
		putchar(' ');
	}
	for (unsigned char _drawy = 0; _drawy < 7; _drawy++) {
		gotoXY_(45, 11 + _drawy);
		for (unsigned char _drawx = 0; _drawx < 31; _drawx++) {
			if (_drawx % 10 == 0) {
				changeconsolecolor_(brightwhite, black);
			}
			else {
				changeconsolecolor_(black, brightwhite);
			}
			putchar(' ');
		}
	}
	changeconsolecolor_(black, brightwhite);
	gotoXY_(45, 18);
	changeconsolecolor_(brightwhite, black);
	for (unsigned char _drawx = 0; _drawx < 31; _drawx++) {
		putchar(' ');
	}
}
void display_slotmachine_(unsigned char* __display_array) {
	unsigned char _display[12][5][5] = { 
		{{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1}},//0
		{{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1}},//1
		{{1,1,1,1,1},{0,0,0,0,1},{1,1,1,1,1},{1,0,0,0,0},{1,1,1,1,1}},//2
		{{1,1,1,1,1},{0,0,0,0,1},{1,1,1,1,1},{0,0,0,0,1},{1,1,1,1,1}},//3
		{{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1}},//4
		{{1,1,1,1,1},{1,0,0,0,0},{1,1,1,1,1},{0,0,0,0,1},{1,1,1,1,1}},//5
		{{1,1,1,1,1},{1,0,0,0,0},{1,1,1,1,1},{1,0,0,0,1},{1,1,1,1,1}},//6
		{{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1}},//7
		{{1,1,1,1,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,0,0,1},{1,1,1,1,1}},//8
		{{1,1,1,1,1},{1,0,0,0,1},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1}},//9
		{{0,0,0,0,0},{0,1,0,1,0},{1,1,1,1,1},{0,1,1,1,0},{0,0,1,0,0}},//heart
		{{0,0,1,0,0},{0,1,1,1,0},{1,1,1,1,1},{0,1,1,1,0},{0,0,1,0,0}}//diamond
	};
	for (unsigned char _z = 0; _z < 3; _z++) {
		unsigned char _ref_x = 48 + (_z * 10);
		unsigned char _ref_y = 12;
		for (unsigned char _y = 0; _y < 5; _y++) {
			gotoXY_(_ref_x, _ref_y + _y);
			for (unsigned char _x = 0; _x < 5; _x++) {
				if (_display[__display_array[_z]][_y][_x] == 1) {
					changeconsolecolor_(brightwhite, black);
				}
				else {
					changeconsolecolor_(black, brightwhite);
				}
				putchar(' ');
			}
		}
	}
}
unsigned char* rollslot__(unsigned char* __slot) {
	unsigned char _slot_rotation_speed[3] = { 255,255,255 };
	while (_slot_rotation_speed[0] + _slot_rotation_speed[1] + _slot_rotation_speed[2] != 0) {
		for (unsigned char _adjust = 0; _adjust < 3; _adjust++) {
			if (randompercent_(1-(_adjust*0.1)) == 1 && _slot_rotation_speed[_adjust] != 0) {
				_slot_rotation_speed[_adjust]--;
			}
		}
		for (unsigned char _adjust = 0; _adjust < 3; _adjust++) {
			if (randompercent_((long double)_slot_rotation_speed[_adjust]/255)) {
				__slot[_adjust] = (__slot[_adjust] + 1) % 12;
			}
		}
		display_slotmachine_(__slot);
	}
	return __slot;
}
long long calculatereward_(unsigned char* __slot, long long _bet_money) {
	unsigned short _paturn[][4] = {
		{7,7,7,777},//777!
		{10,10,10,50},//heart
		{11,11,11,100},//diamond
		{0,0,0,10}//000
	};
	unsigned char _conti = 0;
	unsigned char _count = 0;
	unsigned char _pat = 0;
	double _times = 0;
	for (_pat = 0; _pat < 4; _pat++) {//change when you edit answer sheet
		_conti = 0;
		_count = 0;
		for (unsigned char _st = 0; _st < 3; _st++) {
			if (_paturn[_pat][_st] == __slot[_st]) {
				if (_st != 2 && __slot[_st] == __slot[_st + 1]) {
					_conti++;
				}
				_count++;
			}

		}
		if (_count == 3 && _conti == 2) {
			_times = 1.0;
			break;
		}
		else if (_count == 2 && _conti == 1) {
			_times = 0.5;
			break;
		}
		else if (_count == 2 && _conti == 0) {
			_times = 0.25;
			break;
		}
		else {
			_times = 0.0;
		}
	}
	return (long long)(_times * _paturn[_pat][3] * _bet_money);
}