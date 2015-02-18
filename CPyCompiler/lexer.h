#pragma once

#ifndef _H_SCANNER
#define _H_SCANNER

#include <list>
#include <string>
#include <sstream>

namespace token
{
	enum type{ ERROR, NUMBER, CHARA, STR, ID, OP, KEYWORD, BEGIN, DELIM };

	class token
	{
	public:
		int pos;
		virtual type getType() { return type::ERROR; };
	};

	class number :public token
	{
	public:
		//number(){};
		number(std::string);
		number(long long _val){ val = _val; };
		long long val;
		type getType() { return type::NUMBER; };
	private:
		int toNum(std::string str, long long &ret, int type = 0);
	};

	class chara :public token
	{
	public:
		//chara(){};
		chara(char _ch){ ch = _ch; };
		char ch;
		type getType(){ return type::CHARA; };
	};

	class str :public token
	{
	public:
		//str(){};
		str(std::string _str){ strr = _str; };
		str(char *_str){ strr = _str; };
		std::string strr;
		type getType(){ return type::STR; };
	};

	class id :public token
	{
	public:
		//id(){};
		id(std::string _str){ str = _str; };
		std::string str;
		type getType() { return type::ID; };
	};

	namespace ops
	{
		enum opType{
			ERROR,
			COMMA, SUB_LEFT, SUB_RIGHT, BRACKET_LEFT, BRACKET_RIGHT,
			BRACE_LEFT, BRACE_RIGHT,
			MEMBER,
			POSI, NEGA, INC, DEC, REF, DEREF, NOT, LGNOT,
			DIV, MUL, MOD,
			ADD, SUB,
			SHL, SHR,
			BIG, BIGEQU, LES, LESEQU,
			EQU, NEQU,
			AND, XOR, BOR, LGAND, LGOR,
			ASSIGN, MODASS, DIVASS, MULASS, ADDASS, SUBASS, SHLASS, SHRASS, ANDASS, XORASS, BORASS,
			QMARK, COLON
		};

		struct opItem
		{
			opType val;
			std::string str;
		};

		std::string op2Str(opType op);
	}

	class op :public token
	{
	public:
		op(){};
		//op(std::string _str);
		op(ops::opType _opType){ opType = _opType; };
		ops::opType opType;
		type getType() { return type::OP; };
	};

	namespace keywords
	{
		enum keywords{ ERROR, SINT, S8, S16, S32, S64, UINT, U8, U16, U32, U64, VOID, CONST, DIM, END, FUNCTION, RETURN, IF, ELSE };

		struct keywordItem
		{
			std::string str;
			keywords val;
		};

		std::string kw2Str(keywords kw);
		keywords str2Kw(std::string str);
	}

	class keyword :public token
	{
	public:
		//keyword(){};
		keyword(keywords::keywords _word){ word = _word; };
		keywords::keywords word;
		type getType() { return type::KEYWORD; };
	};

	class begin :public token
	{
	public:
		begin(int _lineN){ lineN = _lineN; };
		int lineN;
		type getType() { return type::BEGIN; };
	};

	class delim :public token
	{
	public:
		delim(int _lineN){ lineN = _lineN; };
		int lineN;
		type getType() { return type::DELIM; };
	};
}
typedef std::list < token::token* > tokenList;

int scanner(std::string *str, tokenList *tList, int lineN);

#endif
