#include "Lexer.h"

vector<token_pair> Lexer::getTokens(string fname)
{
	string s_code = readSourceCodeFromFile(fname);
	vector<Lexer::tokens> regex = getRegexFromFile();
	vector<token_pair> tokens;

	while (!s_code.empty())
	{
		eraseWhitespaces(s_code);
		vector<Lexer::tokens> tmpTokens;
		string buffer;
		bool solved = false;
		if (s_code.size() != 0)
		{
			do
			{
				buffer.push_back(s_code[0]);
				tmpTokens = findMatch(buffer, regex);
				if (tmpTokens.empty())
				{
					buffer.erase(buffer.size() - 1, 1);
					tmpTokens = findMatch(buffer, regex);
					if (tmpTokens.empty())
					{
						buffer.push_back(s_code[0]);
						cout << "\n\nError! Unknown symbol \"" << buffer << "\"\n";
						tokens.clear();
						return tokens;
					}
					Lexer::tokens tempResult = tmpTokens[0];
					for (int i = 1; i < tmpTokens.size(); ++i)
					{
						if (tmpTokens[i].priority < tempResult.priority)
							tempResult = tmpTokens[i];
					}
					tokens.push_back({ tempResult.name, buffer });
					solved = true;
				}
				else
					s_code.erase(0, 1);
			} while (!solved);
		}
	}

	return tokens;
}

vector<Lexer::tokens> Lexer::getRegexFromFile()
{
	vector<Lexer::tokens> result;
	ifstream regexfile("regex.txt");

	if (!regexfile.is_open())
	{
		cout << "\nFile with regular expressions regex.txt can't be opened!\n";
		exit(0);
	}
	else
	{
		while (!regexfile.eof())
		{
			string regx;
			getline(regexfile, regx);
			if (regx.empty())
				break;
			string rgx_name;
			string rgx_rx;
			int rx_pr;

			tr1::regex rxN("^[A-Z_]+");
			tr1::match_results<const char *> name;
			tr1::regex_search(regx.c_str(), name, rxN);

			rgx_name = name.str();
			regx.erase(0, name.str().size() + 1);

			rgx_rx = regx.substr(0, regx.size() - 2);
			regx.erase(0, rgx_rx.size() + 1);

			rx_pr = atoi(regx.c_str());

			result.push_back(Lexer::tokens{ rgx_name , rgx_rx , rx_pr });
		}
		regexfile.close();
	}
	return result;
}

vector<Lexer::tokens> Lexer::findMatch(string str, vector<Lexer::tokens> & myRegex)
{
	vector<tokens> match;
	for (int j = 0; j < myRegex.size(); ++j)
	{
		tr1::regex rx(myRegex[j].rx);
		if (tr1::regex_match(str.begin(), str.end(), rx))
			match.push_back(myRegex[j]);
	}
	return match;
}

void Lexer::eraseWhitespaces(string & str)
{
	char space = ' ';
	char tab = '	';
	char newLine = '\n';
	while (str[0] == space || str[0] == tab || str[0] == newLine)
		str.erase(0, 1);
}

string Lexer::readSourceCodeFromFile(string fname)
{
	ifstream file(fname);
	string code;

	if (!file.is_open())
	{
		cout << "File with source code can't be opened!\n";
		exit(0);
	}
	else
	{
		while (!file.eof())
		{
			string buffer;
			getline(file, buffer);
			code = code + " " + buffer;
		}
		file.close();
	}

	return code;
}
