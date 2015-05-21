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
		errInfo err = parser(ret, &ret2);
		if (err.err)
		{
			cout << err.lineN << ':' << endl;
			if (err.lineN < static_cast<int>(lines.size()))
				cout << lines[err.lineN - 1];
			cout << endl;
			for (; err.pos > 1; err.pos--)
				cout << ' ';
			cout << '^' << endl;
			cout << err.info << endl;
		}
		else
		{
			iCodeSeq ret3;
			err = inter(ret2, ret3, dataType(dataType::VOID, 0));
			if (err.err)
			{
				cout << err.lineN << ':' << endl;
				cout << lines[err.lineN - 1] << endl;
				for (; err.pos > 1; err.pos--)
					cout << ' ';
				cout << '^' << endl;
				cout << err.info << endl;
			}
		}
	}
#ifdef WIN32
	system("pause");
#endif
	return 0;
}
