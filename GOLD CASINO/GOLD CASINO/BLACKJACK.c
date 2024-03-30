#include "CASINO_FUNCTION.h"

#define OPTIONLIST_START_BLACKJACK 5
#define OPTIONLIST_SPACE_BLACKJACK 3

enum blackjackcardtype {
	SPADE,
	CLOVER,
	HEART,
	DIAMOND,
};
enum blackjackcardvaluedefine {
	A = 0,
	J = 10,
	Q = 11,
	K = 12,
};
enum blackjackparticipant {
	PLAYER = 1,
	DEALER = 2,
	SPLIT = 3,
};
enum blackjackoption {
	stand,
	hit,
	doubledown,
	split,
};
enum blackjackwinloss {
	playerwin,
	push,
	dealerwin,
	insureance,
};

struct _split_option {
	unsigned char _issplitavail;
	unsigned char _issplited;
};
struct _insureance_option {
	unsigned char _active;
	unsigned char _is_insureance_posible;
	long long _money;
	long long _money_limit;
};

struct _split_option _split = { 0, };
struct _insureance_option _insureance = { 0, };
unsigned short _blackjackcards[4][13] = { 0, };
long long _blackjack_bet_money = 0;
long long _blackjack_bet_money_split = 0;


unsigned char welcome_blackjack_(void) {
	gotoXY_(0, 10);
	printcenter_("BLACKJACK");
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
void getcard_blackjack_(unsigned char _person, unsigned char _count) {
	/*if (time(NULL) != _random_seed) {
		_random_seed = time(NULL);
		srand(_random_seed);
	}*/
	unsigned char _picked = 0;
	while (_picked < _count) {
		unsigned char _howmanycardhave = 0;
		for (unsigned char _type = 0; _type < 4; _type++) {
			for (unsigned char _value = 0; _value < 13; _value++) {
				if (_blackjackcards[_type][_value] / 100 == _person) {
					_howmanycardhave++;
				}
			}
		}
		unsigned char _cardtype = rand() % 4;
		unsigned char _cardvalue = rand() % 13;
		if (_blackjackcards[_cardtype][_cardvalue] == 0) {
			_blackjackcards[_cardtype][_cardvalue] = _person * 100 + _howmanycardhave;
			_picked++;
		}
	}
}
unsigned char getsumvalue_blackjack_(unsigned char _person) {
	unsigned char _sum = 0;
	unsigned char _countace = 0;
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_blackjackcards[_type][_value] / 100 == _person) {
				unsigned char _scorepercard = _value;
				_scorepercard++;
				switch (_scorepercard) {
				case 11://J
				case 12://Q
				case 13://K
					_scorepercard = 10;
					break;
				case 1://Ace
					_countace++;
					_scorepercard = 0;
					break;
				}
				_sum += _scorepercard;
			}
		}
	}
	signed char _acesimulator;
	for (_acesimulator = 0; _acesimulator <= _countace; _acesimulator++) {
		if (_acesimulator * 11 + _sum > 21) {
			break;
		}
	}
	_acesimulator--;
	_acesimulator = abs(_acesimulator);
	_sum = _sum + _acesimulator * 11 + (_countace - _acesimulator);
	return _sum;
}
unsigned char countcard_(unsigned char _person) {
	unsigned char _count = 0;
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_blackjackcards[_type][_value] / 100 == _person) {
				_count++;
			}
		}
	}
	return _count;
}
unsigned char printsumvalue_blackjack_(unsigned char _person) {
	unsigned char _sum = 0;
	_sum = getsumvalue_blackjack_(_person);
	switch (_person) {
	case PLAYER:
		gotoXY_(1, 17);
		puts("PLAYER");
		gotoXY_(1, 18);
		break;
	case SPLIT:
		gotoXY_(1, 17);
		puts("SPLIT ");
		gotoXY_(1, 18);
		break;
	case DEALER:
		gotoXY_(1, 5);
		puts("DEALER");
		gotoXY_(1, 6);
		break;
	}
	COORD _posbackup = getcurrentcurpos_();
	for (unsigned char _eraser = 0; _eraser < 3; _eraser++) {
		putchar(' ');
	}
	gotoXY_(_posbackup.X, _posbackup.Y);
	if (_sum == 21 && countcard_(_person) == 2 && _split._issplited == 0) {
		printf("blackjack");
		return 0;
	}
	else if (_sum > 21) {
		printf("burst");
		return 1;
	}
	else {
		printf("%hhu", _sum);
		return 0;
	}
}
void drawcard_blackjack_(unsigned char _person, unsigned char _priority, unsigned char _visible) {
	unsigned char _blackjackcardstype[4][8] = { "SPADE","CLOVER","HEART","DIAMOND" };
	unsigned char _blackjackcardsvalue[13][3] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
	unsigned char _print_type;
	unsigned char _print_value = 100;
	unsigned char _reference_x = 10 + _priority * 5;
	unsigned char _reference_y = 27 - _person * 13 + (_priority % 3) * 2;
	if (_person == SPLIT) {
		_reference_y = 27 - 1 * 13 + (_priority % 3) * 2;
	}
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_blackjackcards[_type][_value] % 100 == _priority && _blackjackcards[_type][_value] / 100 == _person) {
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
		printf("%s", _blackjackcardstype[_print_type]);
		gotoXY_(_reference_x + 2, _reference_y + 3);
		printf("%s", _blackjackcardsvalue[_print_value]);
		changeconsolecolor_(black, brightwhite);
		break;
	}
	printsumvalue_blackjack_(_person);
}
void drawoptionlist_blackjack_(unsigned char _selected) {
	unsigned char _blackjackoptionlist[4][12] = { "stand","hit","double down","split" };
	gotoXY_(0, OPTIONLIST_START_BLACKJACK);
	unsigned char _blackjackoption_print_target = 0;
	if (_player_money < _blackjack_bet_money) {
		_blackjackoption_print_target = 2;
	}
	else if (_split._issplitavail == 1 && _split._issplited == 0) {//split avail
		_blackjackoption_print_target = 4;
	}
	else {//only double down
		_blackjackoption_print_target = 3;
	}
	for (unsigned char _option_printed = 0; _option_printed < _blackjackoption_print_target; _option_printed++) {
		printright_(_blackjackoptionlist[_option_printed]);
		moveXY_(-20, OPTIONLIST_SPACE_BLACKJACK);
	}
	gotoXY_(0, OPTIONLIST_START_BLACKJACK + OPTIONLIST_SPACE_BLACKJACK * _selected);
	changeconsolecolor_(brightwhite, black);
	printright_(_blackjackoptionlist[_selected]);
	changeconsolecolor_(black, brightwhite);
}
unsigned char moveoptionlist_blackjack_(unsigned char _k_input, unsigned char _original_blackjackoption) {
	unsigned char _blackjackoptionlist[4][12] = { "stand","hit","double down","split" };
	gotoXY_(0, OPTIONLIST_START_BLACKJACK + OPTIONLIST_SPACE_BLACKJACK * _original_blackjackoption);
	changeconsolecolor_(black, brightwhite);
	unsigned char _blackjackoption_end;
	if (_player_money < _blackjack_bet_money) {
		_blackjackoption_end = 1;
	}
	else if (_split._issplitavail == 1 && _split._issplited == 0) {//split avail
		_blackjackoption_end = 3;
	}
	else {//only double down
		_blackjackoption_end = 2;
	}
	switch (_k_input) {
	case 72://up
		if (_original_blackjackoption == 0) {
			break;
		}
		printright_(_blackjackoptionlist[_original_blackjackoption]);
		_original_blackjackoption--;
		PlaySound(TEXT("movecur.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case 80://down
		if (_original_blackjackoption == _blackjackoption_end) {
			break;
		}
		printright_(_blackjackoptionlist[_original_blackjackoption]);
		_original_blackjackoption++;
		PlaySound(TEXT("movecur.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	}
	gotoXY_(0, OPTIONLIST_START_BLACKJACK + OPTIONLIST_SPACE_BLACKJACK * _original_blackjackoption);
	changeconsolecolor_(brightwhite, black);
	printright_(_blackjackoptionlist[_original_blackjackoption]);
	return _original_blackjackoption;
}
unsigned char selectcurrentoption_blackjack_(unsigned char* __original_blackjackoption_pointer, unsigned char _person) {
	switch (*__original_blackjackoption_pointer) {
	case stand:
		return 1;
		break;
	case hit:
		getcard_blackjack_(_person, 1);
		for (unsigned char _sortcard = 0; _sortcard < 52; _sortcard++) {
			drawcard_blackjack_(_person, _sortcard, 1);
		}
		PlaySound(TEXT("getcard.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if (printsumvalue_blackjack_(_person) == 1) {
			return 1;
		}
		break;
	case doubledown:
		if (_player_money < _blackjack_bet_money) {
			break;
		}
		if (_player_money < _blackjack_bet_money_split) {
			break;
		}
		if (_person == PLAYER) {
			_player_money = _player_money - _blackjack_bet_money;
			_blackjack_bet_money = _blackjack_bet_money * 2;
			gotoXY_(0, 1);
			changeconsolecolor_(black, brightwhite);
			printcenter_("stake: ");
			printf("%lld", _blackjack_bet_money);
		}
		else if (_person == SPLIT) {
			_player_money = _player_money - _blackjack_bet_money_split;
			_blackjack_bet_money_split = _blackjack_bet_money_split * 2;
			gotoXY_(0, 1);
			changeconsolecolor_(black, brightwhite);
			printcenter_("stake: ");
			printf("%lld", _blackjack_bet_money_split);
		}
		getcard_blackjack_(_person, 1);
		for (unsigned char _sortcard = 0; _sortcard < 52; _sortcard++) {
			drawcard_blackjack_(_person, _sortcard, 1);
		}
		PlaySound(TEXT("getcard.wav"), NULL, SND_FILENAME | SND_ASYNC);
		//todo money*2 last
		return 1;
		break;
	case split:
		if (_player_money < _blackjack_bet_money) {
			break;
		}
		_player_money = _player_money - _blackjack_bet_money;
		_blackjack_bet_money_split = _blackjack_bet_money;
		gotoXY_(0, OPTIONLIST_START_BLACKJACK);
		unsigned char _blackjackoptionlist[4][12] = { "stand","hit","double down","split" };
		changeconsolecolor_(black, black);
		for (unsigned char _option_printed = 0; _option_printed < 4; _option_printed++) {
			printright_(_blackjackoptionlist[_option_printed]);
			moveXY_(-20, OPTIONLIST_SPACE_BLACKJACK);
		}
		changeconsolecolor_(black, brightwhite);
		_split._issplited = 1;
		*__original_blackjackoption_pointer = 0;
		drawoptionlist_blackjack_(*__original_blackjackoption_pointer);
		for (unsigned char _type = 0; _type < 4; _type++) {
			for (unsigned char _value = 0; _value < 13; _value++) {
				if (_blackjackcards[_type][_value] == 101) {
					_blackjackcards[_type][_value] = 300;
				}
			}
		}
		getcard_blackjack_(PLAYER, 1);
		getcard_blackjack_(SPLIT, 1);
		for (unsigned char _order = 0; _order < 52; _order++) {
			drawcard_blackjack_(PLAYER, _order, 1);
		}
		printsumvalue_blackjack_(PLAYER);
		break;
	}
	return 0;
}
unsigned char dealerai_blackjack_(void) {
	unsigned char _blackjackoptionlist[4][12] = { "stand","hit","double down" };
	gotoXY_(0, OPTIONLIST_START_BLACKJACK);
	changeconsolecolor_(black, black);
	for (unsigned char _option_printed = 0; _option_printed < 3; _option_printed++) {
		printright_(_blackjackoptionlist[_option_printed]);
		moveXY_(-20, OPTIONLIST_SPACE_BLACKJACK);
	}
	changeconsolecolor_(black, brightwhite);
	for (unsigned char _sortcard = 0; _sortcard < 2; _sortcard++) {
		drawcard_blackjack_(DEALER, _sortcard, 1);
	}
	unsigned long long int _time_before = time(NULL);
	while (getsumvalue_blackjack_(PLAYER) >= getsumvalue_blackjack_(DEALER) && getsumvalue_blackjack_(DEALER) <= 16 && getsumvalue_blackjack_(PLAYER) <= 21) {
		while (time(NULL) - _time_before < 2) {
			if (_kbhit() == TRUE) {
				unsigned char _k_input = _getch();
				if (_k_input == 27) {
					exit(1);
				}
			}
		}
		_time_before = time(NULL);
		getcard_blackjack_(DEALER, 1);
		for (unsigned char _sortcard = 0; _sortcard < 52; _sortcard++) {
			drawcard_blackjack_(DEALER, _sortcard, 1);
		}
		PlaySound(TEXT("getcard.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	if (_split._issplited == 1) {
		while (getsumvalue_blackjack_(SPLIT) >= getsumvalue_blackjack_(DEALER) && getsumvalue_blackjack_(DEALER) <= 16 && getsumvalue_blackjack_(SPLIT) <= 21) {
			Sleep(2000);
			getcard_blackjack_(DEALER, 1);
			for (unsigned char _sortcard = 0; _sortcard < 52; _sortcard++) {
				drawcard_blackjack_(DEALER, _sortcard, 1);
			}
			PlaySound(TEXT("getcard.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	while (_kbhit() == TRUE) {
		unsigned char _junk = _getch();
	}
	return;
}

unsigned char main_blackjack(void) {
	_split._issplitavail = 0;
	_split._issplited = 0;
	_insureance._active = 0;
	_insureance._is_insureance_posible = 0;
	_insureance._money = 0;
	_insureance._money_limit = 0;
	SetConsoleTitleA("BLACKJACK");
	if (welcome_blackjack_() == 1) {
		return 0;
	}
	_blackjack_bet_money = 0;
	_blackjack_bet_money_split = 0;
	unsigned char _blackjackoption = 0;
	_blackjack_bet_money = setstake_(BLACKJACK);
	if (_blackjack_bet_money == -1) {
		return 0;
	}
	clearall_();
	gotoXY_(0, 0);
	printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			_blackjackcards[_type][_value] = 0;
		}
	}
	//_blackjackcards[HEART][0] = 100;
	//_blackjackcards[DIAMOND][0] = 101;
	for (unsigned char _order = 0; _order < 2; _order++) {
		getcard_blackjack_(PLAYER, 1);
		drawcard_blackjack_(PLAYER, _order, 1);
	}
	//_blackjackcards[SPADE][0] = 200;
	//getcard_blackjack_(DEALER, 1);
	for (unsigned char _order = 0; _order < 2; _order++) {
		getcard_blackjack_(DEALER, 1);
		drawcard_blackjack_(DEALER, _order, (_order + 1) % 2);
		gotoXY_(1, 6);
		for (unsigned char eraser = 0; eraser < 9; eraser++) {
			putchar(' ');
		}
	}
	if (_blackjackcards[SPADE][0] == 200) {
		_insureance._is_insureance_posible = 1;
	}
	unsigned char _playercard1 = 99;
	unsigned char _playercard2 = 99;
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_blackjackcards[_type][_value] / 100 == PLAYER && _playercard1 == 99) {
				_playercard1 = _value;
				_playercard1++;
				switch (_playercard1) {
				case 11://J
				case 12://Q
				case 13://K
					_playercard1 = 10;
					break;
				case 1://Ace
					_playercard1 = 0;
					break;
				}
			}
			else if (_blackjackcards[_type][_value] / 100 == PLAYER && _playercard1 != 99) {
				_playercard2 = _value;
				_playercard2++;
				switch (_playercard2) {
				case 11://J
				case 12://Q
				case 13://K
					_playercard2 = 10;
					break;
				case 1://Ace
					_playercard2 = 0;
					break;
				}
			}
		}
	}
	if (_playercard1 == _playercard2) {
		_split._issplitavail = 1;
	}
	gotoXY_(0, 1);
	changeconsolecolor_(black, brightwhite);
	printcenter_("stake: ");
	printf("%lld", _blackjack_bet_money);
	if (_insureance._is_insureance_posible == 1) {
		_insureance._money_limit = _blackjack_bet_money;
		gotoXY_(0, OPTIONLIST_START_BLACKJACK);
		printright_("INSUREANCE amount");
		do {
			gotoXY_(100, OPTIONLIST_START_BLACKJACK + 1);
			changeconsolecolor_(black, brightwhite);
			for (unsigned char _eraser = 0; _eraser < 20; _eraser++) {
				putchar(' ');
			}
			gotoXY_(100, OPTIONLIST_START_BLACKJACK + 1);
			scanf_s("%lld", &_insureance._money);
			if (_insureance._money > _insureance._money_limit) {
				gotoXY_(0, OPTIONLIST_START_BLACKJACK + 2);
				printright_("that is impossible");
				_insureance._money = _insureance._money_limit + 1;
				char _junk = _getch();
				gotoXY_(100, OPTIONLIST_START_BLACKJACK + 2);
				for (unsigned char _eraser = 0; _eraser < 20; _eraser++) {
					putchar(' ');
				}
			}
		} while (_insureance._money == _insureance._money_limit + 1);
		clearall_();
		gotoXY_(0, 1);
		changeconsolecolor_(black, brightwhite);
		printcenter_("stake: ");
		printf("%lld", _blackjack_bet_money);
		_player_money = _player_money - _insureance._money;
		gotoXY_(0, 0);
		printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
		for (unsigned char _order = 0; _order < 2; _order++) {
			drawcard_blackjack_(PLAYER, _order, 1);
		}
		for (unsigned char _order = 0; _order < 2; _order++) {
			drawcard_blackjack_(DEALER, _order, (_order + 1) % 2);
			gotoXY_(1, 6);
			for (unsigned char eraser = 0; eraser < 9; eraser++) {
				putchar(' ');
			}
		}
	}
	drawoptionlist_blackjack_(_blackjackoption);
	char _credit[] = "credit";
	unsigned char _credit_input = 0;
	while (1) {
		unsigned char _playerturnoff_blackjack = 0;
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
		case 'w':
		case 'W':
		case 72:
			_blackjackoption = moveoptionlist_blackjack_(72, _blackjackoption);
			break;
		case 's':
		case 'S':
		case 80:
			_blackjackoption = moveoptionlist_blackjack_(80, _blackjackoption);
			break;
		case ' ':
		case 13://enter
			_playerturnoff_blackjack = selectcurrentoption_blackjack_(&_blackjackoption, PLAYER);
			break;
		case 134:
			for (unsigned char _type = 0; _type < 4; _type++) {
				for (unsigned char _value = 0; _value < 13; _value++) {
					if (_blackjackcards[_type][_value] / 100 == 2) {
						continue;
					}
					_blackjackcards[_type][_value] = 0;
				}
			}
			unsigned char _randval = rand() % 4;
			_blackjackcards[_randval][A] = 100;
			_blackjackcards[(_randval + 1) % 4][K] = 101;
			for (_randval = 0; _randval < 2; _randval++) {
				drawcard_blackjack_(PLAYER, _randval, 1);
			}
			break;
		default:
			if (_k_input == _credit[_credit_input]) {
				_credit_input++;
			}
			else {
				_credit_input = 0;
			}
			if (_credit_input == 6) {
				clearall_();
				gotoXY_(0, 0);
				printleft_("made on earth by human");
				moveXY_(0, 1);
				printleft_("CGOS present");
				moveXY_(0, 1);
				printleft_("Choi Gi O Developed");
				char _junk = _getch();
				_credit_input = 0;
				clearall_();
				gotoXY_(0, 0);
				printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
				for (unsigned char _order = 0; _order < 52; _order++) {
					drawcard_blackjack_(PLAYER, _order, 1);
				}
				for (unsigned char _order = 0; _order < 2; _order++) {
					drawcard_blackjack_(DEALER, _order, (_order + 1) % 2);
					gotoXY_(1, 6);
					for (unsigned char _eraser = 0; _eraser < 9; _eraser++) {
						putchar(' ');
					}
				}
				drawoptionlist_blackjack_(_blackjackoption);
			}
		}

		if (_playerturnoff_blackjack == 1) {
			break;
		}
	}
	if (_split._issplited == 1) {
		gotoXY_(1, 20);
		changeconsolecolor_(black, brightwhite);
		printright_("press any key to switch to split board");
		char junk = _getch();
		clearall_();
		gotoXY_(0, 0);
		printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
		for (unsigned char _order = 0; _order < 52; _order++) {
			drawcard_blackjack_(SPLIT, _order, 1);
		}
		printsumvalue_blackjack_(SPLIT);
		for (unsigned char _order = 0; _order < 2; _order++) {
			drawcard_blackjack_(DEALER, _order, (_order + 1) % 2);
		}
		printsumvalue_blackjack_(DEALER);
		gotoXY_(1, 6);
		for (unsigned char eraser = 0; eraser < 9; eraser++) {
			putchar(' ');
		}
		drawoptionlist_blackjack_(_blackjackoption);
		gotoXY_(0, 1);
		changeconsolecolor_(black, brightwhite);
		printcenter_("stake: ");
		printf("%lld", _blackjack_bet_money_split);
		while (1) {
			unsigned char _playerturnoff_blackjack = 0;
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
				_blackjackoption = moveoptionlist_blackjack_(_k_input, _blackjackoption);
				break;
			case 'w':
			case 'W':
				_blackjackoption = moveoptionlist_blackjack_(72, _blackjackoption);
				break;
			case 's':
			case 'S':
				_blackjackoption = moveoptionlist_blackjack_(80, _blackjackoption);
				break;
			case ' ':
			case 13://enter
				_playerturnoff_blackjack = selectcurrentoption_blackjack_(&_blackjackoption, SPLIT);
				break;
			case 134:
				for (unsigned char _type = 0; _type < 4; _type++) {
					for (unsigned char _value = 0; _value < 13; _value++) {
						if (_blackjackcards[_type][_value] / 100 == 1 || _blackjackcards[_type][_value] / 100 == 2) {
							continue;
						}
						_blackjackcards[_type][_value] = 0;
					}
				}
				unsigned char _randval = rand() % 4;
				_blackjackcards[_randval][A] = 300;
				_blackjackcards[(_randval + 1) % 4][K] = 301;
				for (_randval = 0; _randval < 2; _randval++) {
					drawcard_blackjack_(SPLIT, _randval, 1);
				}
				break;
			}

			if (_playerturnoff_blackjack == 1) {
				break;
			}
		}
		gotoXY_(1, 20);
		changeconsolecolor_(black, brightwhite);
		printright_("press any key to turn end");
		junk = _getch();
		clearall_();
		gotoXY_(0, 0);
		printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
		drawoptionlist_blackjack_(_blackjackoption);
		for (unsigned char _order = 0; _order < 2; _order++) {
			drawcard_blackjack_(DEALER, _order, (_order + 1) % 2);
		}
		printsumvalue_blackjack_(DEALER);
		gotoXY_(1, 6);
		for (unsigned char eraser = 0; eraser < 9; eraser++) {
			putchar(' ');
		}
		for (unsigned char _order = 0; _order < 52; _order++) {
			drawcard_blackjack_(PLAYER, _order, 1);
		}
		printsumvalue_blackjack_(PLAYER);
		gotoXY_(0, 1);
		changeconsolecolor_(black, brightwhite);
		printcenter_("stake: ");
		printf("%lld", _blackjack_bet_money);
	}
	//player turn off
	//dealer turn start
	dealerai_blackjack_();
	gotoXY_(0, 1);
	unsigned char _dealer_card_count = 0;
	for (unsigned char _type = 0; _type < 4; _type++) {
		for (unsigned char _value = 0; _value < 13; _value++) {
			if (_blackjackcards[_type][_value] / 100 == DEALER) {
				_dealer_card_count++;
			}
		}
	}
	if (_dealer_card_count == 2 && getsumvalue_blackjack_(DEALER) == 21) {
		_insureance._active = 1;
	}
	unsigned char _person = PLAYER;
	struct _blackjack_return {
		long long _player;
		long long _player_net;
		long long _split;
		long long _split_net;
		long long _total;
		long long _total_net;
	};
	struct _blackjack_return _blackjack_refund = { 0, };
	unsigned char _result = 0;
	if (getsumvalue_blackjack_(PLAYER) == 21 && countcard_(PLAYER) != 2 && getsumvalue_blackjack_(DEALER) == 21 && countcard_(DEALER) == 2) {
		_blackjack_refund._player = 0;
		_result = dealerwin;
	}
	else if (getsumvalue_blackjack_(PLAYER) == 21 && countcard_(PLAYER) == 2 && getsumvalue_blackjack_(DEALER) == 21 && countcard_(DEALER) != 2) {
		_blackjack_refund._player = _blackjack_bet_money * 2 * 1.5;
		_result = playerwin;
	}
	else if (getsumvalue_blackjack_(PLAYER) == getsumvalue_blackjack_(DEALER)) {
		_blackjack_refund._player = _blackjack_bet_money;
		_result = push;
	}
	else if ((getsumvalue_blackjack_(PLAYER) > getsumvalue_blackjack_(DEALER) && getsumvalue_blackjack_(PLAYER) <= 21) || getsumvalue_blackjack_(DEALER) > 21) {
		_blackjack_refund._player = _blackjack_bet_money * 2;
		if (getsumvalue_blackjack_(PLAYER) == 21 && countcard_(PLAYER) == 2) {
			_blackjack_refund._player = _blackjack_refund._player * 1.5;
		}
		_result = playerwin;
	}
	if (_insureance._active == 0 && _insureance._is_insureance_posible == 1) {
		_blackjack_refund._player = _blackjack_refund._player - _insureance._money;
	}
	else if ((getsumvalue_blackjack_(PLAYER) < getsumvalue_blackjack_(DEALER) && getsumvalue_blackjack_(DEALER) <= 21) || getsumvalue_blackjack_(PLAYER) > 21) {
		_blackjack_refund._player = 0;
		_result = dealerwin;
	}
	if (_insureance._active == 1) {
		_blackjack_refund._player = _blackjack_refund._player + (_insureance._money * 2);
	}
	unsigned char _result_split = 0;
	if (_split._issplited) {
		if (getsumvalue_blackjack_(SPLIT) == 21 && countcard_(SPLIT) != 2 && getsumvalue_blackjack_(DEALER) == 21 && countcard_(DEALER) == 2) {
			_blackjack_refund._split = 0;
			_result_split = dealerwin;
		}
		else if (getsumvalue_blackjack_(SPLIT) == 21 && countcard_(SPLIT) == 2 && getsumvalue_blackjack_(DEALER) == 21 && countcard_(DEALER) != 2) {
			_blackjack_refund._split = _blackjack_bet_money * 2 * 1.5;
			_result_split = playerwin;
		}
		if (getsumvalue_blackjack_(SPLIT) == getsumvalue_blackjack_(DEALER)) {
			_blackjack_refund._split = _blackjack_bet_money_split;
			_result_split = push;
		}
		else if ((getsumvalue_blackjack_(SPLIT) > getsumvalue_blackjack_(DEALER) && getsumvalue_blackjack_(SPLIT) <= 21) || getsumvalue_blackjack_(DEALER) > 21) {
			_blackjack_refund._split = _blackjack_bet_money_split * 2;
			if (getsumvalue_blackjack_(SPLIT) == 21) {
				_blackjack_refund._split = _blackjack_refund._split * 1.5;
			}
			_result_split = playerwin;
		}
		else if ((getsumvalue_blackjack_(SPLIT) < getsumvalue_blackjack_(DEALER) && getsumvalue_blackjack_(DEALER) <= 21) || getsumvalue_blackjack_(SPLIT) > 21) {
			_blackjack_refund._split = 0;
			_result_split = dealerwin;
		}
	}
	_blackjack_refund._player_net = _blackjack_refund._player - _blackjack_bet_money;
	_blackjack_refund._split_net = _blackjack_refund._split - _blackjack_bet_money_split;
	_blackjack_refund._total = _blackjack_refund._player + _blackjack_refund._split;
	_blackjack_refund._total_net = _blackjack_refund._player_net + _blackjack_refund._split_net;
	_player_money = _blackjack_refund._total + _player_money;
	savemoneytofile_(_player_money);
	if (_split._issplited == 1) {
		while (1) {
			switch (_person) {
			case PLAYER:
				clearall_();
				gotoXY_(0, 0);
				printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
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
				if (_insureance._active == 1) {
					printf(" insuranced");
				}
				moveXY_(0, 1);
				printcenter_("you get: ");
				printf("%lld", _blackjack_refund._player);
				moveXY_(0, 1);
				printcenter_("NET profit: ");
				printf("%lld", _blackjack_refund._player_net);
				moveXY_(0, 1);
				printcenter_("total get: ");
				printf("%lld", _blackjack_refund._total);
				moveXY_(0, 1);
				printcenter_("total NET profit: ");
				printf("%lld", _blackjack_refund._total_net);

				drawoptionlist_blackjack_(3);
				for (unsigned char _order = 0; _order < 52; _order++) {
					drawcard_blackjack_(DEALER, _order, 1);
				}
				printsumvalue_blackjack_(DEALER);
				for (unsigned char _order = 0; _order < 52; _order++) {
					drawcard_blackjack_(PLAYER, _order, 1);
				}
				printsumvalue_blackjack_(PLAYER);
				gotoXY_(1, 20);
				printright_("press space or enter key to switch table");
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
					case ' ':
					case 13://enter
						_person = SPLIT;
						break;
					}
				}
				break;
			case SPLIT:
				clearall_();
				gotoXY_(0, 0);
				printcenter_("press '`' to exit program, press backspace to back to menu, press escape to back to main menu");
				gotoXY_(0, 1);
				switch (_result_split) {
				case push:
					printcenter_("PUSH");
					break;
				case dealerwin:
					printcenter_("DEALER WIN");
					break;
				case playerwin:
					printcenter_("SPLIT WIN");
					break;
				}
				moveXY_(0, 1);
				printcenter_("you get: ");
				printf("%lld", _blackjack_refund._split);
				moveXY_(0, 1);
				printcenter_("NET profit: ");
				printf("%lld", _blackjack_refund._split_net);
				moveXY_(0, 1);
				printcenter_("total get: ");
				printf("%lld", _blackjack_refund._total);
				moveXY_(0, 1);
				printcenter_("total NET profit: ");
				printf("%lld", _blackjack_refund._total_net);
				drawoptionlist_blackjack_(3);
				for (unsigned char _order = 0; _order < 52; _order++) {
					drawcard_blackjack_(DEALER, _order, 1);
				}
				printsumvalue_blackjack_(DEALER);
				for (unsigned char _order = 0; _order < 52; _order++) {
					drawcard_blackjack_(SPLIT, _order, 1);
				}
				printsumvalue_blackjack_(SPLIT);
				gotoXY_(1, 20);
				printright_("press space or enter key to switch table");
				while (_person == SPLIT) {
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
					case ' ':
					case 13://enter
						_person = PLAYER;
						break;
					}
				}
				break;
			}
		}
	}
	else {
		gotoXY_(0, 1);
		clearline_();
		for (unsigned char _order = 0; _order < 52; _order++) {
			drawcard_blackjack_(DEALER, _order, 1);
		}
		printsumvalue_blackjack_(DEALER);
		for (unsigned char _order = 0; _order < 52; _order++) {
			drawcard_blackjack_(PLAYER, _order, 1);
		}
		printsumvalue_blackjack_(PLAYER);
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
		if (_insureance._active == 1) {
			printf(" insuranced");
		}
		moveXY_(0, 1);
		printcenter_("you get: ");
		printf("%lld", _blackjack_refund._player);
		moveXY_(0, 1);
		printcenter_("NET profit: ");
		printf("%lld", _blackjack_refund._player_net);
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
}