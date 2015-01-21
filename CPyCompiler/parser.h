#pragma once

#ifndef _H_ANALYZER
#define _H_ANALYZER

#include "scanner.h"

namespace stnode
{
	enum type{ ERROR, NUMBER, CHARA, STR, ID, OP, FUNC, CALL, IF };

	class stnode
	{
	public:
		virtual type getType() { return type::ERROR; };
	};
}

typedef std::list<stnode::stnode> stTree;

namespace stnode
{
	enum varType{ _ERROR, SINT, S8, S16, S32, S64, UINT, U8, U16, U32, U64, SINT_PTR, S8_PTR, S16_PTR, S32_PTR, S64_PTR, UINT_PTR, U8_PTR, U16_PTR, U32_PTR, U64_PTR };

	class number:public stnode
	{
	public:
		number(long long _val){ val = _val; };
		long long val;
		type getType() { return type::NUMBER; };
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
		id(std::string _name, varType _type = varType::_ERROR){ name = _name; dtype = _type; };
		std::string name;
		varType dtype;
		type getType() { return type::ID; };
	};

	namespace op
	{
		enum ops{
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

		enum opType{
			_ERROR, SINGLE, DOUBLE, TRIPLE
		};

		class op :public stnode
		{
		public:
			op(ops _opVal){ opVal = _opVal; };
			ops opVal;
			type getType(){ return type::OP; };
			virtual opType getOpType(){ return opType::_ERROR; };
		};

		class opSingle :public op
		{
		public:
			stnode *arg1;
			opType getOpType(){ return opType::SINGLE; };
		};

		class opDouble :public op
		{
		public:
			stnode *arg1, *arg2;
			opType getOpType(){ return opType::DOUBLE; };
		};
		
		class opTriple :public op
		{
		public:
			stnode *arg1, *arg2, *arg3;
			opType getOpType(){ return opType::TRIPLE; };
		};
	}

	class func :public stnode
	{
	public:
		std::list<id> args;
		stTree block;
		type getType(){ return type::FUNC; };
	};

	class call :public stnode
	{
	public:
		std::list<stnode> args;
		type getType(){ return type::CALL; };
	};

	class ifelse :public stnode
	{
	public:
		stnode exp;
		stTree blockTrue, blockFalse;
		type getType(){ return type::IF; };
	};
}

int parser(tokenList *tList, stTree *sTree);

#endif