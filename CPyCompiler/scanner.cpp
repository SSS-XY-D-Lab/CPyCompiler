#include "stdafx.h"
#include "scanner.h"

namespace token
{
	int number::toNum(std::string str, long long &ret, int type)
	{
		static int numLevel[] = {
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5,
			5, 2, 2, 2, 2, 2, 2, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
		};
		int len = str.length();
		if (len < 1)
			return -1;
		std::stringstream ss;
		bool negative = false;
		if (str.front() == '-')
		{
			str.erase(0, 1);
			negative = true;
			len--;
		}
		else if (str.front() == '+')
		{
			str.erase(0, 1);
			len--;
		}
		switch (type)
		{
			case 0:
				if (len == 0)
					return INT_MIN;
				if (str.back() == 'h' || str.back() == 'H')
				{
					str.erase(len - 1, 1);
					ss << std::hex << str;
					ss >> ret;
				}
				else if (str.front() == '0' && len > 1)
				{
					if (len > 2 && str[1] == 'x')
					{
						str.erase(0, 2);
						ss << std::hex << str;
						ss >> ret;
					}
					else
					{
						str.erase(0, 1);
						ss << std::oct << str;
						ss >> ret;
					}
				}
				else if (str.front() == '\'' && str.back() == '\'')
				{
					if (str.length() == 3)
						ret = (int)(str[1]);
				}
				else
				{
					ss << std::dec << str;
					ss >> ret;
				}
				break;
			case 4:
				if (str.front() == '\'' && str.back() == '\'')
					str.erase(0, 1);
				ret = (int)(str[0]);
				break;
			case 3:
				ss << std::hex << str;
				ss >> ret;
				break;
			case 2:
				ss << std::dec << str;
				ss >> ret;
				break;
			case 1:
				ss << std::oct << str;
				ss >> ret;
				break;
		}
		if (negative)
			ret = -ret;
		return 0;
	}

	number::number(std::string _val)
	{
		if (toNum(_val, val) != 0)
			throw("Invalid Number");
	}

	namespace ops
	{
		std::string op2Str(opType op)
		{
			static opItem opMap[] = {
				opType::ERROR, "ERROR",
				opType::SUB_LEFT, "SUB_LEFT",
				opType::SUB_RIGHT, "SUB_RIGHT",
				opType::BRACKET_LEFT, "BRACKET_LEFT",
				opType::BRACKET_RIGHT, "BRACKET_RIGHT",
				opType::OBJ_MEMBER, "OBJ_MEMBER",
				opType::PTR_MEMBER, "PTR_MEMBER",
				opType::POSI, "POSI",
				opType::NEGA, "NEGA",
				opType::INC, "INC",
				opType::DEC, "DEC",
				opType::REF, "REF",
				opType::DEREF, "DEREF",
				opType::NOT, "NOT",
				opType::LGNOT, "LGNOT",
				opType::DIV, "DIV",
				opType::MUL, "MUL",
				opType::MOD, "MOD",
				opType::ADD, "ADD",
				opType::SUB, "SUB",
				opType::SHL, "SHL",
				opType::SHR, "SHR",
				opType::BIG, "BIG",
				opType::BIGEQU, "BIGEQU",
				opType::LES, "LES",
				opType::LESEQU, "LESEQU",
				opType::EQU, "EQU",
				opType::NEQU, "NEQU",
				opType::AND, "AND",
				opType::XOR, "XOR",
				opType::BOR, "BOR",
				opType::LGAND, "LGAND",
				opType::LGOR, "LGOR",
				opType::ASSIGN, "ASSIGN",
				opType::MODASS, "MODASS",
				opType::DIVASS, "DIVASS",
				opType::MULASS, "MULASS",
				opType::ADDASS, "ADDASS",
				opType::SUBASS, "SUBASS",
				opType::SHLASS, "SHLASS",
				opType::SHRASS, "SHRASS",
				opType::ANDASS, "ANDASS",
				opType::XORASS, "XORASS",
				opType::BORASS, "BORASS",
				opType::COMMA, "COMMA", };
			for (int i = 0; i < 45; i++)
				if (op == opMap[i].val)
					return opMap[i].str;
			return "";
		}
	}

