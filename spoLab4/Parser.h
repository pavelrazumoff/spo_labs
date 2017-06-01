#pragma once
#include "lexer.h"

struct Token
{
	string name;
	string rx;
};

class Parser
{
public:
	bool checkSyntax(vector<token_pair> tokens);
	vector<Token> getSyntaxRegex();
private:
	vector<Token> getRegexFromFile();
};