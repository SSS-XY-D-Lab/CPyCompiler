#include "stdafx.h"
#include "inter.h"

struct idItem
{
	std::string name;
	stnode::varType type;
};
typedef std::vector<idItem> idTableTp;
idTableTp idTable;

typedef std::list<int> idHashItemTp;
typedef std::unordered_map<std::string, idHashItemTp*> idHashTableTp;

int stanalyzer(stnode::stnode *node)
{

	return -1;
}