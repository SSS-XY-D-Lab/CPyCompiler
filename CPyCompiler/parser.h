#pragma once

#ifndef _H_ANALYZER
#define _H_ANALYZER

#include "lexer.h"
#include "type.h"

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
	class vartype :public stnode
	{
	public:
		vartype(dataType::type _vtype){ vtype = _vtype; };
		dataType::type vtype;
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
		id(std::string _name, dataType::type _type = dataType::ERROR){ name = _name; dtype = _type; };
		std::string name;
		dataType::type dtype;
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

		class op :public stnode
		{
		public:
			op(ops _opVal){ opVal = _opVal; argCount = 0; };
			op(ops _opVal, stnode *_arg1){ opVal = _opVal; arg[0] = _arg1; argCount = 1; };
			op(ops _opVal, stnode *_arg1, stnode *_arg2){ opVal = _opVal; arg[0] = _arg1; arg[1] = _arg2; argCount = 2; };
			op(ops _opVal, stnode *_arg1, stnode *_arg2, stnode *_arg3){ opVal = _opVal; arg[0] = _arg1; arg[1] = _arg2; arg[2] = _arg3; argCount = 3; };
			ops opVal;
			int argCount;
			stnode *arg[3];
			dataType::type resType;
			type getType(){ return type::OP; };
		};

		std::string op2Str(ops op);
	}

	class func :public stnode
	{
	public:
		std::string name;
		std::list<id *> args;
		dataType::type retType;
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
