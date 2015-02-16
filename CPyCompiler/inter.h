#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace iCode
{
	enum argType{ ERROR, CONST, ID };
	enum opType{};

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

int stanalyzer(stnode::stnode *node);

#endif