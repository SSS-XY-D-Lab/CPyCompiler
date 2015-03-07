#include "stdafx.h"
#include "st_inter.h"

idTableTp idTable;
funcTableTp funcTable;
//constTableTp constTable;
idHashTableTp idHashTable;

char* str2cstr(std::string arg)
{
	char* ret = new char[arg.size() + 1];
#ifdef _MSC_VER
	strcpy_s(ret, arg.size() + 1, arg.c_str());
#else
	strcpy(ret, arg.c_str());
#endif
	return ret;
}

std::string num2str(long long n)
{
	static std::stringstream sstr;
	std::string ret;
	sstr.clear();
	sstr << n;
	sstr >> ret;
	return ret;
}

char* getDbgLineNStr(const char* msg, int line)
{
	std::string retBuf("inter.cpp:");
	std::stringstream sstr;
	sstr << line;
	sstr >> retBuf;
	retBuf.push_back(':');
	retBuf = retBuf + msg;
	return str2cstr(retBuf);
}

int getID(std::string name)
{
	idHashTableTp::reverse_iterator pItr, pEnd = idHashTable.rend();
	idHashLayerTp::iterator pRes;
	for (pItr = idHashTable.rbegin(); pItr != pEnd; pItr++)
	{
		pRes = (*pItr)->find(name);
		if (pRes != (*pItr)->end())
			return pRes->second;
	}
	return -1;
}

const char* ERR_NEWID_MSG[3] = {
	NULL,
	"Internal Error:No Hash Layer, Please report",
	"Redefinition",
};

int newID(std::string name, dataType type, bool isConst)
{
	if (idHashTable.empty())
		return ERR_NEWID_NOLAYER;
	idHashLayerTp *topLayer = idHashTable.back();
	if (topLayer->count(name) > 0)
		return ERR_NEWID_REDEFINE;
	int id = static_cast<int>(idTable.size());
	topLayer->emplace(name, id);
	idTable.push_back(idItem(type, isConst));
	return id;
}

int newFuncID(std::string name, dataType retType, std::vector<dataType> &argType)
{
	if (idHashTable.empty())
		return ERR_NEWID_NOLAYER;
	idHashLayerTp *topLayer = idHashTable.back();
	if (topLayer->count(name) > 0)
		return ERR_NEWID_REDEFINE;
	int funcID = static_cast<int>(funcTable.size());
	funcTable.push_back(funcItem(retType, argType));
	int id = static_cast<int>(idTable.size());
	idTable.push_back(idItem(funcID));
	topLayer->emplace(name, id);
	return id;
}

errInfo inter_gen_ret(stnode::stnode* node, iCodeSeq &ret, iCode::arg &retVal)
{

	return noErr;
}

errInfo inter_gen(stnode::stnode* node, iCodeSeq &ret)
{
	switch (node->getType())
	{
#ifdef _DEBUG
		case stnode::type::FUNC:
		{
			return errInfo(node->lineN, node->pos, "stAnalyzer_build not executed");
			break;
		}
#endif
		case stnode::type::FUNC_INTER:
		{

		}
	}
	return noErr;
}

errInfo inter(stTree &sTree, iCodeSeq &ret, dataType retType)
{
	stTree::iterator p, pEnd = sTree.end();
	stnode::stnode *pCur;

	idHashTable.push_back(new idHashLayerTp);
	for (p = sTree.begin(); p != pEnd; p++)
	{
		pCur = *p;
		errInfo err = stAnalyzer_build(&pCur);
		if (err.err != NULL)
			return err;
		*p = pCur;
	}
	delete idHashTable.back();
	idHashTable.pop_back();

	for (p = sTree.begin(); p != pEnd; p++)
	{
		pCur = *p;
		errInfo err = stAnalyzer_type(&pCur, retType);
		if (err.err != NULL)
			return err;
		*p = pCur;
	}

	return noErr;
}
