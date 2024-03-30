#include "CASINO_FUNCTION.h"

char _gamename[GAMECOUNT][50] = { "BLACKJACK", "VENTTI", "SLOT MACINE", "DEBUG" };
time_t _random_seed = 0;
long long _player_money;
long long _previous_bet_money = 0;
unsigned long long _string_buffer_malloc_size = 0;
char* __string_buffer;
unsigned char _debug_enable = FALSE;
unsigned char __debug_code[64] = "wwssadadbastart";
unsigned char _debug_code_cur = 0;


COORD getcurrentcurpos_(void) {
	HANDLE _handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO _presentcurpos;
	GetConsoleScreenBufferInfo(_handle, &_presentcurpos);
	COORD _pos;
	_pos.X = _presentcurpos.dwCursorPosition.X;
	_pos.Y = _presentcurpos.dwCursorPosition.Y;
	return _pos;
}
void gotoXY_(short _x, short _y) {
	HANDLE _handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD _targetpos;
	_targetpos.X = _x;
	_targetpos.Y = _y;
	SetConsoleCursorPosition(_handle, _targetpos);
}
void moveXY_(short _x, short _y) {
	COORD _pos = getcurrentcurpos_();
	gotoXY_(_pos.X + _x, _pos.Y + _y);
}
void changeconsolecolor_(unsigned char _backColor, unsigned char _textColor) {
	HANDLE _handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(_handle, (_backColor << 4) + _textColor);
}
void clearline_(void) {
	COORD _eraser = getcurrentcurpos_();
	changeconsolecolor_(black, brightwhite);
	for (unsigned char _eraserpos = 0; _eraserpos < 120; _eraserpos++) {
		gotoXY_(_eraserpos, _eraser.Y);
		putchar(' ');
	}
	gotoXY_(_eraser.X, _eraser.Y);
}
void clearall_(void) {
	COORD _curposmemory = getcurrentcurpos_();
	changeconsolecolor_(black, brightwhite);
	for (unsigned char _eraserpos = 0; _eraserpos < 30; _eraserpos++) {
		gotoXY_(0, _eraserpos);
		clearline_();
	}
	gotoXY_(_curposmemory.X, _curposmemory.Y);
}
void hold_(void) {
	char junk;
	gotoXY_(1, getcurrentcurpos_().Y + 1);
	printf("press any key to continue...");
	junk = _getch();
}
void printleft_(const char* __line) {
	COORD _posbackup = getcurrentcurpos_();
	gotoXY_(0, _posbackup.Y);
	unsigned char _countcharacter = 0;
	for (_countcharacter = 0; __line[_countcharacter] != NULL; _countcharacter++) {
		;
	}
	if (_countcharacter == 0) {
		printf("0 lenght line input error");
		return;
	}
	_countcharacter--;
	printf("%s", __line);
}
void printcenter_(const char* __line) {
	COORD _posbackup = getcurrentcurpos_();
	unsigned char _countcharacter = 0;
	for (_countcharacter = 0; __line[_countcharacter] != NULL; _countcharacter++) {
		;
	}
	if (_countcharacter == 0) {
		printf("0 lenght line input error");
		return;
	}
	_countcharacter--;
	if (_countcharacter % 2 != 0) {
		_countcharacter++;
	}
	gotoXY_(60 - _countcharacter / 2, _posbackup.Y);
	printf("%s", __line);
}
void printright_(const char* __line) {
	COORD _posbackup = getcurrentcurpos_();
	unsigned char _countcharacter = 0;
	for (_countcharacter = 0; __line[_countcharacter] != NULL; _countcharacter++) {
		;
	}
	if (_countcharacter == 0) {
		printf("0 lenght line input error");
		return;
	}
	gotoXY_(120 - _countcharacter, _posbackup.Y);
	printf("%s", __line);
}
void savemoneytofile_(long long _money_value) {
	long long _saved_time = time(NULL);
	srand(_saved_time);
	unsigned long long _saved_rand_operator = 0;
	for (unsigned char _bit = 0; _bit < 64; _bit++) {
		unsigned long long _digit = rand();
		_saved_rand_operator = _saved_rand_operator | (((_digit % 2) << _bit));
	}
	long long _saved_money_1 = _money_value ^ _saved_rand_operator;
	srand(rand());
	_saved_rand_operator = 0;
	for (unsigned char _bit = 0; _bit < 64; _bit++) {
		unsigned long long _digit = rand();
		_saved_rand_operator = _saved_rand_operator | (((_digit % 2) << _bit));
	}
	long long _saved_money_2 = _money_value ^ _saved_rand_operator;
	_saved_time = ~_saved_time;
	FILE* __money_save;
	fopen_s(&__money_save, "casinodata.casinodata", "r");
	if (__money_save == NULL) {
		clearall_();
		gotoXY_(0, 0);
		changeconsolecolor_(blue, brightwhite);
		gotoXY_(0, 0);
		for (unsigned char _eraserposy = 0; _eraserposy <= 30; _eraserposy++) {
			for (unsigned char _eraserpos = 0; _eraserpos < 120; _eraserpos++) {
				putchar(' ');
			}
			gotoXY_(0, _eraserposy);
		}
		gotoXY_(1, 0);
		puts("ERROR!");
		gotoXY_(1, 1);
		printf("CASINO data save fail. contect developer");
		changeconsolecolor_(brightwhite, blue);
		hold_();
		exit(1);
	}
	else {
		fclose(__money_save);

	}
	fopen_s(&__money_save, "casinodata.casinodata", "w");
	if (__money_save == NULL) {
		exit(1);
	}

	char _save_buffer[65];
	lltoa__(_saved_time, _save_buffer, 65);
	fputs(_save_buffer, __money_save);
	fputs("\n", __money_save);
	lltoa__(_saved_money_1, _save_buffer, 65);
	fputs(_save_buffer, __money_save);
	fputs("\n", __money_save);
	lltoa__(_saved_money_2, _save_buffer, 65);
	fputs(_save_buffer, __money_save);
	fputs("\n", __money_save);
	lltoa__((_saved_money_2 ^ _saved_money_1), _save_buffer, 65);
	fputs(_save_buffer, __money_save);
	fputs("\n", __money_save);
	fclose(__money_save);
}
unsigned long long readmoneyfromfile_(void) {
	FILE* __money_read;
	fopen_s(&__money_read, "casinodata.casinodata", "r");
	if (__money_read == NULL) {
		clearall_();
		gotoXY_(0, 0);
		changeconsolecolor_(blue, brightwhite);
		gotoXY_(0, 0);
		for (unsigned char _eraserposy = 0; _eraserposy <= 30; _eraserposy++) {
			for (unsigned char _eraserpos = 0; _eraserpos < 120; _eraserpos++) {
				putchar(' ');
			}
			gotoXY_(0, _eraserposy);
		}
		gotoXY_(1, 0);
		puts("ERROR!");
		gotoXY_(1, 1);
		printf("CASINO data read fail. contect developer");
		changeconsolecolor_(brightwhite, blue);
		hold_();
		exit(1);
	}
	else {
		fclose(__money_read);
	}
	fopen_s(&__money_read, "casinodata.casinodata", "a+");
	if (__money_read == NULL) {
		exit(1);
	}
	fseek(__money_read, 0, SEEK_SET);
	char _read_buffer[66];
	fgets(_read_buffer, 66, __money_read);
	for (unsigned char _delete_enter = 0; _delete_enter < 66; _delete_enter++) {
		if (_read_buffer[_delete_enter] == '\n') {
			_read_buffer[_delete_enter] = NULL;
			break;
		}
	}
	long long _saved_time = atollbin_(_read_buffer);
	_saved_time = ~_saved_time;
	srand(_saved_time);
	unsigned long long _saved_rand_operator = 0;
	for (unsigned char _bit = 0; _bit < 64; _bit++) {
		unsigned long long _digit = rand();
		_saved_rand_operator = _saved_rand_operator | (((_digit % 2) << _bit));
	}
	fgets(_read_buffer, 66, __money_read);
	for (unsigned char _delete_enter = 0; _delete_enter < 72; _delete_enter++) {
		if (_read_buffer[_delete_enter] == '\n') {
			_read_buffer[_delete_enter] = NULL;
			break;
		}
	}
	unsigned char _savefile_damaged = 0;
	long long _saved_money_1 = 0;
	_saved_money_1 = atollbin_(_read_buffer);
	unsigned long long _local_safety = _saved_money_1;
	_saved_money_1 = _saved_money_1 ^ _saved_rand_operator;
	srand(rand());
	_saved_rand_operator = 0;
	for (unsigned char _bit = 0; _bit < 64; _bit++) {
		unsigned long long _digit = rand();
		_saved_rand_operator = _saved_rand_operator | (((_digit % 2) << _bit));
	}
	fgets(_read_buffer, 66, __money_read);
	for (unsigned char _delete_enter = 0; _delete_enter < 66; _delete_enter++) {
		if (_read_buffer[_delete_enter] == '\n') {
			_read_buffer[_delete_enter] = NULL;
			break;
		}
	}
	long long _saved_money_2 = 0;
	_saved_money_2 = atollbin_(_read_buffer);
	_local_safety = _local_safety ^ _saved_money_2;
	_saved_money_2 = _saved_money_2 ^ _saved_rand_operator;
	fgets(_read_buffer, 66, __money_read);
	for (unsigned char _delete_enter = 0; _delete_enter < 66; _delete_enter++) {
		if (_read_buffer[_delete_enter] == '\n') {
			_read_buffer[_delete_enter] = NULL;
			break;
		}
	}
	unsigned long long _safety_checker = 0;
	_safety_checker = atollbin_(_read_buffer);
	if (_local_safety != _safety_checker || _saved_money_1 != _saved_money_2) {
		_savefile_damaged = 1;
	}
	if (_savefile_damaged == 1) {
		clearall_();
		gotoXY_(0, 0);
		changeconsolecolor_(blue, brightwhite);
		gotoXY_(0, 0);
		for (unsigned char _eraserposy = 0; _eraserposy <= 30; _eraserposy++) {
			for (unsigned char _eraserpos = 0; _eraserpos < 120; _eraserpos++) {
				putchar(' ');
			}
			gotoXY_(0, _eraserposy);
		}
		gotoXY_(1, 0);
		puts("ERROR!");
		gotoXY_(1, 1);
		printf("CASINO data damaged. contect developer");
		gotoXY_(1, 2);
		printf("%lld %lld %llu %llu", _saved_money_1, _saved_money_2, _local_safety, _safety_checker);
		changeconsolecolor_(brightwhite, blue);
		hold_();
		exit(1);
	}
	fclose(__money_read);
	return _saved_money_1;
}
void updaterandomseed_(void) {
	if (_random_seed != time(NULL)) {
		_random_seed = time(NULL);
	}
	srand(_random_seed);
}
char* lltoa__(long long _value, char* __str, int _len) {
	int i, j;
	for (i = 0; i < 64 && i < _len - 1; i++) {
		__str[i] = ((_value & ((long long)1 << (63 - i))) ? '1' : '0');
	}
	__str[i] = '\0';
	for (i = 0, j = strlen(__str) - 1; i < j; i++, j--) {
		char _temp = __str[i];
		__str[i] = __str[j];
		__str[j] = _temp;
	}
	return __str;
}
long long atollbin_(const char* __str) {
	long long _num = 0;
	for (unsigned char _cur = 0; __str[_cur] != NULL; _cur++) {
		_num = _num | ((long long)(__str[_cur] - '0') << _cur);
	}
	return _num;
}
void removeright_(void) {
	COORD _backup = getcurrentcurpos_();
	for (unsigned char _eraser = _backup.X; _eraser < 120; _eraser++) {
		putchar(' ');
	}
	gotoXY_(_backup.X, _backup.Y);
}
long long setstake_(unsigned char _current_game) {
	clearall_();
	gotoXY_(0, 0);
	printcenter_("YOU HAVE");
	printf(" %lld LEFT", _player_money);
	gotoXY_(0, 10);
	printcenter_(_gamename[_current_game]);
	gotoXY_(0, 11);
	printcenter_("bet: ");
	char _stake_input[32] = { 0, };
	unsigned char _cur = 0;
	long long _stake = 0;
	while (1) {
		unsigned char _k_input = _getch();

		if (_k_input >= 'A' && _k_input <= 'Z') {
			_k_input = _k_input + ('a' - 'A');
		}

		if (_k_input == 224) {
			_k_input = _getch();
		}


		if (_k_input == 13) {//enter
			if (_stake != 0) {
				_player_money = _player_money - _stake;
				savemoneytofile_(_player_money);
				_previous_bet_money = _stake;
				return _stake;
			}
			else if (_stake == 0) {
				for (unsigned char _eraser = 0; _eraser < 32; _eraser++) {
					_stake_input[_eraser] = NULL;
				}
				_cur = 0;
				gotoXY_(0, 11);
				changeconsolecolor_(black, brightwhite);
				clearline_();
				gotoXY_(0, 11);
				printcenter_("bet: ");
				gotoXY_(63, 11);
			}
		}
		else if (_k_input == '`' || _k_input == '~') {
			exit(1);
		}
		else if (_k_input == 9) {//tap
			char* __pay_list[] = { "all", "half", "quarter", "same", "help", "/?", "?" };
			if (stringautomatch__(_stake_input, __pay_list, 7) == NULL) {
				continue;
			}
			strcpy_s(_stake_input, 32, stringautomatch__(_stake_input, __pay_list, 7));
			gotoXY_(0, 11);
			printcenter_("bet: ");
			printf("%s", _stake_input);
			removeright_();
			_cur = strlen(_stake_input);
		}
		else if (_k_input == 8) {//BS
			if (_cur != 0) {
				_cur--;
				_stake_input[_cur] = NULL;
				gotoXY_(63 + _cur, 11);
				changeconsolecolor_(black, brightwhite);
				putchar(' ');
				moveXY_(-1, 0);
			}
		}
		else if (_k_input == 127 || _k_input == 83) {//del
			if (_stake_input[0] == NULL) {
				continue;
			}
			for (unsigned char _eraser = 0; _eraser < 32; _eraser++) {
				_stake_input[_eraser] = NULL;
			}
			_cur = 0;
			changeconsolecolor_(black, brightwhite);
			gotoXY_(0, 12);
			clearline_();
			gotoXY_(63, 11);
			for (unsigned char _eraser = 0; _eraser < 32; _eraser++) {
				putchar(' ');
			}
			gotoXY_(63, 11);
			continue;
		}
		else if (_k_input == 27) {//escape
			return -1;
		}
		else if ((_k_input >= '0' && _k_input <= '9') || (_k_input >= 'a' && _k_input <= 'z') || _k_input == '/' || _k_input == '?') {
			if ((_cur == 0 && _k_input == '0') || _cur == 32) {
				;
			}
			else {
				_stake_input[_cur] = _k_input;
				putchar(_k_input);
				_cur++;
			}
		}
		else {
			continue;
		}

		unsigned char _is_commandline = 0;
		for (unsigned char _for_cur = 0; _stake_input[_for_cur] != NULL; _for_cur++) {
			if ((_stake_input[_for_cur] >= 'a' && _stake_input[_for_cur] <= 'z') || _stake_input[_for_cur] == '/' || _stake_input[_for_cur] == '?') {
				_is_commandline = 1;
				break;
			}
		}

		changeconsolecolor_(black, brightwhite);
		gotoXY_(0, 12);
		clearline_();
		if (_cur == 32) {
			printcenter_("= ");
			printf("maximum lenght!");
			removeright_();
			continue;
		}
		if (_is_commandline == 1) {
			if (!strcmp(_stake_input, "all")) {
				printcenter_("= ");
				if (_player_money > 0) {
					printf("%lld", _player_money);
					removeright_();
					_stake = _player_money;
				}
				else {
					printf("N/A");
					removeright_();
					_stake = 0;
				}
			}
			else if (!strcmp(_stake_input, "half")) {
				long long _calculated_money = _player_money / 2;
				printcenter_("= ");
				if (_calculated_money > 0) {
					printf("%lld", _calculated_money);
					removeright_();
					_stake = _calculated_money;
				}
				else {
					printf("N/A");
					removeright_();
					_stake = 0;
				}
			}
			else if (!strcmp(_stake_input, "quarter")) {
				long long _calculated_money = _player_money / 4;
				printcenter_("= ");
				if (_calculated_money > 0) {
					printf("%lld", _calculated_money);
					removeright_();
					_stake = _calculated_money;
				}
				else {
					printf("N/A");
					removeright_();
					_stake = 0;
				}
			}
			else if (!strcmp(_stake_input, "same")) {
				long long _calculated_money = _previous_bet_money;
				printcenter_("= ");
				if (_calculated_money > 0 && _calculated_money <= _player_money) {
					printf("%lld", _calculated_money);
					removeright_();
					_stake = _calculated_money;
				}
				else {
					printf("N/A");
					removeright_();
					_stake = 0;
				}
			}
			else if (!strcmp(_stake_input, "help") || !strcmp(_stake_input, "?") || !strcmp(_stake_input, "/?")) {
				clearall_();
				gotoXY_(0, 0);
				puts("commands:");
				puts("`all` for all in");
				puts("`half` for half in");
				puts("`quarter` for quarter in");
				puts("`same` for same in at previous bet");
				puts("any kind decimal number for specific in");
				puts("`help`, `/?`, `?` for commands help");
				hold_();
				clearall_();
				gotoXY_(0, 0);
				printcenter_("YOU HAVE");
				printf(" %lld LEFT", _player_money);
				gotoXY_(0, 10);
				printcenter_(_gamename[_current_game]);
				gotoXY_(0, 11);
				printcenter_("bet: ");
				for (unsigned char _eraser = 0; _eraser < 32; _eraser++) {
					_stake_input[_eraser] = NULL;
				}
				_cur = 0;
				gotoXY_(63, 11);
				_stake = 0;
			}
			else {
				printcenter_("= ");
				printf("unknown command line");
				removeright_();
				_stake = 0;
			}
		}
		else {
			clearline_();
			if (atoll(_stake_input) <= _player_money) {
				_stake = atoll(_stake_input);
			}
			else {
				_stake = 0;
				printcenter_("= ");
				printf("BIGGER than Player Money!");
				removeright_();
			}
		}

		gotoXY_(63 + _cur, 11);
	}
}
char* stringautomatch__(char* __input, char* __list[], unsigned short _list_count) {
	unsigned long long int _min_difference = strnlen_s(__input, 64);
	unsigned char* __candidate = NULL;
	for (int i = 0; i < _list_count; i++) {
		int difference = 0;

		for (int j = 0; __input[j] != '\0' && __list[i][j] != '\0'; j++) {
			if (__input[j] != __list[i][j]) {
				difference++;
			}
		}

		if (difference < _min_difference) {
			_min_difference = difference;
			__candidate = __list[i];
		}
	}

	return __candidate;
	/*
	unsigned short _match_table[100][4] = {{0,},};

	struct _clip_board {
		unsigned short _index;
		unsigned long long _matches;
		unsigned long long _length_error;
	};
	struct _clip_board _candidate = { 0, 0, MAXULONG64};

	for (unsigned short _search_list = 0; _search_list < _list_count; _search_list++) {
		struct _clip_board _compare = { 0, };
		for (unsigned short _search_start_index = 0; _search_start_index < strlen(__input); _search_start_index++) {
			for (unsigned short _search_end_index = _search_start_index; _search_end_index < strlen(__input); _search_end_index++) {

				if (ishavestringinside_(__input,getstringbetween__(__list[_search_list],_search_start_index,_search_end_index))) {
					if (__list[_search_list][0] == '?') {
						break;
					}
					_compare._matches++;
				}
			}
		}
		_match_table[_search_list][0] = _search_list;
		_match_table[_search_list][1] = _compare._matches;
		_compare._matches = 0;
		_match_table[_search_list][2] = abs(strlen(__list[_search_list]) - strlen(__input));
	}
	for (unsigned short _compare_list = 0; _compare_list < _list_count; _compare_list++) {
		if (_candidate._matches <= _match_table[_compare_list][1]) {
			if (_candidate._length_error > _match_table[_compare_list][2]) {
				_candidate._index = _match_table[_compare_list][0];
				_candidate._matches = _match_table[_compare_list][1];
				_candidate._length_error = _match_table[_compare_list][2];
			}
			else {
				_match_table[_compare_list][3]++;
			}
		}
	}

	return __list[_candidate._index];
	*/
}
unsigned char randompercent_(long double _percent) {
	long long _randomValue = rand() % 10000 + 1;
	long double _scaledProbability = _percent * 10000;

	if (_randomValue <= _scaledProbability) {
		return 1;
	}
	else {
		return 0;
	}
}
char* getstringbetween__(char* __string, unsigned short _start, unsigned short _end) {
	unsigned short _sizeof_string = strnlen_s(__string, _end + 1);
	if (__string[_start] == '\0') {
		return 0;
	}
	mallocstringbuffer_(sizeof(char*) * _sizeof_string);
	for (unsigned short _cur = _start; _cur <= _end; _cur++) {
		__string_buffer[_cur - _start] = __string[_cur];
	}
	__string_buffer[_end - _start + 1] = '\0';
	return __string_buffer;
}
char ishavestringinside_(char* __target, char* __filed) {
	char* __answer = strstr(__filed, __target);
	if (__answer == NULL) {
		return 0;
	}
	else {
		return 1;
	}
}
void mallocstringbuffer_(unsigned long long _malloc_size) {
	if (_string_buffer_malloc_size != 0) {
		free(__string_buffer);
	}
	__string_buffer = malloc(sizeof(char) * _malloc_size);
	if (__string_buffer == NULL) {
		changeconsolecolor_(blue, brightwhite);
		for (unsigned char _eraserpos = 0; _eraserpos < 30; _eraserpos++) {
			gotoXY_(0, _eraserpos);
			for (unsigned char _eraserpos = 0; _eraserpos < 120; _eraserpos++) {
				putchar(' ');
			}
		}
		gotoXY_(0, 0);
		printleft_("ERROR! out of memory !");
		SetConsoleTitleA("ERROR!");
		gotoXY_(0, 1);
		hold_();
		exit(1);
	}
	_string_buffer_malloc_size = _malloc_size;
}