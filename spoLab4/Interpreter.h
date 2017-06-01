#pragma once
#include "parser.h"

class Interpreter
{
public:
	void			calc_reverse_polish_notation(vector<token_pair> code);
	void			printRPN();
private:
	vector<string>	revPolNotation;
};