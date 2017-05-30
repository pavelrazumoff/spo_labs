#include "Parser.h"

bool Parser::checkSyntax(vector<token_pair> tokens)
{
	string tokens_in_str;
	for (int i = 0; i < tokens.size(); ++i)
		tokens_in_str = tokens_in_str + tokens[i].name + " ";

	tokens_in_str.erase(tokens_in_str.size() - 1, 1);
	cout << "tokensInStr --> ~" << tokens_in_str << "~\n";

	vector<token> syntax_rx = getSyntaxRegex();

	//for (int i = 0; i < syntaxRx.size(); ++i)
		//cout << syntaxRx[i].name << " == " << syntaxRx[i].rx << "\n";

	cout << "\n";
	for (int i = 0; i < syntax_rx.size(); ++i)
	{
		tr1::regex rx(syntax_rx[i].rx);
		tr1::match_results<const char *> name;
		if (tr1::regex_search(tokens_in_str.c_str(), name, rx))
			cout << left << setw(18) << syntax_rx[i].name << " found\n";
		else
			cout << left << setw(18) << syntax_rx[i].name << " not found!\n";
	}

	tr1::regex rx(syntax_rx[0].rx);
	return tr1::regex_search(tokens_in_str.c_str(), rx);
}

vector<token> Parser::getRegexFromFile()
{
	vector<token> result;
	ifstream rx_file("syntax.txt");

	if (!rx_file.is_open())
	{
		cout << "\nFile with regular expressions syntax.txt can't be opened!\n";
		exit(0);
	}
	else
	{
		token syntax;
		while (!rx_file.eof())
		{
			string regx;
			getline(rx_file, regx);
			if (regx.empty())
				break;

			string rx_name;
			string rx_rx;

			tr1::regex rxN("^[a-z_]+");
			tr1::match_results<const char *> name;

			tr1::regex_search(regx.c_str(), name, rxN);
			rx_name = name.str();
			regx.erase(0, name.str().size() + 2);
			rx_rx = regx;
			
			result.push_back(token{ rx_name , rx_rx });
		}
		rx_file.close();
	}
	return result;
}

vector<token> Parser::getSyntaxRegex()
{
	vector<token> syntax_rx = getRegexFromFile();

	for (int k = 0; k < 5; ++k)
		for (int i = 0; i < syntax_rx.size(); ++i)
			for (int j = 0; j < syntax_rx.size(); ++j)
			{
				tr1::regex rxN(syntax_rx[j].name);
				syntax_rx[i].rx = tr1::regex_replace(syntax_rx[i].rx.c_str(), rxN, syntax_rx[j].rx);
			}
	syntax_rx[0].rx = "^" + syntax_rx[0].rx + "$";

	return syntax_rx;
}