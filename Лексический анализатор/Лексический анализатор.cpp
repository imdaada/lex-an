#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

enum tok_names { KWORD, IDENT, NUM, FLOAT, DELIM };
ifstream fin("txt.txt");

struct token
{

	enum tok_names token_name;
	char* token_value;
	int str_num;

};
struct lexeme_table
{

	struct token tok;
	struct lexeme_table* next;

};

struct magazin
{
	int symbol;
	struct magazin* next;
};

struct magazin* top = NULL;

struct lexeme_table* lt = NULL;
struct lexeme_table* lt_head = NULL;
int str_num = 0;

void add_token(enum tok_names token_name, const char* token_value) {
	if (lt == NULL)
	{
		lt = new lexeme_table;
		lt->next = NULL;
		lt->tok.token_name = token_name;
		lt->tok.token_value = (char*)token_value;
		lt->tok.str_num = str_num;
		lt_head = lt;
	}
	else
	{
		lt->next = new lexeme_table;
		lt->next->next = NULL;
		lt->next->tok.token_name = token_name;
		lt->next->tok.token_value = (char*)token_value;
		lt->next->tok.str_num = str_num;
		lt = lt->next;
	}
}

magazin* put_to_magazin(int i, magazin* m) {
	if (m == NULL)
	{
		m = new magazin();
		m->next = NULL;
		m->symbol = i;
	}
	else {
		magazin* tmp;
		tmp = new magazin();
		tmp->next = m;
		tmp->symbol = i;
		m = tmp;
	}
	return m;
}

magazin* get_from_magazin(magazin* m) {
	return m;
}

magazin* delete_from_magazin(magazin* m) {
	if (m != NULL) {
		magazin* tmp;
		tmp = m->next;
		delete m;
		m = tmp;
	}
	return m;
}

int trans_to_table(lexeme_table* lt) {
	if (lt == NULL)
	{
		return 13;
	}
	else if (strcmp(lt->tok.token_value, ")")==0)
		return 0;
	else if (strcmp(lt->tok.token_value, "(")==0)
		return 1;
	else if (strcmp(lt->tok.token_value, "do")==0)
		return 2;
	else if (strcmp(lt->tok.token_value, "while")==0)
		return 3;
	else if (strcmp(lt->tok.token_value, "for")==0)
		return 4;
	else if (strcmp(lt->tok.token_value, ";")==0)
		return 5;
	else if (strcmp(lt->tok.token_value, ",")==0)
		return 6;
	else if (strcmp(lt->tok.token_value, "=")==0)
		return 7;
	else if (lt->tok.token_name == 1)
		return 8;
	else if ((lt->tok.token_name == 2))
		return 9;
	else if ((lt->tok.token_name == 3))
		return 10;
	else if (strcmp(lt->tok.token_value, "}")==0)
		return 11;
	else if (strcmp(lt->tok.token_value, "{")==0)
		return 12;
	else
		return 13;
}

bool compare(magazin** mg, magazin** rule) {
	magazin* help_mg = NULL;
	magazin* help_rule = NULL;
	while (1)
	{
		if (*rule == NULL)
		{
			while (help_rule != NULL)
			{
				*rule = put_to_magazin(help_rule->symbol, *rule);
				help_rule = delete_from_magazin(help_rule);
			}
			return true;
		}
		else if (*mg == NULL) {
			while (help_mg != NULL)
			{
				*mg = put_to_magazin(help_mg->symbol, *mg);
				*rule = put_to_magazin(help_rule->symbol, *rule);
				help_mg = delete_from_magazin(help_mg);
				help_rule = delete_from_magazin(help_rule);
			}
			return false;
		}
		else if (get_from_magazin(*mg)->symbol == get_from_magazin(*rule)->symbol) {
			help_mg = put_to_magazin((*mg)->symbol, help_mg);
			help_rule = put_to_magazin((*rule)->symbol, help_rule);
			*mg = delete_from_magazin(*mg);
			*rule = delete_from_magazin(*rule);
		}
		else {
			while (help_mg != NULL)
			{
				*mg = put_to_magazin(help_mg->symbol, *mg);
				*rule = put_to_magazin(help_rule->symbol, *rule);
				help_mg = delete_from_magazin(help_mg);
				help_rule = delete_from_magazin(help_rule);
			}
			return false;
		}
	}
}