	namespace keywords
	{
		std::string kw2Str(keywords kw)
		{
			static keywordItem keywordMap[] =
			{
				"sint", keywords::SINT,
				"s8", keywords::S8,
				"s16", keywords::S16,
				"s32", keywords::S32,
				"s64", keywords::S64,
				"uint", keywords::UINT,
				"u8", keywords::U8,
				"u16", keywords::U16,
				"u32", keywords::U32,
				"u64", keywords::U64,
				"void", keywords::VOID,
				"const", keywords::CONST,
				"dim", keywords::DIM,
				"end", keywords::END,
				"function", keywords::FUNCTION,
				"return", keywords::RETURN,
				"if", keywords::IF,
				"else", keywords::ELSE,
			};
			for (int i = 0; i < 27; i++)
				if (kw == keywordMap[i].val)
					return keywordMap[i].str;
			return "";
		}

		keywords str2Kw(std::string str)
		{
			static keywordItem keywordMap[] =
			{
				"sint", keywords::SINT,
				"s8", keywords::S8,
				"s16", keywords::S16,
				"s32", keywords::S32,
				"s64", keywords::S64,
				"uint", keywords::UINT,
				"u8", keywords::U8,
				"u16", keywords::U16,
				"u32", keywords::U32,
				"u64", keywords::U64,
				"void", keywords::VOID,
				"const", keywords::CONST,
				"dim", keywords::DIM,
				"end", keywords::END,
				"function", keywords::FUNCTION,
				"return", keywords::RETURN,
				"if", keywords::IF,
				"else", keywords::ELSE,
				"byte", keywords::S8,
				"char", keywords::S8,
				"dword", keywords::S32,
				"int", keywords::SINT,
				"llong", keywords::S64,
				"long", keywords::S32,
				"qword", keywords::S64,
				"short", keywords::S16,
				"ubyte", keywords::U8,
				"uchar", keywords::U8,
				"udword", keywords::U32,
				"ullong", keywords::U64,
				"ulong", keywords::U32,
				"uqword", keywords::U64,
				"ushort", keywords::U16,
				"uword", keywords::U16,
				"word", keywords::S16,
			};
			for (int i = 0; i < 27; i++)
				if (str == keywordMap[i].str)
					return keywordMap[i].val;
			return keywords::ERROR;
		}
	}

	keyword::keyword(std::string _keyword)
	{
		word = keywords::str2Kw(_keyword);
		if (word == keywords::keywords::ERROR)
			throw(NotAKeyWord());
	}
}

bool isOCTDigit(char a)
{
	return a >= '0' && a <= '7';
}

