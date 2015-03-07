#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"

namespace stnode
{
	class id_inter :public stnode
	{
	public:
		id_inter(int _id){ id = _id; };
		int id;
		type getType() { return type::ID_INTER; };
	};

	class func_inter :public stnode
	{
	public:
		int funcID;
		std::list<int> args;
		dataType retType;
		stTree *block;
		type getType() { return type::FUNC_INTER; };
	};

	class call_inter :public stnode
	{
	public:
		call_inter(int _funcID, std::vector<stnode*> &_args){ funcID = _funcID; args = _args; };
		call_inter(int _funcID, std::list<stnode*> &_args){ funcID = _funcID; for (std::list<stnode*>::iterator p = _args.begin(), pEnd = _args.end(); p != pEnd; p++) args.push_back(*p); };
		int funcID;
		std::vector<stnode*> args;
		type getType(){ return type::CALL_INTER; };
	};

	struct allocUnit_inter
	{
		allocUnit_inter(int _var, size_t _subCount){ varID = _var; subCount = _subCount; init = false; };
		allocUnit_inter(int _var, size_t _subCount, stnode **_val){ varID = _var; subCount = _subCount; init = true; val = _val; };
		int varID;
		size_t subCount;
		bool init;
		stnode **val;
	};

	class alloc_inter :public stnode
	{
	public:
		alloc_inter(bool _isConst){ isConst = _isConst; };
		~alloc_inter();
		std::list<allocUnit_inter> var;
		bool isConst;
		type getType(){ return type::ALLOC_INTER; };
	};
}

namespace iCode
{
	enum argType{ ERROR, CONST, ID };
	enum opType{ ADD, SUB, MUL, DIV, MOD, SHL, SHR, R_ADD, I_ADD };
	enum codeType{ _ERROR, NORMAL, LABEL, JUMP };

	class arg
	{
	public:
		virtual argType getType(){ return argType::ERROR; };
	};

	class con :public arg
	{
	public:
		long long val;
		argType getType(){ return argType::CONST; };
	};

	class id :public arg
	{
	public:
		int sn;
		argType getType(){ return argType::ID; };
	};

	class iCode
	{
	public:
		virtual codeType getType(){ return codeType::_ERROR; };
	};

	class code :public iCode
	{
	public:
		opType op;
		arg *ret, *arg1, *arg2;
		codeType getType(){ return codeType::NORMAL; };
	};

	class label :public iCode
	{
	public:
		int labelNo;
		codeType getType(){ return codeType::LABEL; };
	};

	class jump :public iCode
	{
	public:
		int labelNo;
		codeType getType(){ return codeType::JUMP; };
	};
}
typedef std::list<iCode::iCode*> iCodeSeq;

errInfo inter(stTree &sTree, iCodeSeq &ret);

#endif
