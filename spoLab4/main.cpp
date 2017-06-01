#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

void main()
{
	cout << "Lab 4\n\n";
	cout << "Enter file name\n";
	string filename;
	cin >> filename;

	Lexer lexer;
	vector<token_pair> tokens = lexer.getTokens(filename);

	if (!tokens.empty())
	{
		cout << "\nLexer didn't find errors.\n";

		Parser parser;
		if (parser.checkSyntax(tokens))
			cout << "\nParser didn't find errors.\n";
		else
			cout << "\nParser found an error.\n";
	}
	else
		cout << "\nLexer found an error.\n";

	Interpreter interpr;
	interpr.calc_reverse_polish_notation(tokens);
	interpr.printRPN();

	cout << "\n";
	system("pause");
}