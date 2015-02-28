#include "stdafx.h"
#include "inter.h"

struct idItem
{
	idItem(dataType::type _type, bool _isConst){ type = _type; isFunc = false; isConst = _isConst; };
	idItem(dataType::type _type, dataType::type _retType){ type = _type; isFunc = true; retType = _retType; isConst = true; };
	dataType::type type, retType;
	bool isConst, isFunc;
};
typedef std::vector<idItem> idTableTp;
idTableTp idTable;
typedef std::unordered_map<int, int> constTableTp;
constTableTp constTable;

typedef std::unordered_map<std::string, int> idHashLayerTp;
typedef std::list<idHashLayerTp*> idHashTableTp;
idHashTableTp idHashTable;

char* getDbgLineNStr(const char* msg, int line)
{
	std::string retBuf("inter.cpp:");
	std::stringstream sstr;
	sstr << line;
	sstr >> retBuf;
	retBuf.push_back(':');
	retBuf = retBuf + msg;
	char* ret = new char[retBuf.size() + 1];
#ifdef _MSC_VER
	strcpy_s(ret, retBuf.size() + 1, retBuf.c_str());
#else
	strcpy(ret, retBuf.c_str());
#endif
	return ret;
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

const int ERR_NEWID_NOLAYER = -1;
const int  ERR_NEWID_REDEFINE = -2;
const char* ERR_NEWID_MSG[3] = {
	NULL,
	"Internal Error:No Hash Layer, Please report",
	"Redefinition",
};

int newID(std::string name, dataType::type type, bool isConst = false)
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

int newFuncID(std::string name, dataType::type retType)
{
	if (idHashTable.empty())
		return ERR_NEWID_NOLAYER;
	idHashLayerTp *topLayer = idHashTable.back();
	if (topLayer->count(name) > 0)
		return ERR_NEWID_REDEFINE;
	int id = static_cast<int>(idTable.size());
	topLayer->emplace(name, id);
	idTable.push_back(idItem(dataType::VOID_PTR, retType));
	return id;
}

errInfo stAnalyzer_build(stnode::stnode **node)
{
	switch ((*node)->getType())
	{
		case stnode::type::ID:
		{
			stnode::id *oldNode = static_cast<stnode::id*>(*node);
			int id = getID(oldNode->name);
			if (id == -1)
				return errInfo(oldNode->lineN, oldNode->pos, "Undefined Variant on inter.cpp:");
			stnode::id_inter *newNode = new stnode::id_inter(id);
			newNode->lineN = oldNode->lineN;
			newNode->pos = oldNode->pos;
			delete oldNode;
			*node = newNode;
			break;
		}
		case stnode::type::OP:
		{
			stnode::op::op *opNode = static_cast<stnode::op::op*>(*node);
			for (int i = 0; i < opNode->argCount; i++)
			{
				errInfo err = stAnalyzer_build(&opNode->arg[i]);
				if (err.err != NULL)
					return err;
			}
			break;
		}
		case stnode::type::FUNC:
		{
			stnode::func *oldNode = static_cast<stnode::func*>(*node);

			int funcID = newFuncID(oldNode->name, oldNode->retType);
			if (funcID < 0)
				return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-funcID]);
			idHashTable.push_back(new idHashLayerTp);
			stnode::func_inter *newNode = new stnode::func_inter;
			newNode->lineN = oldNode->lineN;
			newNode->pos = oldNode->pos;
			newNode->funcID = funcID;
			newNode->retType = oldNode->retType;

			{
				std::list<stnode::id*>::iterator p, pEnd = oldNode->args.end();
				int argID;
				for (p = oldNode->args.begin(); p != pEnd; p++)
				{
					argID = newID((*p)->name, (*p)->dtype, true);
					if (argID < 0)
						return errInfo((*p)->lineN, (*p)->pos, ERR_NEWID_MSG[-argID]);
					newNode->args.push_back(argID);
				}
			}

			{
				stTree *block = oldNode->block;
				stTree::iterator p, pEnd = block->end();
				stnode::stnode *ptr;
				errInfo err = noErr;
				for (p = block->begin(); p != pEnd; p++)
				{
					ptr = *p;
					err = stAnalyzer_build(&ptr);
					*p = ptr;
				}
				newNode->block = block;
			}

			delete oldNode;
			delete idHashTable.back();
			idHashTable.pop_back();
			*node = newNode;
			break;
		}
		case stnode::type::CALL:
		{
			stnode::call *callNode = static_cast<stnode::call*>(*node);
			stnode::stnode *tmpPtr = callNode->id;
			errInfo err = stAnalyzer_build(&tmpPtr);
			if (err.err != NULL)
				return err;
			if (tmpPtr->getType() != stnode::type::ID_INTER)
				return errInfo(callNode->id->lineN, callNode->id->pos, getDbgLineNStr("Internal error;Please contact developer", __LINE__));
			err = stAnalyzer_build(&(callNode->args));
			if (err.err != NULL)
				return err;

			stnode::call_inter *newNode = new stnode::call_inter(static_cast<stnode::id_inter*>(tmpPtr)->id, callNode->args);
			delete callNode;
			*node = newNode;
			break;
		}
		case stnode::type::RETURN:
		{
			stnode::ret *retNode = static_cast<stnode::ret*>(*node);
			errInfo err = stAnalyzer_build(&(retNode->retVal));
			if (err.err != NULL)
				return err;
			break;
		}
		case stnode::type::IF:
		{
			stnode::ifelse *ifNode = static_cast<stnode::ifelse*>(*node);
			idHashTable.push_back(new idHashLayerTp);

			errInfo err = stAnalyzer_build(&(ifNode->exp));
			if (err.err != NULL)
				return err;

			stTree::iterator p, pEnd;
			stnode::stnode *ptr;

			pEnd = ifNode->blockTrue->end();
			for (p = ifNode->blockTrue->begin(); p != pEnd; p++)
			{
				ptr = *p;
				err = stAnalyzer_build(&ptr);
				*p = ptr;
			}

			if (ifNode->blockFalse != NULL)
			{
				pEnd = ifNode->blockFalse->end();
				for (p = ifNode->blockFalse->begin(); p != pEnd; p++)
				{
					ptr = *p;
					err = stAnalyzer_build(&ptr);
					*p = ptr;
				}
			}

			delete idHashTable.back();
			idHashTable.pop_back();
			break;
		}
		case stnode::type::ALLOC:
		{
			stnode::alloc *oldNode = static_cast<stnode::alloc*>(*node);
			stnode::alloc_inter *newNode = new stnode::alloc_inter(oldNode->isConst);

			std::list<stnode::allocUnit>::iterator p, pEnd = oldNode->var.end();
			int varID;
			for (p = oldNode->var.begin(); p != pEnd; p++)
			{
				if (p->subCount > 0)
					varID = newID(p->var->name, toPtr(p->var->dtype));
				else
					varID = newID(p->var->name, p->var->dtype);
				if (varID < 0)
					return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-varID]);
				if (p->init)
					newNode->var.push_back(stnode::allocUnit_inter(varID, p->subCount, p->val));
				else
					newNode->var.push_back(stnode::allocUnit_inter(varID, p->subCount));
			}

			oldNode->convert = true;
			delete oldNode;
			*node = newNode;
			break;
		}
		case stnode::type::TREE:
		{
			stnode::expTree *treeNode = static_cast<stnode::expTree*>(*node);
			errInfo err = stAnalyzer_build(&(treeNode->exp));
			if (err.err != NULL)
				return err;
			err = stAnalyzer_build(&(treeNode->prev));
			if (err.err != NULL)
				return err;
			break;
		}
	}
	return noErr;
}

