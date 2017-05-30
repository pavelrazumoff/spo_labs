#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

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
		bool correct = parser.checkSyntax(tokens);
		if (correct)
			cout << "\nParser didn't find errors.\n";
		else
			cout << "\nParser found an error.\n";
	}
	else
		cout << "\nLexer found an error.\n";

	Interpreter interpr;
	vector<string> rpn = interpr.get_reverse_polish_notation(tokens);

	cout << "\n";
	for (int i = 0; i < rpn.size(); ++i)
		cout << i + 1 << " " << rpn[i] << "\n";

	cout << "\n";
	system("pause");
}