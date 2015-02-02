#include "stdafx.h"
#include "scanner.h"

#define nextChar p++, pos++
#define prevChar p--, pos--

namespace token
{
	int number::toNum(std::string str, long long &ret, int type)
	{
		static const int numLevel[] = {
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5,
			5, 2, 2, 2, 2, 2, 2, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
		};
		size_t len = str.length();
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
			static const opItem opMap[] = {
				opType::ERROR, "ERROR",
				opType::SUB_LEFT, "SUB_LEFT",
				opType::SUB_RIGHT, "SUB_RIGHT",
				opType::BRACKET_LEFT, "BRACKET_LEFT",
				opType::BRACKET_RIGHT, "BRACKET_RIGHT",
				opType::MEMBER, "MEMBER",
				opType::BRACE_LEFT, "BRACE_LEFT",
				opType::BRACE_RIGHT, "BRACE_RIGHT",
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
				opType::COMMA, "COMMA", 
				opType::QMARK, "QUESTION MARK",
				opType::COLON, "COLON",
			};
			for (int i = 0; i < 48; i++)
				if (op == opMap[i].val)
					return opMap[i].str;
			return "";
		}
	}

	namespace keywords
	{
		std::string kw2Str(keywords kw)
		{
			static const keywordItem keywordMap[] =
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
			static const keywordItem keywordMap[] =
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
}

bool isOCTDigit(char a)
{
	return a >= '0' && a <= '7';
}

int scanner(std::string *str, tokenList *tList, int lineN)
{
	tList->push_back(new token::begin(lineN));
	tList->back()->pos = 0;
	int pos = 1;
	std::string::iterator p, pEnd = str->end();
	for (p = str->begin(); p != pEnd; nextChar)
	{
		switch (*p)
		{
			case '#':
				p = pEnd;
				break;
			case ';':
				tList->push_back(new token::delim(-1));
				tList->back()->pos = pos;
				break;
			case '(':
				tList->push_back(new token::op(token::ops::opType::BRACKET_LEFT));
				tList->back()->pos = pos;
				break;
			case ')':
				tList->push_back(new token::op(token::ops::opType::BRACKET_RIGHT));
				tList->back()->pos = pos;
				break;
			case '[':
				tList->push_back(new token::op(token::ops::opType::SUB_LEFT));
				tList->back()->pos = pos;
				break;
			case ']':
				tList->push_back(new token::op(token::ops::opType::SUB_RIGHT));
				tList->back()->pos = pos;
				break;
			case '{':
				tList->push_back(new token::op(token::ops::opType::BRACE_LEFT));
				tList->back()->pos = pos;
				break;
			case '}':
				tList->push_back(new token::op(token::ops::opType::BRACE_RIGHT));
				tList->back()->pos = pos;
				break;
			case ',':
				tList->push_back(new token::op(token::ops::opType::COMMA));
				tList->back()->pos = pos;
				break;
			case '.':
				tList->push_back(new token::op(token::ops::opType::MEMBER));
				tList->back()->pos = pos;
				break;
			case '~':
				tList->push_back(new token::op(token::ops::opType::NOT));
				tList->back()->pos = pos;
				break;
			case '+':
				nextChar;
				if (p != pEnd)
				{
					if (*p == '=')
					{
						tList->push_back(new token::op(token::ops::opType::ADDASS));
						tList->back()->pos = pos - 1;
					}
					else if (*p == '+')
					{
						tList->push_back(new token::op(token::ops::opType::INC));
						tList->back()->pos = pos - 1;
					}
					else if (isdigit(*p))
					{
						prevChar;
						goto number_process;
					}
					else
					{
						prevChar;
						if (tList->empty())
						{
							tList->push_back(new token::op(token::ops::opType::POSI));
							tList->back()->pos = pos;
						}
						else
						{
							switch (tList->back()->getType())
							{
								case token::type::ID:
								case token::type::NUMBER:
								case token::type::CHARA:
								case token::type::STR:
									tList->push_back(new token::op(token::ops::opType::ADD));
									tList->back()->pos = pos;
									break;
								default:
									tList->push_back(new token::op(token::ops::opType::POSI));
									tList->back()->pos = pos;
							}
						}
					}
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::ADD));
					tList->back()->pos = pos;
				}
				break;
			case '-':
				nextChar;
				if (p != pEnd)
				{
					if (*p == '=')
					{
						tList->push_back(new token::op(token::ops::opType::SUBASS));
						tList->back()->pos = pos - 1;
					}
					else if (*p == '-')
					{
						tList->push_back(new token::op(token::ops::opType::DEC));
						tList->back()->pos = pos - 1;
					}
					else if (isdigit(*p))
					{
						prevChar;
						goto number_process;
					}
					else
					{
						prevChar;
						if (tList->empty())
						{
							tList->push_back(new token::op(token::ops::opType::NEGA));
							tList->back()->pos = pos;
						}
						else
						{
							switch (tList->back()->getType())
							{
								case token::type::ID:
								case token::type::NUMBER:
								case token::type::CHARA:
								case token::type::STR:
								{
									tList->push_back(new token::op(token::ops::opType::SUB));
									tList->back()->pos = pos;
								}
									break;
								default:
								{
									tList->push_back(new token::op(token::ops::opType::NEGA));
									tList->back()->pos = pos;
								}
							}
						}
					}
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::SUB));
					tList->back()->pos = pos;
				}
				break;
			case '*':
				nextChar;
				if (p != pEnd && *p == '=')
				{
					tList->push_back(new token::op(token::ops::opType::MULASS));
					tList->back()->pos = pos - 1;
				}
				else
				{
					prevChar;
					if (tList->empty())
					{
						tList->push_back(new token::op(token::ops::opType::DEREF));
						tList->back()->pos = pos;
					}
					else
					{
						switch (tList->back()->getType())
						{
							case token::type::ID:
							case token::type::NUMBER:
							case token::type::CHARA:
							case token::type::STR:
								tList->push_back(new token::op(token::ops::opType::MUL));
								tList->back()->pos = pos;
								break;
							case token::type::OP:
							{
								token::op* op = dynamic_cast<token::op*>(tList->back());
								switch (op->opType)
								{
									case token::ops::opType::SUB_RIGHT:
									case token::ops::opType::BRACKET_RIGHT:
									case token::ops::opType::INC:
									case token::ops::opType::DEC:
										tList->push_back(new token::op(token::ops::opType::MUL));
										tList->back()->pos = pos;
										break;
									default:
										tList->push_back(new token::op(token::ops::opType::DEREF));
										tList->back()->pos = pos;
								}
								break;
							}
							default:
								tList->push_back(new token::op(token::ops::opType::DEREF));
								tList->back()->pos = pos;
						}
					}
				}
				break;
			case '/':
				nextChar;
				if (p != pEnd && *p == '=')
				{
					tList->push_back(new token::op(token::ops::opType::DIVASS));
					tList->back()->pos = pos - 1;
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::DIV));
					tList->back()->pos = pos;
				}
				break;
			case '%':
				nextChar;
				if (p != pEnd && *p == '=')
				{
					tList->push_back(new token::op(token::ops::opType::MODASS));
					tList->back()->pos = pos - 1;
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::MOD));
					tList->back()->pos = pos;
				}
				break;
			case '=':
				nextChar;
				if (p != pEnd && *p == '=')
				{
					tList->push_back(new token::op(token::ops::opType::EQU));
					tList->back()->pos = pos - 1;
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::ASSIGN));
					tList->back()->pos = pos;
				}
				break;
			case '!':
				nextChar;
				if (p != pEnd && *p == '=')
				{
					tList->push_back(new token::op(token::ops::opType::NEQU));
					tList->back()->pos = pos - 1;
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::LGNOT));
					tList->back()->pos = pos;
				}
				break;
			case '&':
				nextChar;
				if (p != pEnd)
				{
					if (*p == '=')
					{
						tList->push_back(new token::op(token::ops::opType::ANDASS));
						tList->back()->pos = pos - 1;
					}
					else if (*p == '&')
					{
						tList->push_back(new token::op(token::ops::opType::LGAND));
						tList->back()->pos = pos - 1;
					}
					else
					{
						prevChar;
						if (tList->empty())
						{
							tList->push_back(new token::op(token::ops::opType::REF));
							tList->back()->pos = pos;
						}
						else
						{
							switch (tList->back()->getType())
							{
								case token::type::ID:
								case token::type::NUMBER:
								case token::type::CHARA:
								case token::type::STR:
									tList->push_back(new token::op(token::ops::opType::AND));
									tList->back()->pos = pos;
									break;
								case token::type::OP:
								{
									token::op* op = dynamic_cast<token::op*>(tList->back());
									switch (op->opType)
									{
										case token::ops::opType::SUB_RIGHT:
										case token::ops::opType::BRACKET_RIGHT:
										case token::ops::opType::INC:
										case token::ops::opType::DEC:
											tList->push_back(new token::op(token::ops::opType::AND));
											tList->back()->pos = pos;
											break;
										default:
											tList->push_back(new token::op(token::ops::opType::REF));
											tList->back()->pos = pos;
									}
									break;
								}
								default:
									tList->push_back(new token::op(token::ops::opType::REF));
									tList->back()->pos = pos;
							}
						}
					}
				}
				else
				{
					prevChar;
					if (tList->empty())
					{
						tList->push_back(new token::op(token::ops::opType::REF));
						tList->back()->pos = pos;
					}
					else
					{
						switch (tList->back()->getType())
						{
							case token::type::ID:
							case token::type::NUMBER:
							case token::type::CHARA:
							case token::type::STR:
								tList->push_back(new token::op(token::ops::opType::AND));
								tList->back()->pos = pos;
								break;
							case token::type::OP:
							{
								token::op* op = dynamic_cast<token::op*>(tList->back());
								switch (op->opType)
								{
									case token::ops::opType::SUB_RIGHT:
									case token::ops::opType::BRACKET_RIGHT:
									case token::ops::opType::INC:
									case token::ops::opType::DEC:
										tList->push_back(new token::op(token::ops::opType::AND));
										tList->back()->pos = pos;
										break;
									default:
										tList->push_back(new token::op(token::ops::opType::REF));
										tList->back()->pos = pos;
								}
								break;
							}
							default:
								tList->push_back(new token::op(token::ops::opType::REF));
								tList->back()->pos = pos;
						}
					}
				}
				break;
			case '|':
				nextChar;
				if (p != pEnd)
				{
					if (*p == '=')
					{
						tList->push_back(new token::op(token::ops::opType::BORASS));
						tList->back()->pos = pos - 1;
					}
					else if (*p == '|')
					{
						tList->push_back(new token::op(token::ops::opType::LGOR));
						tList->back()->pos = pos - 1;
					}
					else
					{
						prevChar;
						tList->push_back(new token::op(token::ops::opType::BOR));
						tList->back()->pos = pos;
					}
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::BOR));
					tList->back()->pos = pos;
				}
				break;
			case '^':
				nextChar;
				if (p != pEnd && *p == '=')
				{
					tList->push_back(new token::op(token::ops::opType::XORASS));
					tList->back()->pos = pos - 1;
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::XOR));
					tList->back()->pos = pos;
				}
				break;
			case '>':
				nextChar;
				if (p != pEnd)
				{
					if (*p == '=')
					{
						tList->push_back(new token::op(token::ops::opType::BIGEQU));
						tList->back()->pos = pos - 1;
					}
					else if (*p == '>')
					{
						nextChar;
						if (p != pEnd && *p == '=')
						{
							tList->push_back(new token::op(token::ops::opType::SHRASS));
							tList->back()->pos = pos - 2;
						}
						else
						{
							prevChar;
							tList->push_back(new token::op(token::ops::opType::SHR));
							tList->back()->pos = pos - 1;
							
						}
					}
					else
					{
						prevChar;
						tList->push_back(new token::op(token::ops::opType::BIG));
						tList->back()->pos = pos;
					}
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::BIG));
					tList->back()->pos = pos;
				}
				break;
			case '<':
				nextChar;
				if (p != pEnd)
				{
					if (*p == '=')
					{
						tList->push_back(new token::op(token::ops::opType::LESEQU));
						tList->back()->pos = pos - 1;
					}
					else if (*p == '<')
					{
						nextChar;
						if (p != pEnd && *p == '=')
						{
							tList->push_back(new token::op(token::ops::opType::SHLASS));
							tList->back()->pos = pos - 2;
						}
						else
						{
							prevChar;
							tList->push_back(new token::op(token::ops::opType::SHL));
							tList->back()->pos = pos - 1;
						}
					}
					else
					{
						prevChar;
						tList->push_back(new token::op(token::ops::opType::LES));
						tList->back()->pos = pos;
					}
				}
				else
				{
					prevChar;
					tList->push_back(new token::op(token::ops::opType::LES));
					tList->back()->pos = pos;
				}
				break;
			case '\'':
			{
				nextChar;
				if (p == pEnd)
				{
					prevChar;
					break;
				}
				char _ch = *p;
				nextChar;
				if (p == pEnd)
				{
					prevChar;
					prevChar;
					break;
				}
				if ((*p) != '\'')
				{
					prevChar;
					prevChar;
				}
				else
				{
					tList->push_back(new token::chara(_ch));
					tList->back()->pos = pos - 1;
				}
				break;
			}
			case '"':
			{
				std::string _str;
				nextChar;
				int startPos = pos;
				for (; p != pEnd && (*p) != '"'; nextChar)
				{
					_str.push_back(*p);
					if (*p == '\\')
					{
						nextChar;
						_str.push_back(*p);
					}
				}
				if (p == pEnd)
					return pos;
				tList->push_back(new token::str(_str));
				tList->back()->pos = startPos;
				break;
			}
			case ' ':
			case '\t':
				break;
			default:
				if (isalpha(*p) || (*p) == '_')
				{
					std::string token;
					int startPos = pos;
					for (; p != pEnd && (isalnum(*p) || (*p) == '_'); nextChar)
						token.push_back(*p);
					prevChar;
					token::keywords::keywords kw = token::keywords::str2Kw(token);
					if (kw == token::keywords::keywords::ERROR)
					{
						tList->push_back(new token::id(token));
						tList->back()->pos = startPos;
					}
					else
					{
						tList->push_back(new token::keyword(kw));
						tList->back()->pos = startPos;
					}
				}
				else if (isdigit(*p))
				{
				number_process:
					std::string::iterator pTmp = p;
					std::string token;
					int startPos = pos;
					if ((*p) == '+' || (*p) == '-')
					{
						token.push_back(*p);
						nextChar;
					}
					int ary = 10;
					if (*p == '0')
					{
						token.push_back(*p);
						nextChar;
						if (p != pEnd)
						{
							ary = 8;
							if (tolower(*p) == 'x')
							{
								token.push_back(*p);
								nextChar;
								ary = 16;
							}
						}
					}
					switch (ary)
					{
						case 8:
							for (; p != pEnd && isOCTDigit(*p); nextChar)
								token.push_back(*p);
							break;
						case 10:
							for (; p != pEnd && isdigit(*p); nextChar)
								token.push_back(*p);
							break;
						case 16:
							for (; p != pEnd && isxdigit(*p); nextChar)
								token.push_back(*p);
							break;
					}
					if (p != pEnd && (*p) == '.')
					{
						nextChar;
						if (p != pEnd)
						{
							if ((ary == 8 && isOCTDigit(*p) == false) || (ary == 10 && isdigit(*p) == false) || (ary == 16 && isxdigit(*p) == false))
							{
								prevChar;
								prevChar;
							}
							else
							{
								token.push_back('.');
								switch (ary)
								{
									case 8:
										for (; p != pEnd && isOCTDigit(*p); nextChar)
											token.push_back(*p);
										break;
									case 10:
										for (; p != pEnd && isdigit(*p); nextChar)
											token.push_back(*p);
										break;
									case 16:
										for (; p != pEnd && isxdigit(*p); nextChar)
											token.push_back(*p);
										break;
								}
							}
						}
						else
							prevChar;
					}
					if (p != pEnd && (*p) == 'E')
					{
						nextChar;
						if (p != pEnd)
						{
							token.push_back('E');
							if ((*p) == '+' || (*p) == '-')
							{
								token.push_back(*p);
								nextChar;
							}
							if ((ary == 8 && isOCTDigit(*p) == false) || (ary == 10 && isdigit(*p) == false) || (ary == 16 && isxdigit(*p) == false))
							{
								prevChar;
								if ((*p) == '+' || (*p) == '-')
								{
									prevChar;
									token.pop_back();
								}
								prevChar;
								token.pop_back();
							}
							else
							{
								for (; p != pEnd && isdigit(*p); nextChar)
									token.push_back(*p);
							}
						}
						else
							prevChar;
					}
					tList->push_back(new token::number(token));
					tList->back()->pos = startPos;
					prevChar;
				}
				else
				{
					return pos;
				}
		}
		if (p == pEnd)
			break;
	}
	tList->push_back(new token::delim(lineN));
	tList->back()->pos = pos;
	return -1;
}