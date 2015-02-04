#include "stdafx.h"
#include "inter.h"


idIndexLayer *correct;
idIndexTable idIndex;
idTable idTbl;

int getID(std::string &name)
{
	idIndexTable::reverse_iterator p, pEnd = idIndex.rend();
	idIndexLayer::iterator pRet;
	idIndexLayer *pTop;
	for (p = idIndex.rbegin(); p != pEnd; p++)
	{
		pTop = *p;
		pRet = pTop->find(name);
		if (pRet != pTop->end())
			return pRet->second;
	}
	return NULL;
}

int inter_arg(stnode::stnode *node)
{

}

int inter_node(stnode::stnode *node, icodeSeq &ret)
{
	switch (node->getType)
	{
		case stnode::type::NUMBER:

	}
}

int inter(stTree &sTree, icodeSeq &ret)
{
	correct = new idIndexLayer;
	idIndex.push_back(correct);
	stTree::iterator p, pEnd = sTree.end();
	for (p = sTree.begin(); p != pEnd; p++)
	{

	}
	return -1;
}