int state_key_words = 0;
int state_other = 0;
int num_word = 3;
int iterator_kwords = 0;
string* kwords = new string[100];

int avtomat[8][8] = {
	{2, 1, 4, 4, 5, -1, 5, -6},
	{-3, 1, -3, -3, 6, -3, -3, -6},
{2, 3, -2, -2, -2, -2, -2, -6},
{-2, 3, -2, -2, -2, -2, -2, -6},
{-5, 1, -5, -5, -5, -5, -5, -6},
{-5, -5, -5, -5, -5, -5, 5, -6},
{-1, 7, -1, -1, -1, -1, -1, -6},
{-4, 7, -4, -4, -4, -4, -4, -6}
};


int syntax[20][14]{
	{-1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1},
	{-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1},
	{-1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1},
	{-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, 6, -1, 6},
	{-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1},
	{7, -1, -1, -1, -1, 7, 7, 0, -1, -1, -1, -1, -1, -1},
	{8, -1, -1, -1, -1, 8, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, -1, -1, -1, -1, 9, 9, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, 10, -1, 10},
	{-1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, -1},
	{-1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1},
	{-1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, 1},
	{-1, -1, 2, 2, 2, -1, -1, -1, -1, -1, -1, 2, 2, 2},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, 3},
	{4, -1, -1, -1, -1, 4, 4, -1, -1, -1, -1, -1, -1, -1},
	{0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, -1, -1, -1, -1, 5, 5, -1, -1, -1, -1, -1, -1, -1}
};


int trans_to_other(char a) {
	if (a >= 'a' && a <= 'z')
		return 0;
	else if (a >= '0' && a <= '9')
		return 1;
	else if (a == '.')
		return 4;
	else if (a == '+')
		return 2;
	else if (a == '-')
		return 3;
	else if (a == '\0')
		return 5;
	else if (a == '~')
		return 7;
	else
		return 6;
}

tok_names trans_answ(int a) {
	switch (a)
	{
	case -2:
		return IDENT;
	case -3:
		return NUM;
	case -4:
		return FLOAT;
	case -5:
		return DELIM;
	}
}


