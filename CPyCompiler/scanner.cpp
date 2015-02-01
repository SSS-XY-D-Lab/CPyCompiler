#include "stdafx.h"
#include "scanner.h"

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
	std::string::iterator p, pEnd = str->end();
	for (p = str->begin(); p != pEnd; p++)
	{
		switch (*p)
		{
			case '#':
				p = pEnd;
				break;
			case ';':
				tList->push_back(new token::delim(-1));
				break;
			case '(':
				tList->push_back(new token::op(token::ops::opType::BRACKET_LEFT));
				break;
			case ')':
				tList->push_back(new token::op(token::ops::opType::BRACKET_RIGHT));
				break;
			case '[':
				tList->push_back(new token::op(token::ops::opType::SUB_LEFT));
				break;
			case ']':
				tList->push_back(new token::op(token::ops::opType::SUB_RIGHT));
				break;
			case '{':
				tList->push_back(new token::op(token::ops::opType::BRACE_LEFT));
				break;
			case '}':
				tList->push_back(new token::op(token::ops::opType::BRACE_RIGHT));
				break;
			case ',':
				tList->push_back(new token::op(token::ops::opType::COMMA));
				break;
			case '.':
				tList->push_back(new token::op(token::ops::opType::MEMBER));
				break;
			case '~':
				tList->push_back(new token::op(token::ops::opType::NOT));
				break;
			case '+':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList->push_back(new token::op(token::ops::opType::ADDASS));
					else if (*p == '+')
						tList->push_back(new token::op(token::ops::opType::INC));
					else if (isdigit(*p))
					{
						p--;
						goto number_process;
					}
					else
					{
						p--;
						if (tList->empty())
							tList->push_back(new token::op(token::ops::opType::POSI));
						else
						{
							switch (tList->back()->getType())
							{
								case token::type::ID:
								case token::type::NUMBER:
								case token::type::CHARA:
								case token::type::STR:
									tList->push_back(new token::op(token::ops::opType::ADD));
									break;
								default:
									tList->push_back(new token::op(token::ops::opType::POSI));
							}
						}
					}
				}
				else
				{
					tList->push_back(new token::op(token::ops::opType::ADD));
					p--;
				}
				break;
			case '-':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList->push_back(new token::op(token::ops::opType::SUBASS));
					else if (*p == '-')
						tList->push_back(new token::op(token::ops::opType::DEC));
					else if (isdigit(*p))
					{
						p--;
						goto number_process;
					}
					else
					{
						p--;
						if (tList->empty())
							tList->push_back(new token::op(token::ops::opType::NEGA));
						else
						{
							switch (tList->back()->getType())
							{
								case token::type::ID:
								case token::type::NUMBER:
								case token::type::CHARA:
								case token::type::STR:
									tList->push_back(new token::op(token::ops::opType::SUB));
									break;
								default:
									tList->push_back(new token::op(token::ops::opType::NEGA));
							}
						}
					}
				}
				else
				{
					tList->push_back(new token::op(token::ops::opType::SUB));
					p--;
				}
				break;
			case '*':
				p++;
				if (p != pEnd && *p == '=')
					tList->push_back(new token::op(token::ops::opType::MULASS));
				else
				{
					p--;
					if (tList->empty())
						tList->push_back(new token::op(token::ops::opType::DEREF));
					else
					{
						switch (tList->back()->getType())
						{
							case token::type::ID:
							case token::type::NUMBER:
							case token::type::CHARA:
							case token::type::STR:
								tList->push_back(new token::op(token::ops::opType::MUL));
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
										break;
									default:
										tList->push_back(new token::op(token::ops::opType::DEREF));
								}
								break;
							}
							default:
								tList->push_back(new token::op(token::ops::opType::DEREF));
						}
					}
				}
				break;
			case '/':
				p++;
				if (p != pEnd && *p == '=')
					tList->push_back(new token::op(token::ops::opType::DIVASS));
				else
				{
					tList->push_back(new token::op(token::ops::opType::DIV));
					p--;
				}
				break;
			case '%':
				p++;
				if (p != pEnd && *p == '=')
					tList->push_back(new token::op(token::ops::opType::MODASS));
				else
				{
					tList->push_back(new token::op(token::ops::opType::MOD));
					p--;
				}
				break;
			case '=':
				p++;
				if (p != pEnd && *p == '=')
					tList->push_back(new token::op(token::ops::opType::EQU));
				else
				{
					tList->push_back(new token::op(token::ops::opType::ASSIGN));
					p--;
				}
				break;
			case '!':
				p++;
				if (p != pEnd && *p == '=')
					tList->push_back(new token::op(token::ops::opType::NEQU));
				else
				{
					tList->push_back(new token::op(token::ops::opType::LGNOT));
					p--;
				}
				break;
			case '&':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList->push_back(new token::op(token::ops::opType::ANDASS));
					else if (*p == '&')
						tList->push_back(new token::op(token::ops::opType::LGAND));
					else
					{
						p--;
						if (tList->empty())
						{
							std::string::iterator pItr, pEnd = str->end();
							int i = 0;
							for (pItr = str->begin(); pItr != pEnd && pItr != p; pItr++)
								i++;
							return i;
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
									break;
								default:
									tList->push_back(new token::op(token::ops::opType::REF));
							}
						}
					}
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
							break;
						default:
							tList->push_back(new token::op(token::ops::opType::REF));
					}
					p--;
				}
				break;
			case '|':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList->push_back(new token::op(token::ops::opType::BORASS));
					else if (*p == '|')
						tList->push_back(new token::op(token::ops::opType::LGOR));
					else
					{
						tList->push_back(new token::op(token::ops::opType::BOR));
						p--;
					}
				}
				else
				{
					tList->push_back(new token::op(token::ops::opType::BOR));
					p--;
				}
				break;
			case '^':
				p++;
				if (p != pEnd && *p == '=')
					tList->push_back(new token::op(token::ops::opType::XORASS));
				else
				{
					tList->push_back(new token::op(token::ops::opType::XOR));
					p--;
				}
				break;
			case '>':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList->push_back(new token::op(token::ops::opType::BIGEQU));
					else if (*p == '>')
					{
						p++;
						if (p != pEnd && *p == '=')
							tList->push_back(new token::op(token::ops::opType::SHRASS));
						else
						{
							tList->push_back(new token::op(token::ops::opType::SHR));
							p--;
						}
					}
					else
					{
						tList->push_back(new token::op(token::ops::opType::BIG));
						p--;
					}
				}
				else
				{
					tList->push_back(new token::op(token::ops::opType::BIG));
					p--;
				}
				break;
			case '<':
				p++;
				if (p != pEnd)
				{
					if (*p == '=')
						tList->push_back(new token::op(token::ops::opType::LESEQU));
					else if (*p == '<')
					{
						p++;
						if (p != pEnd && *p == '=')
							tList->push_back(new token::op(token::ops::opType::SHLASS));
						else
						{
							tList->push_back(new token::op(token::ops::opType::SHL));
							p--;
						}
					}
					else
					{
						tList->push_back(new token::op(token::ops::opType::LES));
						p--;
					}
				}
				else
				{
					tList->push_back(new token::op(token::ops::opType::LES));
					p--;
				}
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
					tList->push_back(new token::chara(_ch));
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
				tList->push_back(new token::str(_str));
				break;
			}
			case ' ':
			case '\t':
				break;
			default:
				if (isalpha(*p) || (*p) == '_')
				{
					std::string token;
					for (; p != pEnd && (isalnum(*p) || (*p) == '_'); p++)
						token.push_back(*p);
					token::keywords::keywords kw = token::keywords::str2Kw(token);
					if (kw == token::keywords::keywords::ERROR)
						tList->push_back(new token::id(token));
					else
						tList->push_back(new token::keyword(kw));
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
						if (p != pEnd)
						{
							ary = 8;
							if (tolower(*p) == 'x')
							{
								token.push_back(*p);
								p++;
								ary = 16;
							}
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
					tList->push_back(new token::number(token));
					p--;
				}
				else
				{
					std::string::iterator pItr, pEnd = str->end();
					int i = 0;
					for (pItr = str->begin(); pItr != pEnd && pItr != p; pItr++)
						i++;
					return i;
				}
		}
		if (p == pEnd)
			break;
	}
	tList->push_back(new token::delim(lineN));
	return -1;
}