tokenList scanner(std::string str)
{
	tokenList tList;
	std::string::iterator p, pEnd = str.end();
	for (p = str.begin(); p != pEnd; p++)
	{
		switch (*p)
		{
			case '#':
				p = pEnd;
				break;
			case '(':
				tList.push_back(new token::op(token::ops::opType::BRACKET_LEFT));
				break;
			case ')':
				tList.push_back(new token::op(token::ops::opType::BRACKET_RIGHT));
				break;
			case '[':
				tList.push_back(new token::op(token::ops::opType::SUB_LEFT));
				break;
			case ']':
				tList.push_back(new token::op(token::ops::opType::SUB_RIGHT));
				break;
			case ',':
				tList.push_back(new token::op(token::ops::opType::COMMA));
				break;
			case '~':
				tList.push_back(new token::op(token::ops::opType::NOT));
				break;
			case '+':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList.push_back(new token::op(token::ops::opType::ADDASS));
					else if (*p == '+')
						tList.push_back(new token::op(token::ops::opType::INC));
					else if (isdigit(*p))
					{
						p--;
						goto number_process;
					}
					else
					{
						tList.push_back(new token::op(token::ops::opType::ADD));
						p--;
					}
				}
				else
				{
					tList.push_back(new token::op(token::ops::opType::ADD));
					p--;
				}
				break;
			case '-':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList.push_back(new token::op(token::ops::opType::SUBASS));
					else if (*p == '-')
						tList.push_back(new token::op(token::ops::opType::DEC));
					else if (isxdigit(*p))
					{
						p--;
						goto number_process;
					}
					else
					{
						tList.push_back(new token::op(token::ops::opType::SUB));
						p--;
					}
				}
				else
				{
					tList.push_back(new token::op(token::ops::opType::SUB));
					p--;
				}
				break;
			case '*':
				p++;
				if (p != pEnd && *p == '=')
					tList.push_back(new token::op(token::ops::opType::MULASS));
				else
				{
					p--;
					switch (tList.back()->getType())
					{
						case token::type::ID:
						case token::type::NUMBER:
						case token::type::CHARA:
						case token::type::STR:
							tList.push_back(new token::op(token::ops::opType::MUL));
							break;
						default:
							tList.push_back(new token::op(token::ops::opType::DEREF));
					}
				}
				break;
			case '/':
				p++;
				if (p != pEnd && *p == '=')
					tList.push_back(new token::op(token::ops::opType::DIVASS));
				else
				{
					tList.push_back(new token::op(token::ops::opType::DIV));
					p--;
				}
				break;
			case '%':
				p++;
				if (p != pEnd && *p == '=')
					tList.push_back(new token::op(token::ops::opType::MODASS));
				else
				{
					tList.push_back(new token::op(token::ops::opType::MOD));
					p--;
				}
				break;
			case '=':
				p++;
				if (p != pEnd && *p == '=')
					tList.push_back(new token::op(token::ops::opType::EQU));
				else
				{
					tList.push_back(new token::op(token::ops::opType::ASSIGN));
					p--;
				}
				break;
			case '!':
				p++;
				if (p != pEnd && *p == '=')
					tList.push_back(new token::op(token::ops::opType::NEQU));
				else
				{
					tList.push_back(new token::op(token::ops::opType::LGNOT));
					p--;
				}
				break;
			case '&':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList.push_back(new token::op(token::ops::opType::ANDASS));
					else if (*p == '&')
						tList.push_back(new token::op(token::ops::opType::LGAND));
					else
					{
						switch (tList.back()->getType())
						{
							case token::type::ID:
							case token::type::NUMBER:
							case token::type::CHARA:
							case token::type::STR:
								tList.push_back(new token::op(token::ops::opType::AND));
								break;
							default:
								tList.push_back(new token::op(token::ops::opType::REF));
						}
						p--;
					}
				}
				else
				{
					switch (tList.back()->getType())
					{
						case token::type::ID:
						case token::type::NUMBER:
						case token::type::CHARA:
						case token::type::STR:
							tList.push_back(new token::op(token::ops::opType::AND));
							break;
						default:
							tList.push_back(new token::op(token::ops::opType::REF));
					}
					p--;
				}
				break;
			case '|':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList.push_back(new token::op(token::ops::opType::BORASS));
					else if (*p == '|')
						tList.push_back(new token::op(token::ops::opType::LGOR));
					else
					{
						tList.push_back(new token::op(token::ops::opType::BOR));
						p--;
					}
				}
				else
				{
					tList.push_back(new token::op(token::ops::opType::BOR));
					p--;
				}
				break;
			case '^':
				p++;
				if (p != pEnd && *p == '=')
					tList.push_back(new token::op(token::ops::opType::XORASS));
				else
				{
					tList.push_back(new token::op(token::ops::opType::XOR));
					p--;
				}
				break;
			case '>':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList.push_back(new token::op(token::ops::opType::BIGEQU));
					else if (*p == '>')
					{
						p++;
						if (p != pEnd && *p == '=')
							tList.push_back(new token::op(token::ops::opType::SHRASS));
						else
						{
							tList.push_back(new token::op(token::ops::opType::SHR));
							p--;
						}
					}
					else
					{
						tList.push_back(new token::op(token::ops::opType::BIG));
						p--;
					}
				}
				else
				{
					tList.push_back(new token::op(token::ops::opType::BIG));
					p--;
				}
				break;
			case '<':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList.push_back(new token::op(token::ops::opType::LESEQU));
					else if (*p == '<')
					{
						p++;
						if (p != pEnd && *p == '=')
							tList.push_back(new token::op(token::ops::opType::SHLASS));
						else
						{
							tList.push_back(new token::op(token::ops::opType::SHL));
							p--;
						}
					}
					else
					{
						tList.push_back(new token::op(token::ops::opType::LES));
						p--;
					}
				}
				else
				{
					tList.push_back(new token::op(token::ops::opType::LES));
					p--;
				}
				break;
			case ';':
				tList.push_back(new token::delim());
				break;
			case '\'':
			{
				p++;
				if (p == pEnd)
				{
					p--;
					break;
				}
				char _ch = *p;
				p++;
				if (p == pEnd)
				{
					p--;
					p--;
					break;
				}
				if ((*p) != '\'')
				{
					p--;
					p--;
				}
				else
					tList.push_back(new token::chara(_ch));
				break;
			}
			case '"':
			{
				std::string _str;
				p++;
				for (; p != pEnd && (*p) != '"'; p++)
				{
					_str.push_back(*p);
					if (*p == '\\')
					{
						p++;
						_str.push_back(*p);
					}
				}
				tList.push_back(new token::str(_str));
				break;
			}
			case ' ':
				break;
			default:
				if (isalpha(*p) || (*p) == '_')
				{
					std::string token;
					for (; p != pEnd && (isalnum(*p) || (*p) == '_'); p++)
						token.push_back(*p);
					try
					{
						tList.push_back(new token::keyword(token));
					}
					catch (token::NotAKeyWord)
					{
						tList.push_back(new token::id(token));
					}
					p--;
				}
				else if (isdigit(*p))
				{
				number_process:
					std::string::iterator pTmp = p;
					std::string token;
					if ((*p) == '+' || (*p) == '-')
					{
						token.push_back(*p);
						p++;
					}
					int ary = 10;
					if (*p == '0')
					{
						token.push_back(*p);
						p++;
						ary = 8;
						if (tolower(*p) == 'x')
						{
							token.push_back(*p);
							p++;
							ary = 16;
						}
					}
					switch (ary)
					{
						case 8:
							for (; p != pEnd && isOCTDigit(*p); p++)
								token.push_back(*p);
							break;
						case 10:
							for (; p != pEnd && isdigit(*p); p++)
								token.push_back(*p);
							break;
						case 16:
							for (; p != pEnd && isxdigit(*p); p++)
								token.push_back(*p);
							break;
					}
					if (p != pEnd && (*p) == '.')
					{
						p++;
						if (p != pEnd)
						{
							if ((ary == 8 && isOCTDigit(*p) == false) || (ary == 10 && isdigit(*p) == false) || (ary == 16 && isxdigit(*p) == false))
							{
								p--;
								p--;
							}
							else
							{
								token.push_back('.');
								switch (ary)
								{
									case 8:
										for (; p != pEnd && isOCTDigit(*p); p++)
											token.push_back(*p);
										break;
									case 10:
										for (; p != pEnd && isdigit(*p); p++)
											token.push_back(*p);
										break;
									case 16:
										for (; p != pEnd && isxdigit(*p); p++)
											token.push_back(*p);
										break;
								}
							}
						}
						else
							p--;
					}
					if (p != pEnd && (*p) == 'E')
					{
						p++;
						if (p != pEnd)
						{
							token.push_back('E');
							if ((*p) == '+' || (*p) == '-')
							{
								token.push_back(*p);
								p++;
							}
							if ((ary == 8 && isOCTDigit(*p) == false) || (ary == 10 && isdigit(*p) == false) || (ary == 16 && isxdigit(*p) == false))
							{
								p--;
								if ((*p) == '+' || (*p) == '-')
								{
									p--;
									token.pop_back();
								}
								p--;
								token.pop_back();
							}
							else
							{
								for (; p != pEnd && isdigit(*p); p++)
									token.push_back(*p);
							}
						}
						else
							p--;
					}
					tList.push_back(new token::number(token));
					p--;
				}
				else
				{
					std::string::iterator pItr, pEnd = str.end();
					int i = 0;
					for (pItr = str.begin(); pItr != pEnd && pItr != p; pItr++)
						i++;
					throw(i);
				}
		}
		if (p == pEnd)
			break;
	}
	tList.push_back(new token::delim);
	return tList;
}