errInfo getNodeType(stnode::stnode **node, dataType::type &type, dataType::type retType)
{
	stnode::stnode *ptr = *node;
	switch (ptr->getType())
	{
		case stnode::type::NUMBER:
		{
			type = minNum(static_cast<stnode::number*>(ptr)->val);
			break;
		}
		case stnode::type::CHARA:
		{
			type = dataType::S8;
			break;
		}
		case stnode::type::STR:
		{
			type = dataType::S8_PTR;
			break;
		}
		case stnode::type::ID:
		{
			return errInfo(ptr->lineN, ptr->pos, "stAnalyzer_build not executed");
			break;
		}
		case stnode::type::ID_INTER:
		{
			type = idTable[static_cast<stnode::id_inter*>(ptr)->id].type;
			break;
		}
		case stnode::type::OP:
		{
			stnode::op::op* opPtr = static_cast<stnode::op::op*>(ptr);
			if (opPtr->resType != dataType::ERROR)
			{
				type = opPtr->resType;
			}
			else
			{
				stAnalyzer_type(node, retType);
				return getNodeType(node, type, retType);
			}
			break;
		}
		case stnode::type::CAST:
		{
			type = static_cast<stnode::cast*>(ptr)->vtype;
			break;
		}
		case stnode::type::CALL:
		{
			return errInfo(ptr->lineN, ptr->pos, "stAnalyzer_build not executed");
			break;
		}
		case stnode::type::CALL_INTER:
		{
			idItem funcItem = idTable[static_cast<stnode::call_inter*>(ptr)->funcID];
			if (funcItem.isFunc == false)
				return errInfo(ptr->lineN, ptr->pos, "Call of non-function");
			type = funcItem.retType;
			break;
		}
		case stnode::type::TREE:
		{
			return getNodeType(&(static_cast<stnode::expTree*>(ptr)->exp), type, retType);
			break;
		}
		default:
		{
			return errInfo(ptr->lineN, ptr->pos, "It shouldn't be here");
		}
	}
	return noErr;
}

