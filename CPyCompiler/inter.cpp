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
idHashTableTp idHashTable;

int stanalyzer(stnode::stnode **node)
{
	switch ((*node)->getType())
	{
		case stnode::type::ID:
		{
			stnode::id *n = static_cast<stnode::id*>(*node);
			idHashTableTp::iterator pItr;
			pItr = idHashTable.find(n->name);
			if (pItr == idHashTable.end())
				return n->pos;
		}
	}
	return -1;
}