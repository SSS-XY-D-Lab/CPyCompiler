#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"

namespace stnode
{
	class id_global :public stnode
	{
	public:
		id_global(size_t _id){ id = _id; };
		size_t id;
		type getType() { return type::ID_GLOBAL; };
	};

	class id_local :public stnode
	{
	public:
		id_local(dataType _dtype, size_t _shift){ dtype = _dtype; shift = _shift; };
		dataType dtype;
		size_t shift;
		type getType() { return type::ID_LOCAL; };
	};

	class func_inter :public stnode
	{
	public:
		size_t funcID;
		std::list<size_t> args;
		dataType retType;
		stTree *block;
		type getType() { return type::FUNC_INTER; };
	};

	class call_inter :public stnode
	{
	public:
		call_inter(size_t _funcID, std::vector<stnode*> &_args){ funcID = _funcID; args = _args; };
		call_inter(size_t _funcID, std::list<stnode*> &_args){ funcID = _funcID; for (std::list<stnode*>::iterator p = _args.begin(), pEnd = _args.end(); p != pEnd; p++) args.push_back(*p); };
		size_t funcID;
		std::vector<stnode*> args;
		type getType(){ return type::CALL_INTER; };
	};

	struct allocUnit_global
	{
		allocUnit_global(size_t _var, size_t _subCount){ varID = _var; subCount = _subCount; init = false; };
		allocUnit_global(size_t _var, size_t _subCount, stnode **_val){ varID = _var; subCount = _subCount; init = true; val = _val; };
		size_t varID;
		size_t subCount;
		bool init;
		stnode **val;
	};

	struct allocUnit_local
	{
		allocUnit_local(dataType _dtype, size_t _subCount){ dtype = _dtype; subCount = _subCount; init = false; };
		allocUnit_local(dataType _dtype, size_t _subCount, stnode **_val){ dtype = _dtype; subCount = _subCount; init = true; val = _val; };
		dataType dtype;
		size_t subCount;
		bool init;
		stnode **val;
	};

	class alloc_global :public stnode
	{
	public:
		alloc_global(bool _isConst){ isConst = _isConst; };
		~alloc_global();
		std::list<allocUnit_global> var;
		bool isConst;
		type getType(){ return type::ALLOC_GLOBAL; };
	};

	class alloc_local :public stnode
	{
	public:
		alloc_local(bool _isConst){ isConst = _isConst; };
		~alloc_local();
		std::list<allocUnit_local> var;
		bool isConst;
		type getType(){ return type::ALLOC_LOCAL; };
	};
}

namespace iCode
{
	enum argType{ ERROR, CONST, STR, ID_GLOBAL, ID_LOCAL, CSHIFT, TEMP };
	enum opType{
		NUL, SET,
		ADD, SUB, MUL, DIV, MOD, SHL, SHR, AND, BOR, XOR, NOT,
		R_ADD, I_ADD, G_ADD,
		PUSH, POP, PEEK, SP_ADD, SP_SUB
	};
	enum ifType{
		IFE, IFN, IFG, IFGE, IFL, IFLE
	};
	enum codeType{ _ERROR, NORMAL, LABEL, JUMP, JUMP_IF, CALL, RET };

	class arg
	{
	public:
		virtual argType getType(){ return argType::ERROR; };
	};

	class con :public arg
	{
	public:
		con(long long _val){ val = _val; }
		long long val;
		argType getType(){ return argType::CONST; };
	};

	class str :public arg
	{
	public:
		str(size_t _strID){ strID = _strID; }
		size_t strID;
		argType getType(){ return argType::STR; };
	};

	class id_global :public arg
	{
	public:
		id_global(size_t _sn){ sn = _sn; }
		size_t sn;
		argType getType(){ return argType::ID_GLOBAL; };
	};

	class id_local :public arg
	{
	public:
		id_local(size_t _shift){ shift = _shift; }
		size_t shift;
		argType getType(){ return argType::ID_LOCAL; };
	};

	class cshift :public arg
	{
	public:
		cshift(arg *_base, size_t _shift){ base = _base; shift = _shift; }
		arg *base;
		size_t shift;
		argType getType(){ return argType::CSHIFT; };
	};

	class iCode
	{
	public:
		virtual codeType getType(){ return codeType::_ERROR; };
	};

	class temp :public arg
	{
	public:
		temp(){ ref = 0; };
		std::list<iCode*> pCode;
		int ref;
		argType getType(){ return argType::TEMP; };
	};

	class code :public iCode
	{
	public:
		code(opType _op, arg *_ret, arg *_arg1, arg *_arg2 = NULL){ op = _op; ret = _ret; arg1 = _arg1; arg2 = _arg2; };
		opType op;
		arg *ret, *arg1, *arg2;
		dataType retType, argType;
		codeType getType(){ return codeType::NORMAL; };
	};

	class label :public iCode
	{
	public:
		label(size_t _labelNo){ labelNo = _labelNo; }
		size_t labelNo;
		codeType getType(){ return codeType::LABEL; };
	};

	class jump :public iCode
	{
	public:
		jump(size_t _labelNo){ labelNo = _labelNo; }
		size_t labelNo;
		codeType getType(){ return codeType::JUMP; };
	};

	class jump_if :public iCode
	{
	public:
		jump_if(ifType _op, size_t _labelNo, arg *_arg1, arg *_arg2){ op = _op; arg1 = _arg1; arg2 = _arg2; labelNo = _labelNo; }
		ifType op;
		arg *arg1, *arg2;
		size_t labelNo;
		codeType getType(){ return codeType::JUMP_IF; };
	};

	class call :public iCode
	{
	public:
		call(size_t _labelNo, arg *_ret){ labelNo = _labelNo; ret = _ret; }
		size_t labelNo;
		arg *ret;
		codeType getType(){ return codeType::CALL; };
	};

	class ret :public iCode
	{
	public:
		ret(arg *_val = NULL){ val = _val; }
		arg *val;
		codeType getType(){ return codeType::RET; };
	};
}
typedef std::list<iCode::iCode*> iCodeSeq;

errInfo inter(stTree &sTree, iCodeSeq &ret, dataType retType);

#endif