errInfo stAnalyzer_type(stnode::stnode **node, dataType::type retType)
{
	switch ((*node)->getType())
	{
		case stnode::type::OP:
		{
			stnode::op::op *opNode = static_cast<stnode::op::op*>(*node);
			switch (opNode->getOpType())
			{
				case stnode::op::opType::ARITHMETIC:
				{
					dataType::type type = dataType::SINT, typeRet = dataType::ERROR, types[3];
					stnode::stnode *ptr;
					int i;
					//Get dataType of the args
					for (i = 0; i < opNode->argCount; i++)
					{
						errInfo err = getNodeType(&opNode->arg[i], typeRet, retType);
						if (err.err != NULL)
							return err;
						types[i] = typeRet;
						if (typeLvl(type) < typeLvl(typeRet))
							type = typeRet;
					}
					//Write cast node
					for (i = 0; i < opNode->argCount; i++)
					{
						if (typeLvl(types[i]) < typeLvl(type))
						{
							opNode->arg[i] = new stnode::cast(opNode->arg[i], type);
						}
					}
				}
				case stnode::op::opType::ASSIGNMENT:
				{
					//cast R to L
				}
				default:
				{
					//Needn't auto cast
				}
			}
			break;
		}
		case stnode::type::FUNC_INTER:
		{
			
			break;
		}
		case stnode::type::CALL:
		{
			stnode::call *callNode = static_cast<stnode::call*>(*node);
			
			break;
		}
		case stnode::type::RETURN:
		{
			stnode::ret *retNode = static_cast<stnode::ret*>(*node);
			
			break;
		}
		case stnode::type::IF:
		{
			stnode::ifelse *ifNode = static_cast<stnode::ifelse*>(*node);

			errInfo err = stAnalyzer_type(&(ifNode->exp), retType);
			if (err.err != NULL)
				return err;

			stTree::iterator p, pEnd;
			stnode::stnode *ptr;

			pEnd = ifNode->blockTrue->end();
			for (p = ifNode->blockTrue->begin(); p != pEnd; p++)
			{
				ptr = *p;
				err = stAnalyzer_type(&ptr, retType);
				*p = ptr;
			}

			if (ifNode->blockFalse != NULL)
			{
				pEnd = ifNode->blockFalse->end();
				for (p = ifNode->blockFalse->begin(); p != pEnd; p++)
				{
					ptr = *p;
					err = stAnalyzer_type(&ptr, retType);
					*p = ptr;
				}
			}

			break;
		}
		case stnode::type::ALLOC_INTER:
		{
			
			break;
		}
		case stnode::type::TREE:
		{
			stnode::expTree *treeNode = static_cast<stnode::expTree*>(*node);
			errInfo err = stAnalyzer_type(&(treeNode->exp), retType);
			if (err.err != NULL)
				return err;
			err = stAnalyzer_type(&(treeNode->prev), retType);
			if (err.err != NULL)
				return err;
			break;
		}
	}
	return noErr;
}

errInfo inter(stTree sTree, iCodeSeq &ret)
{
	idHashTable.push_back(new idHashLayerTp);
	stTree::iterator p, pEnd = sTree.end();
	stnode::stnode *pCur;
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

	return noErr;
}