int main()
{
	magazin* rul_1 = NULL;
	rul_1 = put_to_magazin(14, rul_1);
	rul_1 = put_to_magazin(15, rul_1);
	magazin* rule_2 = NULL;
	rule_2 = put_to_magazin(15, rule_2);
	magazin* rule_3 = NULL;
	rule_3 = put_to_magazin(16, rule_3);
	magazin* rule_5 = NULL;
	rule_5 = put_to_magazin(12, rule_5);
	rule_5 = put_to_magazin(14, rule_5);
	rule_5 = put_to_magazin(11, rule_5);
	magazin* rule_6 = NULL;
	rule_6 = put_to_magazin(12, rule_6);
	rule_6 = put_to_magazin(11, rule_6);
	magazin* rule_7 = NULL;
	rule_7 = put_to_magazin(3, rule_7);
	rule_7 = put_to_magazin(1, rule_7);
	rule_7 = put_to_magazin(18, rule_7);
	rule_7 = put_to_magazin(0, rule_7);
	rule_7 = put_to_magazin(15, rule_7);
	magazin* rule_8 = NULL;
	rule_8 = put_to_magazin(2, rule_8);
	rule_8 = put_to_magazin(15, rule_8);
	rule_8 = put_to_magazin(3, rule_8);
	rule_8 = put_to_magazin(1, rule_8);
	rule_8 = put_to_magazin(18, rule_8);
	rule_8 = put_to_magazin(0, rule_8);
	rule_8 = put_to_magazin(5, rule_8);
	magazin* rule_9 = NULL;
	rule_9 = put_to_magazin(4, rule_9);
	rule_9 = put_to_magazin(1, rule_9);
	rule_9 = put_to_magazin(18, rule_9);
	rule_9 = put_to_magazin(5, rule_9);
	rule_9 = put_to_magazin(18, rule_9);
	rule_9 = put_to_magazin(5, rule_9);
	rule_9 = put_to_magazin(18, rule_9);
	rule_9 = put_to_magazin(0, rule_9);
	rule_9 = put_to_magazin(15, rule_9);
	magazin* rule_10 = NULL;
	rule_10 = put_to_magazin(4, rule_10);
	rule_10 = put_to_magazin(1, rule_10);
	rule_10 = put_to_magazin(5, rule_10);
	rule_10 = put_to_magazin(18, rule_10);
	rule_10 = put_to_magazin(5, rule_10);
	rule_10 = put_to_magazin(18, rule_10);
	rule_10 = put_to_magazin(0, rule_10);
	rule_10 = put_to_magazin(15, rule_10);
	magazin* rule_11 = NULL;
	rule_11 = put_to_magazin(4, rule_11);
	rule_11 = put_to_magazin(1, rule_11);
	rule_11 = put_to_magazin(18, rule_11);
	rule_11 = put_to_magazin(5, rule_11);
	rule_11 = put_to_magazin(5, rule_11);
	rule_11 = put_to_magazin(18, rule_11);
	rule_11 = put_to_magazin(0, rule_11);
	rule_11 = put_to_magazin(15, rule_11);
	magazin* rule_12 = NULL;
	rule_12 = put_to_magazin(4, rule_12);
	rule_12 = put_to_magazin(1, rule_12);
	rule_12 = put_to_magazin(18, rule_12);
	rule_12 = put_to_magazin(5, rule_12);
	rule_12 = put_to_magazin(18, rule_12);
	rule_12 = put_to_magazin(5, rule_12);
	rule_12 = put_to_magazin(0, rule_12);
	rule_12 = put_to_magazin(15, rule_12);
	magazin* rule_13 = NULL;
	rule_13 = put_to_magazin(4, rule_13);
	rule_13 = put_to_magazin(1, rule_13);
	rule_13 = put_to_magazin(5, rule_13);
	rule_13 = put_to_magazin(5, rule_13);
	rule_13 = put_to_magazin(18, rule_13);
	rule_13 = put_to_magazin(0, rule_13);
	rule_13 = put_to_magazin(15, rule_13);
	magazin* rule_14 = NULL;
	rule_14 = put_to_magazin(4, rule_14);
	rule_14 = put_to_magazin(1, rule_14);
	rule_14 = put_to_magazin(18, rule_14);
	rule_14 = put_to_magazin(5, rule_14);
	rule_14 = put_to_magazin(5, rule_14);
	rule_14 = put_to_magazin(0, rule_14);
	rule_14 = put_to_magazin(15, rule_14);
	magazin* rule_15 = NULL;
	rule_15 = put_to_magazin(4, rule_15);
	rule_15 = put_to_magazin(1, rule_15);
	rule_15 = put_to_magazin(5, rule_15);
	rule_15 = put_to_magazin(18, rule_15);
	rule_15 = put_to_magazin(5, rule_15);
	rule_15 = put_to_magazin(0, rule_15);
	rule_15 = put_to_magazin(15, rule_15);
	magazin* rule_16 = NULL;
	rule_16 = put_to_magazin(4, rule_16);
	rule_16 = put_to_magazin(1, rule_16);
	rule_16 = put_to_magazin(5, rule_16);
	rule_16 = put_to_magazin(5, rule_16);
	rule_16 = put_to_magazin(0, rule_16);
	rule_16 = put_to_magazin(15, rule_16);
	magazin* rule_17 = NULL;
	rule_17 = put_to_magazin(17, rule_17);
	magazin* rule_18 = NULL;
	rule_18 = put_to_magazin(18, rule_18);
	rule_18 = put_to_magazin(6, rule_18);
	rule_18 = put_to_magazin(17, rule_18);
	magazin* rule_19 = NULL;
	rule_19 = put_to_magazin(8, rule_19);
	rule_19 = put_to_magazin(7, rule_19);
	rule_19 = put_to_magazin(19, rule_19);
	magazin* rule_20 = NULL;
	rule_20 = put_to_magazin(8, rule_20);
	magazin* rule_21 = NULL;
	rule_21 = put_to_magazin(9, rule_21);
	magazin* rule_22 = NULL;
	rule_22 = put_to_magazin(10, rule_22);
	magazin* dopusk = NULL;
	dopusk = put_to_magazin(14, dopusk);

	char a;
	char buf[50];
	/*cout << a;
	add_token(KWORD, b);
	cout << lt->tok.token_value;*/
	ifstream lex("lex.txt");
	while (!lex.eof()) {
		//fin >> a;
		char* buff = new char[100];
		if (!lex.eof())
		{
			/*int i = trans_to_state_key_words(a);
			state_key_words = key_words[state_key_words][i];
			if (state_key_words < 0) {
				key_word_result(state_key_words);
				cout << lt_head->tok.token_value;
				return 0;
			}*/
			lex.getline(buff, 100);
			kwords[iterator_kwords] = buff;
			/*if (!kwords[iterator_kwords].compare("for")) {
				cout << kwords[iterator_kwords];
			}*/
			iterator_kwords++;
		}
	}
	ofstream off("txt_help.txt");
	while (!fin.eof()) {
		char* buff = new char[300];
		if (!fin.eof())
		{
			fin.getline(buff, 300);
			off << buff << ' ' << '~' << ' ';
		}
	}
	off.close();
	fin.close();
	fin.open("txt_help.txt");
	while (!fin.eof()) {
		char* buff = new char[300];
		if (!fin.eof())
		{
			fin.getline(buff, 100, ' ');
			bool check = false;
			for (int i = 0; i < iterator_kwords; i++)
			{
				if (kwords[i] == buff)
				{
					check = true;
					add_token(KWORD, buff);
				}
			}
			if (!check) {
				int o = 0;
				int o_l = 0;
				int state = 0;
				int lex_int;
				char* h;
				char* lexem = new char[100];
				while (buff[o] != '\0') {
					lex_int = trans_to_other(buff[o]);
					state = avtomat[state][lex_int];
					switch (state)
					{
					case -1: 
						cout << "error" << '\n';
						state = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -2:
						lexem[o_l] = '\0';
						add_token(trans_answ(-2), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -3:
						lexem[o_l] = '\0';
						add_token(trans_answ(-3), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -4:
						lexem[o_l] = '\0';
						add_token(trans_answ(-4), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -5:
						lexem[o_l] = '\0';
						add_token(trans_answ(-5), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -6:
						str_num++;
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						o++;
						break;
					default:
						lexem[o_l] = buff[o];
						o++;
						o_l++;
						break;
					}
				}
					lexem[o_l] = buff[0];
					lex_int = trans_to_other(buff[o]);
					state = avtomat[state][lex_int];
					switch (state)
					{
					case -1:
						//cout << "error" << '\n';
						state = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -2:
						lexem[o_l] = '\0';
						add_token(trans_answ(-2), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -3:
						lexem[o_l] = '\0';
						add_token(trans_answ(-3), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -4:
						lexem[o_l] = '\0';
						add_token(trans_answ(-4), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					case -5:
						lexem[o_l] = '\0';
						add_token(trans_answ(-5), lexem);
						state = 0;
						o_l = 0;
						h = lexem;
						lexem = new char[100];
						break;
					}
			}
		}
	}
	//add_token(KWORD, "sds");
	lexeme_table *ll = lt_head;
	while (ll != NULL)
	{
		cout << ll->tok.token_name << ' ' << ll->tok.token_value << ' ' << trans_to_table(ll) << ' ' << ll->tok.str_num <<'\n';
		ll = ll->next;
	}
	cout << trans_to_table(NULL) << '\n';

	// test
	magazin* m1 = NULL, *m2 = NULL;
	m1 = put_to_magazin(1, m1);
	m1 = put_to_magazin(2, m1);
	m2 = put_to_magazin(2, m2);
	m2 = put_to_magazin(1, m2);
	cout  << m1->symbol << m2->symbol << compare(&m1, &m2) << '\n';
	//*
	magazin* magz = NULL;
	int from_magz, from_sequense, resul;
	ll = lt_head;
	while (1)
	{
		if (magz == NULL) {
			from_magz = 13;
		}
		else
			from_magz = magz->symbol;
		if (ll == NULL) {
			from_sequense = 13;
		}
		else
			from_sequense = trans_to_table(ll);
		resul = syntax[from_magz][from_sequense];
		
		switch (resul)
		{
		case 0:
			magz = put_to_magazin(from_sequense, magz);
			ll = ll->next;
			break;
		case 1:
			if (compare(&magz, &dopusk)) {
			//	if (magz == NULL) {
					cout << "DOPUSTIT";
					return 0;
			//	}
			//	else {
			//		cout << "Error - o-n";
			//		return 0;
			//	}
			}
			else {
				cout << "Error - o";
				return 0;
			}
		case 2:
			if (compare(&magz, &rule_7))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_9))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_10))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_11))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_12))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_13))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_14))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_15))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rule_16))
				magz = put_to_magazin(16, magz);
			else if (compare(&magz, &rul_1))
				 magz = put_to_magazin(14, magz);
			else if (compare(&magz, &rule_2))
				 magz = put_to_magazin(14, magz);
			else {
				cout << "Error - 2";
				return 0;
			}
			break;
		case 3:
			if (compare(&magz, &rule_3))
				magz = put_to_magazin(15, magz);
			else {
				cout << "Error - 3";
				return 0;
			}
			break;
		case 4:
			if (compare(&magz, &rule_17))
				magz = put_to_magazin(18, magz);
			else if (compare(&magz, &rule_18))
				magz = put_to_magazin(18, magz);
			else {
				cout << "Error - 4";
				return 0;
			}
			break;
		case 5:
			if (compare(&magz, &rule_19))
				magz = put_to_magazin(17, magz);
			else {
				cout << "Error - 5";
				return 0;
			}
			break;
		case 6:
			if (compare(&magz, &rule_8))
				magz = put_to_magazin(16, magz);
			else {
				cout << "Error - 6";
				return 0;
			}
			break;
		case 7:
			if (compare(&magz, &rule_20))
				magz = put_to_magazin(19, magz);
			else {
				cout << "Error - 7";
				return 0;
			}
			break;
		case 8:
			if (compare(&magz, &rule_21))
				magz = put_to_magazin(19, magz);
			else {
				cout << "Error - 8";
				return 0;
			}
			break;
		case 9:
			if (compare(&magz, &rule_22))
				magz = put_to_magazin(19, magz);
			else {
				cout << "Error - 9";
				return 0;
			}
			break;
		case 10:
			if (compare(&magz, &rule_5))
				magz = put_to_magazin(15, magz);
			else if (compare(&magz, &rule_6))
				magz = put_to_magazin(15, magz);
			else {
				cout << "Error - 10";
				return 0;
			}
			break;
		case -1:
			cout << "Error - sq";
			return 0;
		}

		//ll = ll->next;
	}
}
