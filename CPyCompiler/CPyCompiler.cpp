// CPyCompiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "scanner.h"
#include "parser.h"
using namespace std;

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
		pos = scanner(&str, &ret, lineN);
		if (pos != -1)
		{
			cout << str << endl;
			for (; pos > 0; pos--)
				cout << ' ';
			cout << '^' << endl;
			break;
		}
		/*
		pos = parser(ret, &ret2);
		if (pos != -1)
		{
			cout << str << endl;
			for (; pos > 0; pos--)
				cout << ' ';
			cout << '^' << endl;
			break;
		}
		*/
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
	}
	system("pause");
	return 0;
}

