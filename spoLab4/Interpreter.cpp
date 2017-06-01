#include "interpreter.h"

void Interpreter::calc_reverse_polish_notation(vector<token_pair> code)
{
	vector<token_pair> buffer;
	vector<token_pair> buffer1;
	vector<token_pair> buffer2;

	vector<int> labels;
	code.push_back({ "end", "end" });

	for (int i = 0; i < code.size(); ++i)
	{
		if (i == 57)
			int s = 13;

		if (code[i].name == "KW_IF" || code[i].name == "KW_FOR")
			buffer2.push_back(code[i]);
		else
		{
			if (code[i].name == "LBRACE")
				buffer1.push_back(code[i]);
			else
			{
				if (code[i].name == "DIGIT" || code[i].name == "VAR" || code[i].name == "UNA_ARITHM_OPER")
					revPolNotation.push_back(code[i].value);
				else
				{
					if (buffer.empty() && code[i].value != "=" && code[i].value != "}" && code[i].value != ";" && code[i].value != "end")
						buffer.push_back(code[i]);
					else
					{
						if (code[i].value == "+" || code[i].value == "-" || code[i].value == ">" || code[i].value == "<" || code[i].value == "==" || code[i].value == "!=")
						{
							while (!buffer.empty())
							{
								if (buffer.back().name == "BIN_ARITHM_OPER")
								{
									revPolNotation.push_back(buffer.back().value);
									buffer.pop_back();
								}
								else break;
							}
							buffer.push_back(code[i]);
						}

						if (code[i].value == "*" || code[i].value == "/")
						{
							if (buffer.back().value == "*" || buffer.back().value == "/")
							{
								revPolNotation.push_back(buffer.back().value);
								buffer.pop_back();
							}
							else
								buffer.push_back(code[i]);
						}

						if (code[i].value == ")")
						{
							while (buffer.back().value != "(")
							{
								revPolNotation.push_back(buffer.back().value);
								buffer.pop_back();
							}

							if (buffer.back().value == "(")
								buffer.pop_back();
							
							if (buffer2.back().value == "if" && buffer2.size() > labels.size())
							{
								labels.push_back(revPolNotation.size());
								revPolNotation.push_back("void_label");
								revPolNotation.push_back("!F");
							}

							if (!buffer1.empty() && buffer1.back().value != "{")
								buffer1.pop_back();
						}

						if (code[i].value == "(")
							buffer.push_back(code[i]);

						if (code[i].value == "}")
						{
							if (!buffer1.empty())
							{
								if (buffer1.back().value == "{" && buffer2.back().name == "KW_IF")
								{
									revPolNotation[labels.back()] = "@" + to_string(revPolNotation.size() + 1);
									labels.pop_back();
									buffer1.pop_back();
									buffer2.pop_back();
								}
								else
								{
									if (buffer1.back().value == "{" && buffer2.back().name == "KW_FOR")
									{
										revPolNotation.push_back("@" + to_string(labels.back() + 1));
										revPolNotation.push_back("!");
										labels.pop_back();
										buffer1.pop_back();
										buffer2.pop_back();

										revPolNotation[labels.back()] = "@" + to_string(revPolNotation.size() + 1);
										labels.pop_back();
										buffer1.pop_back();
										buffer2.pop_back();
									}
								}
							}
						}

						if (code[i].value == "=")
							buffer1.push_back(code[i]);

						if (code[i].value == ";")
						{
							while (!buffer.empty() && buffer.back().value != "(")
							{
								revPolNotation.push_back(buffer.back().value);
								buffer.pop_back();
							}

							while (!buffer1.empty() && buffer1.back().value != "{")
							{
								revPolNotation.push_back(buffer1.back().value);
								buffer1.pop_back();
							}

							if (!buffer2.empty())
							{
								if (buffer2.back().name == "KW_FOR")
								{
									if (buffer2.size() > labels.size())
										labels.push_back(revPolNotation.size());
									else
									{
										if (buffer2.size() == labels.size())
										{
											if (buffer2.back().value == "for")
											{
												buffer2.push_back({ "KW_IF", "if" });
												labels.push_back(revPolNotation.size());
												revPolNotation.push_back("void_label");
												revPolNotation.push_back("!F");
											}
										}
									}
								}
							}
						}

						if (code[i].value == "end")
						{
							while (!buffer.empty())
							{
								revPolNotation.push_back(buffer.back().value);
								buffer.pop_back();
							}
							while (!buffer2.empty())
							{
								if (buffer2.back().name == "KW_IF")
								{
									revPolNotation[labels.back()] = "@" + to_string(revPolNotation.size() + 1);
									labels.pop_back();
									buffer2.pop_back();
								}
							}
						}
					}
				}
			}
		}
	}
}

void Interpreter::printRPN()
{
	cout << "\n";
	for (int i = 0; i < revPolNotation.size(); ++i)
		cout << i << " " << revPolNotation[i] << "\n";
}