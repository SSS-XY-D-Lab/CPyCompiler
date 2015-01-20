#pragma once

#ifndef _H_ANALYZER
#define _H_ANALYZER

#include "scanner.h"

namespace stnode
{
	enum type{ ERROR, NUMBER, CHARA, STR, ID, OP };

	class stnode
	{
	public:
		virtual type getType() { return type::ERROR; };
	};

	namespace numbers
	{
		enum numType{ ERROR, SINT, S8, S16, S32, S64, UINT, U8, U16, U32, U64 };
	}

	class number:public stnode
	{
	public:
		number(long long _val){ val = _val; };
		long long val;
		type getType() { return type::NUMBER; };
		virtual numbers::numType getNumType(){ return numbers::ERROR; };
	};

	class chara :public stnode
	{
	public:
		chara(char _ch){ ch = _ch; };
		char ch;
		type getType(){ return type::CHARA; };
	};

	class str :public stnode
	{
	public:
		str(std::string _str){ strr = _str; };
		str(char *_str){ strr = _str; };
		std::string strr;
		type getType(){ return type::STR; };
	};

	class id :public stnode
	{
	public:
		id(std::string _str){ str = _str; };
		std::string str;
		type getType() { return type::ID; };
	};

	namespace op
	{
		enum opType{
			ERROR,
			SUB_LEFT, SUB_RIGHT, BRACKET_LEFT, BRACKET_RIGHT, OBJ_MEMBER, PTR_MEMBER,
			POSI, NEGA, INC, DEC, REF, DEREF, NOT, LGNOT,
			DIV, MUL, MOD,
			ADD, SUB,
			SHL, SHR,
			BIG, BIGEQU, LES, LESEQU,
			EQU, NEQU,
			AND, XOR, BOR, LGAND, LGOR,
			ASSIGN, MODASS, DIVASS, MULASS, ADDASS, SUBASS, SHLASS, SHRASS, ANDASS, XORASS, BORASS,
			COMMA
		};

		class op :public stnode
		{
		public:

			type getType(){ return type::OP; };
		};
	}
}

#endif