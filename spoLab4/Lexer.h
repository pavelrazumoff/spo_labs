#pragma once
#include "header.h"

struct token_pair
{
	string name;
	string value;
};

struct Tokens
{
	string name;
	string rx;
	int priority;
};

class Lexer
{
public:
	vector<token_pair>		getTokens(string fname);
	vector<Tokens>			getRegexFromFile();
private:
	vector<Tokens>			findMatch(string str, vector<Tokens> & myRegex);
	void					eraseWhitespaces(string & str);
	string					readSourceCodeFromFile(string fname);
};