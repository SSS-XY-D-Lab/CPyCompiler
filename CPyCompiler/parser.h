#pragma once

#ifndef _H_ANALYZER
#define _H_ANALYZER

#include "lexer.h"

struct errInfo
{
	errInfo(int _lineN, int _pos, const char *_err){ lineN = _lineN; pos = _pos; err = _err; };
	int lineN, pos;
	const char* err;
};
#define noErr (errInfo(-1, -1, NULL))

namespace stnode
{
	enum type{ ERROR, TYPE, NUMBER, CHARA, STR, ID, ID_INTER, OP, FUNC, FUNC_INTER, CALL, RETURN, IF, ALLOC, ALLOC_INTER, TREE, DELIM };

	class stnode
	{
	public:
		int lineN, pos;
		virtual type getType() { return type::ERROR; };
	};
}

typedef std::list<stnode::stnode*> stTree;

namespace stnode
{
	enum varType{ _ERROR, VOID, SINT, S8, S16, S32, S64, UINT, U8, U16, U32, U64, VOID_PTR, SINT_PTR, S8_PTR, S16_PTR, S32_PTR, S64_PTR, UINT_PTR, U8_PTR, U16_PTR, U32_PTR, U64_PTR, VOID_PTR_2, SINT_PTR_2, S8_PTR_2, S16_PTR_2, S32_PTR_2, S64_PTR_2, UINT_PTR_2, U8_PTR_2, U16_PTR_2, U32_PTR_2, U64_PTR_2 };

	class vartype :public stnode
	{
	public:
		vartype(varType _vtype){ vtype = _vtype; };
		varType vtype;
		type getType(){ return type::TYPE; };
	};

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
		id(std::string _name, varType _type = varType::_ERROR, long long _subCount = 0){ name = _name; dtype = _type; subCount = _subCount; };
		std::string name;
		varType dtype;
		long long subCount;
		type getType() { return type::ID; };
	};

	namespace op
	{
		enum ops{
			ERROR,
			COMMA, ARRAY_SUB, MEMBER, CAST,
			POSI, NEGA, INC_POST, DEC_POST, INC_PRE, DEC_PRE,
			REF, DEREF, NOT, LGNOT,
			DIV, MUL, MOD,
			ADD, SUB,
			SHL, SHR,
			BIG, BIGEQU, LES, LESEQU,
			EQU, NEQU,
			AND, XOR, BOR, LGAND, LGOR,
			ASSIGN, MODASS, DIVASS, MULASS, ADDASS, SUBASS, SHLASS, SHRASS, ANDASS, XORASS, BORASS,
			COLONEXP
		};

		enum opType{
			_ERROR, SINGLE, DOUBLE, TRIPLE
		};

		class op :public stnode
		{
		public:
			op(){ opVal = ops::ERROR; };
			op(ops _opVal){ opVal = _opVal; };
			ops opVal;
			type getType(){ return type::OP; };
			virtual opType getOpType(){ return opType::_ERROR; };
		};

		class opSingle :public op
		{
		public:
			opSingle(ops _opVal, stnode *_arg1 = NULL){ opVal = _opVal; arg1 = _arg1; };
			stnode *arg1;
			opType getOpType(){ return opType::SINGLE; };
		};

		class opDouble :public op
		{
		public:
			opDouble(ops _opVal, stnode *_arg1 = NULL, stnode *_arg2 = NULL){ opVal = _opVal; arg1 = _arg1; arg2 = _arg2; };
			stnode *arg1, *arg2;
			opType getOpType(){ return opType::DOUBLE; };
		};

		class opTriple :public op
		{
		public:
			opTriple(ops _opVal, stnode *_arg1 = NULL, stnode *_arg2 = NULL, stnode *_arg3 = NULL){ opVal = _opVal; arg1 = _arg1; arg2 = _arg2; arg3 = _arg3; };
			stnode *arg1, *arg2, *arg3;
			opType getOpType(){ return opType::TRIPLE; };
		};

		std::string op2Str(ops op);
	}

	class func :public stnode
	{
	public:
		std::string name;
		std::list<id *> args;
		varType retType;
		stTree *block;
		type getType(){ return type::FUNC; };
	};

	class call :public stnode
	{
	public:
		call(stnode *_id, stnode *_args = NULL){ id = _id; args = _args; };
		stnode* id;
		stnode* args;
		type getType(){ return type::CALL; };
	};

	class ret :public stnode
	{
	public:
		stnode *retVal;
		type getType(){ return type::RETURN; };
	};

	class ifelse :public stnode
	{
	public:
		stnode *exp;
		stTree *blockTrue, *blockFalse;
		type getType(){ return type::IF; };
	};

	struct allocUnit
	{
		allocUnit(id* _var){ var = _var; init = false; };
		allocUnit(id* _var, stnode **_val){ var = _var; init = true; val = _val; };
		id *var;
		bool init;
		stnode **val;
	};

	class alloc :public stnode
	{
	public:
		alloc(bool _readOnly){ readOnly = _readOnly; convert = false; };
		~alloc();
		std::list<allocUnit> var;
		bool readOnly;
		bool convert;
		type getType(){ return type::ALLOC; };
	};

	class expTree :public stnode
	{
	public:
		expTree(stnode *_exp, stnode *_son){ exp = _exp; son = _son; };
		stnode *exp, *son;
		type getType(){ return type::TREE; };
	};

	class delim :public stnode
	{
	public:
		type getType(){ return type::DELIM; };
	};
}

errInfo parser(tokenList &tList, stTree *sTree);

extern int yacc_lineN;
extern stnode::stnode *yacc_result;
extern tokenList::iterator yacc_p, yacc_pEnd;
extern const char *yacc_err;
int yyparse();

#endif
