#include "stdafx.h"
#include "st_inter.h"

strTableTp strTable;
//constTableTp constTable;

std::mutex strMutex, labelMutex;
int labelCount = 0;

int newStr(const std::string &str)
{
	strMutex.lock();
	int id = static_cast<int>(strTable.size());
	strTable.push_back(str);
	strMutex.unlock();
	return id;
}

int newLbl()
{
	labelMutex.lock();
	int id = labelCount;
	labelCount++;
	labelMutex.unlock();
	return id;
}

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

char* getDbgLineNStr(const char* msg, const char* file, int line)
{
	std::string retBuf;
	std::stringstream sstr;
	sstr << line;
	sstr >> retBuf;
	retBuf = file + ':' + retBuf;
	retBuf.push_back(':');
	retBuf = retBuf + msg;
	return str2cstr(retBuf);
}

errInfo inter_gen(stnode::stnode* node, iCodeSeq &ret, iCode::arg **retVal)
{
	switch (node->getType())
	{
		case stnode::type::NUMBER:
		{
			*retVal = new iCode::con(static_cast<stnode::number *>(node)->val);
			break;
		}
		case stnode::type::CHARA:
		{
			*retVal = new iCode::con(static_cast<stnode::chara *>(node)->ch);
			break;
		}
		case stnode::type::STR:
		{
			int id = newStr(static_cast<stnode::str *>(node)->strr);
			*retVal = new iCode::con(static_cast<stnode::chara *>(node)->ch);
			break;
		}
		case stnode::type::ID_INTER:
		{
			*retVal = new iCode::id(static_cast<stnode::id_inter *>(node)->id);
			break;
		}
		case stnode::type::OP:
		{
			stnode::op::op *opNode = static_cast<stnode::op::op *>(node);
			switch (opNode->opVal)
			{
				case stnode::op::ops::ADD:

					break;
			}
		}
		case stnode::type::FUNC_INTER:
		{
			stnode::func_inter *funcNode = static_cast<stnode::func_inter *>(node);
			stTree *block = funcNode->block;
			stTree::iterator p = block->begin(), pEnd = block->end();
			iCode::arg *retVal;
			ret.push_back(new iCode::label(funcNode->funcID));
			for (; p != pEnd; p++)
			{
				errInfo err = inter_gen(*p, ret, &retVal);
				if (err.err)
					return err;
			}
			break;
		}
		
	}
	return noErr;
}

errInfo inter(stTree &sTree, iCodeSeq &ret, dataType retType)
{
	stTree::iterator p, pEnd = sTree.end();
	stnode::stnode *pCur;

	idHashTableTp idHashTable;
	idHashTable.push_back(new idHashLayerTp);
	for (p = sTree.begin(); p != pEnd; p++)
	{
		pCur = *p;
		errInfo err = stAnalyzer_build(&pCur, idHashTable);
		if (err.err)
			return err;
		*p = pCur;
	}
	delete idHashTable.back();
	idHashTable.pop_back();

	for (p = sTree.begin(); p != pEnd; p++)
	{
		pCur = *p;
		errInfo err = stAnalyzer_type(&pCur, retType, idHashTable);
		if (err.err)
			return err;
		*p = pCur;
	}

	return noErr;
}
