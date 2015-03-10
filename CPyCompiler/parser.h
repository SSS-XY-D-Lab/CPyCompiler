#pragma once

#ifndef _H_ANALYZER
#define _H_ANALYZER

#include "lexer.h"
#include "type.h"

struct errInfo
{
	errInfo(){ err = false; info = NULL; }
	errInfo(int _lineN, int _pos, const char *_info){ err = true; lineN = _lineN; pos = _pos; info = _info; };
	bool err;
	int lineN, pos;
	const char* info;
};
const errInfo noErr = errInfo();

namespace stnode
{
	enum type{ 
		ERROR, 
		NUMBER, CHARA, STR, ID, OP, CAST, FUNC, CALL, RETURN, IF, ALLOC, TREE, DELIM,
		ID_INTER, FUNC_INTER, CALL_INTER, ALLOC_INTER
	};

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
		id(std::string _name, dataType _type){ name = _name; dtype = _type; };
		std::string name;
		dataType dtype;
		type getType() { return type::ID; };
	};

	namespace op
	{
		enum ops{
			ERROR,
			ARRAY_SUB,
			POSI, NEGA, INC_POST, DEC_POST, INC_PRE, DEC_PRE,
			REF, DEREF, NOT, LGNOT,
			DIV, MUL, MOD,
			ADD, SUB,
			SHL, SHR,
			BIG, BIGEQU, LES, LESEQU,
			EQU, NEQU,
			AND, XOR, BOR, LGAND, LGOR,
			ASSIGN, MODASS, DIVASS, MULASS, ADDASS, SUBASS, SHLASS, SHRASS, ANDASS, XORASS, BORASS,
		};

		enum opType{ ARITHMETIC, ASSIGNMENT, LOGICAL, POINTER, OTHER };

		class op :public stnode
		{
		public:
			op(ops _opVal){ opVal = _opVal; argCount = 0; };
			op(ops _opVal, stnode *_arg1){ opVal = _opVal; arg[0] = _arg1; arg[1] = NULL; arg[2] = NULL; argCount = 1; };
			op(ops _opVal, stnode *_arg1, stnode *_arg2){ opVal = _opVal; arg[0] = _arg1; arg[1] = _arg2; arg[2] = NULL; argCount = 2; };
			op(ops _opVal, stnode *_arg1, stnode *_arg2, stnode *_arg3){ opVal = _opVal; arg[0] = _arg1; arg[1] = _arg2; arg[2] = _arg3; argCount = 3; };
			ops opVal;
			int argCount;
			stnode *arg[3];
			dataType resType;
			type getType(){ return type::OP; };

			opType getOpType();
		};

		std::string op2Str(ops op);
	}

	class cast :public stnode
	{
	public:
		cast(stnode *_node, dataType _vtype){ node = _node; vtype = _vtype; };
		stnode *node;
		dataType vtype;
		type getType(){ return type::CAST; };
	};

	class func :public stnode
	{
	public:
		std::string name;
		std::list<id *> args;
		dataType retType;
		stTree *block;
		type getType(){ return type::FUNC; };
	};

	class call :public stnode
	{
	public:
		call(id *_funcID, stnode *_args = NULL){ funcID = _funcID; args = _args; };
		id* funcID;
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
		allocUnit(id* _var, size_t _subCount){ var = _var; subCount = _subCount; init = false; };
		allocUnit(id* _var, stnode **_val, size_t _subCount){ var = _var; init = true; val = _val; subCount = _subCount; };
		id *var;
		size_t subCount;
		bool init;
		stnode **val;
	};

	class alloc :public stnode
	{
	public:
		alloc(bool _isConst){ isConst = _isConst; convert = false; };
		~alloc();
		std::list<allocUnit> var;
		bool isConst;
		bool convert;
		type getType(){ return type::ALLOC; };
	};

	class expTree :public stnode
	{
	public:
		expTree(stnode *_exp, stnode *_prev){ exp = _exp; prev = _prev; };
		stnode *exp, *prev;
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
int yyparse(void);

#endif
