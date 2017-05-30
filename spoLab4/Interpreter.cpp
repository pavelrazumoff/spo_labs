#include "Interpreter.h"

vector<string> Interpreter::get_reverse_polish_notation(vector<token_pair> code)
{
	vector<string> revpn;

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
					revpn.push_back(code[i].value);
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
									revpn.push_back(buffer.back().value);
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
								revpn.push_back(buffer.back().value);
								buffer.pop_back();
							}
							else
								buffer.push_back(code[i]);
						}

						if (code[i].value == ")")
						{
							while (buffer.back().value != "(")
							{
								revpn.push_back(buffer.back().value);
								buffer.pop_back();
							}

							if (buffer.back().value == "(")
								buffer.pop_back();
							
							if (buffer2.back().value == "if" && buffer2.size() > labels.size())
							{
								labels.push_back(revpn.size());
								revpn.push_back("void_label");
								revpn.push_back("!F");
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
									revpn[labels.back()] = "@" + to_string(revpn.size() + 1);
									labels.pop_back();
									buffer1.pop_back();
									buffer2.pop_back();
								}
								else
								{
									if (buffer1.back().value == "{" && buffer2.back().name == "KW_FOR")
									{
										revpn.push_back("@" + to_string(labels.back() + 1));
										revpn.push_back("!");
										labels.pop_back();
										buffer1.pop_back();
										buffer2.pop_back();

										revpn[labels.back()] = "@" + to_string(revpn.size() + 1);
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
								revpn.push_back(buffer.back().value);
								buffer.pop_back();
							}

							while (!buffer1.empty() && buffer1.back().value != "{")
							{
								revpn.push_back(buffer1.back().value);
								buffer1.pop_back();
							}

							if (!buffer2.empty())
							{
								if (buffer2.back().name == "KW_FOR")
								{
									if (buffer2.size() > labels.size())
										labels.push_back(revpn.size());
									else
									{
										if (buffer2.size() == labels.size())
										{
											if (buffer2.back().value == "for")
											{
												buffer2.push_back({ "KW_IF", "if" });
												labels.push_back(revpn.size());
												revpn.push_back("void_label");
												revpn.push_back("!F");
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
								revpn.push_back(buffer.back().value);
								buffer.pop_back();
							}
							while (!buffer2.empty())
							{
								if (buffer2.back().name == "KW_IF")
								{
									revpn[labels.back()] = "@" + to_string(revpn.size() + 1);
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

	return revpn;
}

vector<string> Interpreter::getSimpleRPN(vector<token_pair> code)
{
	vector<string> rpn;
	vector<token_pair> buffer;
	code.push_back({ "end", "end" });

	for (int i = 0; i < code.size(); ++i)
	{
		if (code[i].name == "DIGIT" || code[i].name == "VAR")
			rpn.push_back(code[i].value);
		else
		{
			if (buffer.empty())
				buffer.push_back(code[i]);
			else
			{
				if (code[i].value == "+" || code[i].value == "-" || code[i].value == ">" || code[i].value == "<" || code[i].value == "==" || code[i].value == "!=")
				{
					while (!buffer.empty())
					{
						if (buffer.back().name == "BIN_ARITHM_OPER")
						{
							rpn.push_back(buffer.back().value);
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
						rpn.push_back(buffer.back().value);
						buffer.pop_back();
					}
					else
						buffer.push_back(code[i]);
				}

				if (code[i].value == ")")
				{
					while (buffer.back().value != "(")
					{
						rpn.push_back(buffer.back().value);
						buffer.pop_back();
					}

					if (buffer.back().value == "(")
						buffer.pop_back();
				}

				if (code[i].value == "(")
					buffer.push_back(code[i]);

				if (code[i].value == "end")
				{
					while (!buffer.empty())
					{
						rpn.push_back(buffer.back().value);
						buffer.pop_back();
					}
				}
			}
		}
	}

	return rpn;
}