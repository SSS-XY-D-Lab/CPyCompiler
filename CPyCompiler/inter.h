#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"
#include <unordered_map>
#include <vector>

namespace inter
{
	enum opType{};

	struct arg
	{
		stnode::type type;
		std::string name;
	};

	struct op
	{
		opType op;
		int arg1, arg2, res;
	};
}
typedef std::unordered_map<std::string, int> idIndexLayer;
typedef std::list<idIndexLayer*> idIndexTable;
typedef std::vector<inter::arg> idTable;

typedef std::list<inter::op> icodeSeq;

int inter(stTree &ret);

#endif