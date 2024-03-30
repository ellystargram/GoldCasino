#include "CASINO_FUNCTION.h"

#define OPTIONLIST_START_VENTTI 5
#define OPTIONLIST_SPACE_VENTTI 3

enum ventticardtype {
	SPADE,
	CLOVER,
	HEART,
	DIAMOND,
};
enum ventticardvaluedefine {
	J = 10,
	Q = 11,
	K = 12,
};
enum venttiparticipant {
	PLAYER = 1,
	DEALER = 2,
};
enum venttioption {
	stand,
	hit,
};
enum venttiwinloss {
	playerwin,
	push,
	dealerwin,
};

unsigned short _ventticards[4][13] = { 0, };
long long _ventti_bet_money = 0;

unsigned char welcome_ventti_(void) {
	gotoXY_(0, 10);
	printcenter_("VENTTI");
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
void getcard_ventti_(unsigned char _person, unsigned char _count) {
	if (time(NULL) != _random_seed) {
		_random_seed = time(NULL);
	}
	srand(_random_seed);
	unsigned char _picked = 0;
	while (_picked < _count) {
		unsigned char _howmanycardhave = 0;
		for (unsigned char _type = 0; _type < 4; _type++) {
			for (unsigned char _value = 0; _value < 13; _value++) {
				if (_ventticards[_type][_value] / 100 == _person) {
					_howmanycardhave++;
				}
			}
		}
		unsigned char _cardtype = rand() % 4;
		unsigned char _cardvalue = rand() % 13;
		if (_ventticards[_cardtype][_cardvalue] == 0) {
			_ventticards[_cardtype][_cardvalue] = _person * 100 + _howmanycardhave;
			_picked++;
		}
	}
}
unsigned char getsumvalue_ventti_(unsigned char _person) {
	unsigned char _sum = 0;
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_ventticards[_type][_value] / 100 == _person) {
				unsigned char _scorepercard = _value;
				_scorepercard++;
				_sum += _scorepercard;
			}
		}
	}
	return _sum;
}
unsigned char printsumvalue_ventti_(unsigned char _person) {
	unsigned char _sum = 0;
	_sum = getsumvalue_ventti_(_person);
	switch (_person) {
	case PLAYER:
		gotoXY_(1, 17);
		puts("PLAYER");
		gotoXY_(1, 18);
		break;
	case DEALER:
		gotoXY_(1, 5);
		puts("DEALER");
		gotoXY_(1, 6);
		break;
	}
	COORD _posbackup = getcurrentcurpos_();
	for (unsigned char eraser = 0; eraser < 3; eraser++) {
		putchar(' ');
	}
	gotoXY_(_posbackup.X, _posbackup.Y);
	if (_sum > 21) {
		printf("burst");
		return 1;
	}
	else {
		printf("%hhu", _sum);
		return 0;
	}
}
void drawcard_ventti_(unsigned char _person, unsigned char _priority, unsigned char _visible) {
	unsigned char _ventticardstype[4][8] = { "SPADE","CLOVER","HEART","DIAMOND" };
	unsigned char _ventticardsvalue[13][3] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
	unsigned char _print_type;
	unsigned char _print_value = 100;
	unsigned char _reference_x = 10 + _priority * 5;
	unsigned char _reference_y = 27 - _person * 13 + (_priority % 3) * 2;
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_ventticards[_type][_value] % 100 == _priority && _ventticards[_type][_value] / 100 == _person) {
				_print_type = _type;
				_print_value = _value;
				_value = 13;
				_type = 4;
			}
		}
	}
	if (_print_value == 100) {
		return;
	}
	changeconsolecolor_(black, brightwhite);
	gotoXY_(_reference_x, _reference_y);
	changeconsolecolor_(brightwhite, black);
	unsigned char _drawline;
	for (_drawline = 0; _drawline < 11; _drawline++) {
		putchar(' ');
	}
	for (_drawline = 0; _drawline < 6; _drawline++) {
		gotoXY_(_reference_x, _reference_y + _drawline + 1);
		putchar(' ');
		gotoXY_(_reference_x + 10, _reference_y + _drawline + 1);
		putchar(' ');
	}
	gotoXY_(_reference_x, _reference_y + 7);
	for (_drawline = 0; _drawline < 11; _drawline++) {
		putchar(' ');
	}
	changeconsolecolor_(black, brightwhite);
	for (unsigned char _painty = 0; _painty < 6; _painty++) {
		for (unsigned char _paintx = 0; _paintx < 9; _paintx++) {
			gotoXY_(_reference_x + 1 + _paintx, _reference_y + 1 + _painty);
			putchar(' ');
		}
	}
	switch (_visible) {
	case 0:
		for (unsigned char _painty = 0; _painty < 6; _painty++) {
			for (unsigned char _paintx = 0; _paintx < 9; _paintx++) {
				gotoXY_(_reference_x + 1 + _paintx, _reference_y + 1 + _painty);
				changeconsolecolor_((_painty + _paintx) % 16, brightwhite);
				putchar(' ');
			}
		}
		changeconsolecolor_(black, brightwhite);
		break;
	default:
		if (_print_type == HEART || _print_type == DIAMOND) {
			changeconsolecolor_(black, red);
		}
		else {
			changeconsolecolor_(black, brightwhite);
		}
		gotoXY_(_reference_x + 2, _reference_y + 2);
		printf("%s", _ventticardstype[_print_type]);
		gotoXY_(_reference_x + 2, _reference_y + 3);
		printf("%s", _ventticardsvalue[_print_value]);
		changeconsolecolor_(black, brightwhite);
		break;
	}
	printsumvalue_ventti_(_person);
}
void drawoptionlist_ventti_(unsigned char _selected) {
	unsigned char _venttioptionlist[2][6] = { "stand","hit" };
	gotoXY_(0, OPTIONLIST_START_VENTTI);
	unsigned char _venttioption_print_target = 2;
	for (unsigned char _option_printed = 0; _option_printed < _venttioption_print_target; _option_printed++) {
		printright_(_venttioptionlist[_option_printed]);
		moveXY_(-20, OPTIONLIST_SPACE_VENTTI);
	}
	gotoXY_(0, OPTIONLIST_START_VENTTI + OPTIONLIST_SPACE_VENTTI * _selected);
	changeconsolecolor_(brightwhite, black);
	printright_(_venttioptionlist[_selected]);
	changeconsolecolor_(black, brightwhite);
}
unsigned char moveoptionlist_ventti_(unsigned char _k_input, unsigned char _original_venttioption) {
	unsigned char _venttioptionlist[2][6] = { "stand","hit" };
	gotoXY_(0, OPTIONLIST_START_VENTTI + OPTIONLIST_SPACE_VENTTI * _original_venttioption);
	changeconsolecolor_(black, brightwhite);
	unsigned char _venttioption_end = 1;
	switch (_k_input) {
	case 72://up
		if (_original_venttioption == 0) {
			break;
		}
		printright_(_venttioptionlist[_original_venttioption]);
		_original_venttioption--;
		PlaySound(TEXT("movecur.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case 80://down
		if (_original_venttioption == _venttioption_end) {
			break;
		}
		printright_(_venttioptionlist[_original_venttioption]);
		_original_venttioption++;
		PlaySound(TEXT("movecur.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	}
	gotoXY_(0, OPTIONLIST_START_VENTTI + OPTIONLIST_SPACE_VENTTI * _original_venttioption);
	changeconsolecolor_(brightwhite, black);
	printright_(_venttioptionlist[_original_venttioption]);
	return _original_venttioption;
}
unsigned char selectcurrentoption_ventti_(unsigned char* __original_venttioption_pointer, unsigned char _person) {
	switch (*__original_venttioption_pointer) {
	case stand:
		return 1;
		break;
	case hit:
		getcard_ventti_(_person, 1);
		for (unsigned char _sortcard = 0; _sortcard < 52; _sortcard++) {
			drawcard_ventti_(_person, _sortcard, 1);
		}
		PlaySound(TEXT("getcard.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if (printsumvalue_ventti_(_person) == 1) {
			return 1;
		}
		break;
	}
	return 0;
}
unsigned char dealerai_ventti_(void) {
	unsigned char _venttioptionlist[4][12] = { "stand","hit","double down" };
	gotoXY_(0, OPTIONLIST_START_VENTTI);
	changeconsolecolor_(black, black);
	for (unsigned char _option_printed = 0; _option_printed < 3; _option_printed++) {
		printright_(_venttioptionlist[_option_printed]);
		moveXY_(-20, OPTIONLIST_SPACE_VENTTI);
	}
	changeconsolecolor_(black, brightwhite);
	for (unsigned char _sortcard = 0; _sortcard < 2; _sortcard++) {
		drawcard_ventti_(DEALER, _sortcard, 1);
	}
	while (getsumvalue_ventti_(PLAYER) >= getsumvalue_ventti_(DEALER) && getsumvalue_ventti_(DEALER) <= 16 && getsumvalue_ventti_(PLAYER) <= 21) {
		Sleep(2000);
		getcard_ventti_(DEALER, 1);
		for (unsigned char _sortcard = 0; _sortcard < 52; _sortcard++) {
			drawcard_ventti_(DEALER, _sortcard, 1);
		}
		PlaySound(TEXT("getcard.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	return;
}

unsigned char main_ventti(void) {
	SetConsoleTitleA("VENTTI");
	if (welcome_ventti_() == 1) {
		return 0;
	}
	_ventti_bet_money = setstake_(VENTTI);
	if (_ventti_bet_money == -1) {
		return 0;
	}
	unsigned char _venttioption = 0;
	clearall_();
	gotoXY_(0, 0);
	printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			_ventticards[_type][_value] = 0;
		}
	}
	//_ventticards[HEART][0] = 100;
	//_ventticards[DIAMOND][0] = 101;
	for (unsigned char _order = 0; _order < 2; _order++) {
		getcard_ventti_(PLAYER, 1);
		drawcard_ventti_(PLAYER, _order, 1);
	}
	for (unsigned char _order = 0; _order < 2; _order++) {
		getcard_ventti_(DEALER, 1);
		drawcard_ventti_(DEALER, _order, (_order + 1) % 2);
		gotoXY_(1, 6);
		for (unsigned char eraser = 0; eraser < 9; eraser++) {
			putchar(' ');
		}
	}
	moveXY_(0, -3);
	drawoptionlist_ventti_(_venttioption);
	gotoXY_(0, 1);
	changeconsolecolor_(black, brightwhite);
	printcenter_("stake: ");
	printf("%lld", _ventti_bet_money);
	while (1 && getsumvalue_ventti_(PLAYER) <= 21) {
		unsigned char _playerturnoff_ventti = 0;
		unsigned char _k_input = _getch();
		switch (_k_input) {
		case '`':
		case '~':
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			exit(1);
			break;
		case 27://escape
			return 0;
			break;
		case 8://backspace
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			return 1;
			break;
		case 72://up
		case 80://down
			_venttioption = moveoptionlist_ventti_(_k_input, _venttioption);
			break;
		case 'w':
		case 'W':
			_venttioption = moveoptionlist_ventti_(72, _venttioption);
			break;
		case 's':
		case 'S':
			_venttioption = moveoptionlist_ventti_(80, _venttioption);
			break;
		case ' ':
		case 13://enter
			_playerturnoff_ventti = selectcurrentoption_ventti_(&_venttioption, PLAYER);
			break;
		}

		if (_playerturnoff_ventti == 1) {
			break;
		}
	}
	//player turn off
	//dealer turn start
	dealerai_ventti_();
	gotoXY_(0, 1);
	unsigned char _person = PLAYER;
	struct _ventti_return {
		long long _player;
		long long _player_net;
	};
	struct _ventti_return _ventti_refund = { 0, };
	unsigned char _result = 0;
	if (getsumvalue_ventti_(PLAYER) == getsumvalue_ventti_(DEALER)) {
		_ventti_refund._player = _ventti_bet_money;
		_result = push;
	}
	else if ((getsumvalue_ventti_(PLAYER) > getsumvalue_ventti_(DEALER) && getsumvalue_ventti_(PLAYER) <= 21) || getsumvalue_ventti_(DEALER) > 21) {
		_ventti_refund._player = _ventti_bet_money * 2;
		if (getsumvalue_ventti_(PLAYER) == 21) {
			_ventti_refund._player = _ventti_refund._player * 1.5;
		}
		_result = playerwin;
	}
	else if ((getsumvalue_ventti_(PLAYER) < getsumvalue_ventti_(DEALER) && getsumvalue_ventti_(DEALER) <= 21) || getsumvalue_ventti_(PLAYER) > 21) {
		_ventti_refund._player = 0;
		_result = dealerwin;
	}
	_ventti_refund._player_net = _ventti_refund._player - _ventti_bet_money;
	_player_money = _ventti_refund._player + _player_money;
	savemoneytofile_(_player_money);
	gotoXY_(0, 1);
	clearline_();
	for (unsigned char _order = 0; _order < 52; _order++) {
		drawcard_ventti_(DEALER, _order, 1);
	}
	printsumvalue_ventti_(DEALER);
	for (unsigned char _order = 0; _order < 52; _order++) {
		drawcard_ventti_(PLAYER, _order, 1);
	}
	printsumvalue_ventti_(PLAYER);
	gotoXY_(0, 1);
	switch (_result) {
	case push:
		printcenter_("PUSH");
		break;
	case dealerwin:
		printcenter_("DEALER WIN");
		break;
	case playerwin:
		printcenter_("PLAYER WIN");
		break;
	}
	moveXY_(0, 1);
	printcenter_("you get: ");
	printf("%lld", _ventti_refund._player);
	moveXY_(0, 1);
	printcenter_("NET profit: ");
	printf("%lld", _ventti_refund._player_net);
	while (_person == PLAYER) {
		char _k_input = _getch();
		switch (_k_input) {
		case '`':
		case '~':
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			exit(1);
			break;
		case 27://escape
			return 0;
			break;
		case 8://backspace
			PlaySound(TEXT("select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			return 1;
			break;
		}
	}
}