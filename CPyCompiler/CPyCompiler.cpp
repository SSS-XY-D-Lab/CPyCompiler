// CPyCompiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lexer.h"
#include "parser.h"
#include "inter.h"
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
		tokenList::const_iterator pT, pTEnd = ret.cend();
		for (pT = ret.cbegin(); pT != pTEnd; pT++)
		{
			switch ((*pT)->getType())
			{
				case token::type::ID:
				{
					token::id* tk = static_cast<token::id*>(*pT);
					cout << "ID:" << tk->str << endl;
					break;
				}
				case token::type::CHARA:
				{
					token::chara* tk = static_cast<token::chara*>(*pT);
					cout << "CHAR:" << tk->ch << endl;
					break;
				}
				case token::type::STR:
				{
					token::str* tk = static_cast<token::str*>(*pT);
					cout << "STRING:" << tk->strr << endl;
					break;
				}
				case token::type::OP:
				{
					token::op* tk = static_cast<token::op*>(*pT);
					cout << "OPERATOR:" << token::ops::op2Str(tk->opType) << endl;
					break;
				}
				case token::type::NUMBER:
				{
					token::number* tk = static_cast<token::number*>(*pT);
					cout << "NUMBER:" << tk->val << endl;
					break;
				}
				case token::type::KEYWORD:
				{
					token::keyword* tk = static_cast<token::keyword*>(*pT);
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
		else
		{
			iCodeSeq ret3;
			errInfo err = inter(ret2, ret3);
		}
	}
#ifdef WIN32
	system("pause");
#endif
	return 0;
}
