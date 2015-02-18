#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"
#include <unordered_map>
#include <vector>

namespace stnode
{
	class id_inter :public stnode
	{
	public:
		int id;
		id_inter(int _id){ id = _id; };
		type getType() { return type::ID_INTER; };
	};

	class func_inter :public stnode
	{
	public:
		int funcID;
		std::list<int> args;
		varType retType;
		stTree *block;
		type getType() { return type::FUNC_INTER; };
	};

	struct allocUnit_inter
	{
		allocUnit_inter(int _var, int _subCount){ varID = _var; subCount = _subCount; init = false; };
		allocUnit_inter(int _var, int _subCount, stnode **_val){ varID = _var; subCount = _subCount; init = true; val = _val; };
		int varID;
		int subCount;
		bool init;
		stnode **val;
	};

	class alloc_inter :public stnode
	{
	public:
		alloc_inter(bool _readOnly){ readOnly = _readOnly; };
		~alloc_inter();
		std::list<allocUnit_inter> var;
		bool readOnly;
		type getType(){ return type::ALLOC_INTER; };
	};
}

namespace iCode
{
	enum argType{ ERROR, CONST, ID };
	enum opType{ ADD, SUB, MUL, DIV, MOD, SHL, SHR, R_ADD, I_ADD };

	class arg
	{
	public:
		virtual argType getType(){ return argType::ERROR; };
	};

	class con :public arg
	{
	public:
		long long val;
		stnode::varType type;
		argType getType(){ return argType::CONST; };
	};

	class id :public arg
	{
	public:
		int sn;
		argType getType(){ return argType::ID; };
	};

	struct code
	{
		opType op;
		arg *ret, *arg1, *arg2;
	};
}
typedef std::list<iCode::code> iCodeSeq;

errInfo stAnalyzer(stnode::stnode **node);

#endif
