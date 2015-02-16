#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"
#include <unordered_map>
#include <unordered_set>
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

int stanalyzer(stnode::stnode **node);

#endif