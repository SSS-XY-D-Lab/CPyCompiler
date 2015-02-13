#pragma once

#ifndef _H_INTER
#define _H_INTER

#include "parser.h"
#include <unordered_map>
#include <unordered_set>

namespace iCode
{
	class id
	{
	public:
		std::string name;
		stnode::varType type;
	};
}

int stanalyzer(stnode::stnode *node);

#endif