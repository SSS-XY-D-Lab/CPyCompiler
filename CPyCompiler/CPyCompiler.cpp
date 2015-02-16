// CPyCompiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lexer.h"
#include "parser.h"
#include <vector>
using namespace std;

vector<string> lines;

int main(int argc, char* argv[])
{
	string str;
	tokenList ret;
	stTree ret2;
	int pos = -1;
	int lineN = 1;
	getline(cin, str, '\n');
	while (str != "")
	{
		lines.push_back(str);
		pos = scanner(&str, &ret, lineN);
		if (pos != -1)
		{
			cout << str << endl;
			for (; pos > 1; pos--)
				cout << ' ';
			cout << '^' << endl;
			break;
		}
		lineN++;
		getline(cin, str, '\n');
	}
	if (pos == -1)
	{
		tokenList::const_iterator p, pEnd = ret.cend();
		for (p = ret.cbegin(); p != pEnd; p++)
		{
			switch ((*p)->getType())
			{
				case token::type::ID:
				{
					token::id* tk = dynamic_cast<token::id*>(*p);
					cout << "ID:" << tk->str << endl;
					break;
				}
				case token::type::CHARA:
				{
					token::chara* tk = dynamic_cast<token::chara*>(*p);
					cout << "CHAR:" << tk->ch << endl;
					break;
				}
				case token::type::STR:
				{
					token::str* tk = dynamic_cast<token::str*>(*p);
					cout << "STRING:" << tk->strr << endl;
					break;
				}
				case token::type::OP:
				{
					token::op* tk = dynamic_cast<token::op*>(*p);
					cout << "OPERATOR:" << token::ops::op2Str(tk->opType) << endl;
					break;
				}
				case token::type::NUMBER:
				{
					token::number* tk = dynamic_cast<token::number*>(*p);
					cout << "NUMBER:" << tk->val << endl;
					break;
				}
				case token::type::KEYWORD:
				{
					token::keyword* tk = dynamic_cast<token::keyword*>(*p);
					cout << "KEYWORD:" << token::keywords::kw2Str(tk->word) << endl;
					break;
				}
				case token::type::DELIM:
				{
					cout << "DELIMETER" << endl;
					break;
				}
			}
		}
		errInfo err = parser(ret, &ret2);
		if (err.err != NULL)
		{
			cout << lines[err.lineN - 1] << endl;
			for (; err.pos > 1; err.pos--)
				cout << ' ';
			cout << '^' << endl;
			cout << err.err << endl;
		}
	}
	system("pause");
	return 0;
}
