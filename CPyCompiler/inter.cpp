#include "stdafx.h"
#include "inter.h"

struct idItem
{
	idItem(dataType _type, bool _isConst){ type = _type; funcID = -1; isConst = _isConst; };
	idItem(int _funcID){ type = dataType(dataType::VOID, 1); funcID = _funcID; isConst = true; };
	dataType type;
	bool isConst;
	int funcID;
};
typedef std::vector<idItem> idTableTp;
idTableTp idTable;

struct funcItem
{
	funcItem(dataType _retType, std::vector<dataType> &_argType){ retType = _retType; argType = _argType; };
	dataType retType;
	std::vector<dataType> argType;
};
typedef std::vector<funcItem> funcTableTp;
funcTableTp funcTable;

typedef std::unordered_map<int, int> constTableTp;
//constTableTp constTable;

typedef std::unordered_map<std::string, int> idHashLayerTp;
typedef std::list<idHashLayerTp*> idHashTableTp;
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

const int ERR_NEWID_NOLAYER = -1;
const int  ERR_NEWID_REDEFINE = -2;
const char* ERR_NEWID_MSG[3] = {
	NULL,
	"Internal Error:No Hash Layer, Please report",
	"Redefinition",
};

int newID(std::string name, dataType type, bool isConst = false)
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

errInfo stAnalyzer_build(stnode::stnode **node)
{
	switch ((*node)->getType())
	{
		case stnode::type::ID:
		{
			stnode::id *oldNode = static_cast<stnode::id*>(*node);
			int id = getID(oldNode->name);
			if (id == -1)
				return errInfo(oldNode->lineN, oldNode->pos, "Undefined Variant");
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

			idHashTable.push_back(new idHashLayerTp);
			stnode::func_inter *newNode = new stnode::func_inter;
			newNode->lineN = oldNode->lineN;
			newNode->pos = oldNode->pos;
			newNode->retType = oldNode->retType;
			std::vector<dataType> argType;

			{
				std::list<stnode::id*>::iterator p, pEnd = oldNode->args.end();
				int argID;
				for (p = oldNode->args.begin(); p != pEnd; p++)
				{
					argID = newID((*p)->name, (*p)->dtype, true);
					if (argID < 0)
						return errInfo((*p)->lineN, (*p)->pos, ERR_NEWID_MSG[-argID]);
					argType.push_back((*p)->dtype);
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
					if (err.err != NULL)
						return err;
					*p = ptr;
				}
				newNode->block = block;
			}

			int funcID = newFuncID(oldNode->name, oldNode->retType, argType);
			if (funcID < 0)
				return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-funcID]);
			newNode->funcID = funcID;

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
				return errInfo(callNode->id->lineN, callNode->id->pos, getDbgLineNStr("Internal error:Please contact developer", __LINE__));
			
			err = stAnalyzer_build(&(callNode->args));
			if (err.err != NULL)
				return err;
			std::list<stnode::stnode*> argList;
			stnode::stnode* ptr = callNode->args;
			while (ptr->getType() != stnode::type::TREE)
			{
				argList.push_front(static_cast<stnode::expTree*>(ptr)->exp);
				ptr = static_cast<stnode::expTree*>(ptr)->prev;
			}

			stnode::call_inter *newNode = new stnode::call_inter(static_cast<stnode::id_inter*>(tmpPtr)->id, argList);
			newNode->lineN = callNode->lineN;
			newNode->pos = callNode->pos;
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
					varID = newID(p->var->name, toRef(p->var->dtype));
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
			newNode->lineN = oldNode->lineN;
			newNode->pos = oldNode->pos;
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

errInfo getNodeType(stnode::stnode **node, dataType &type, dataType retType)
{
	stnode::stnode *ptr = *node;
	switch (ptr->getType())
	{
		case stnode::type::NUMBER:
		{
			type = dataType(minNum(static_cast<stnode::number*>(ptr)->val), 0, true);
			break;
		}
		case stnode::type::CHARA:
		{
			type = dataType(dataType::S8, 0, true);
			break;
		}
		case stnode::type::STR:
		{
			type = dataType(dataType::S8, 1);
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
			if (opPtr->resType.dType != dataType::ERROR)
			{
				type = opPtr->resType;
			}
			else
			{
				errInfo err = stAnalyzer_type(node, retType);
				if (err.err != NULL)
					return err;
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
			if (funcItem.funcID == -1)
				return errInfo(ptr->lineN, ptr->pos, "Call of non-function");
			type = funcTable[funcItem.funcID].retType;
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

errInfo stAnalyzer_type(stnode::stnode **node, dataType retType)
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
					dataType type = dataType(dataType::SINT, 0), typeRet = dataType(dataType::ERROR, 0), types[3];
					int i;
					//Get dataType of the args
					for (i = 0; i < opNode->argCount; i++)
					{
						errInfo err = getNodeType(&(opNode->arg[i]), typeRet, retType);
						if (err.err != NULL)
							return err;
						types[i] = typeRet;
						if (typeLvl(type) < typeLvl(typeRet))
							type = typeRet;
					}
					//Write cast node
					for (i = 0; i < opNode->argCount; i++)
					{
						if (types[i].isConst == false)
						{
							if (types[i].ptrLvl != type.ptrLvl || (types[i].dType == dataType::VOID && types[i].ptrLvl == 0))
							{
								return errInfo(opNode->arg[i]->lineN, opNode->arg[i]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(types[i]) + "to" + type2Str(type)));
							}
							if (typeLvl(types[i]) < typeLvl(type))
							{
								opNode->arg[i] = new stnode::cast(opNode->arg[i], type);
							}
						}
					}
					opNode->resType = type;
					break;
				}
				case stnode::op::opType::ASSIGNMENT:
				{
					//cast R to L
					if (opNode->argCount != 2)
						return errInfo(opNode->lineN, opNode->pos, "Assignment operator must have 2 arg");
					dataType type1, type2;
					errInfo err = getNodeType(&opNode->arg[1], type2, retType);
					if (err.err != NULL)
						return err;
					if (type2.isConst == false)
					{
						err = getNodeType(&opNode->arg[0], type1, retType);
						if (err.err != NULL)
							return err;
						if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
						{
							return errInfo(opNode->arg[1]->lineN, opNode->arg[1]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + "to" + type2Str(type1)));
						}
						if (type2.dType != type1.dType)
						{
							opNode->arg[1] = new stnode::cast(opNode->arg[1], type1);
						}
					}
					opNode->resType = type1;
					break;
				}
				case stnode::op::opType::LOGICAL:
				{
					opNode->resType = dataType(dataType::SINT, 0);
					break;
				}
				case stnode::op::opType::POINTER:
				{
					dataType type;
					switch (opNode->opVal)
					{
						case stnode::op::ops::ARRAY_SUB:
						{
							if (opNode->argCount != 2)
								return errInfo(opNode->lineN, opNode->pos, "Operator must have 2 arg");
							errInfo err = getNodeType(&opNode->arg[0], type, retType);
							if (err.err != NULL)
								return err;
							if (type.ptrLvl < 1)
								return errInfo(opNode->arg[1]->lineN, opNode->arg[1]->pos, "Need a pointer");
							opNode->resType = deref(type);
							break;
						}
						case stnode::op::ops::DEREF:
						{
							if (opNode->argCount != 1)
								return errInfo(opNode->lineN, opNode->pos, "Operator must have 1 arg");
							errInfo err = getNodeType(&opNode->arg[0], type, retType);
							if (err.err != NULL)
								return err;
							if (type.ptrLvl < 1)
								return errInfo(opNode->arg[1]->lineN, opNode->arg[1]->pos, "Need a pointer");
							opNode->resType = deref(type);
							break;
						}
						case stnode::op::ops::REF:
						{
							if(opNode->argCount != 1)
								return errInfo(opNode->lineN, opNode->pos, "Operator must have 1 arg");
							errInfo err = getNodeType(&opNode->arg[0], type, retType);
							if (err.err != NULL)
								return err;
							opNode->resType = toRef(type);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case stnode::type::FUNC_INTER:
		{
			stnode::func_inter *funcNode = static_cast<stnode::func_inter*>(*node);
			dataType newRetType = funcNode->retType;

			stTree *block = funcNode->block;
			stTree::iterator p, pEnd = block->end();
			stnode::stnode *ptr;
			errInfo err = noErr;
			for (p = block->begin(); p != pEnd; p++)
			{
				ptr = *p;
				err = stAnalyzer_type(&ptr, newRetType);
				if (err.err != NULL)
					return err;
				*p = ptr;
			}

			break;
		}
		case stnode::type::CALL:
		{
			return errInfo((*node)->lineN, (*node)->pos, "stAnalyzer_build not executed");
			break;
		}
		case stnode::type::CALL_INTER:
		{
			stnode::call_inter *callNode = static_cast<stnode::call_inter*>(*node);
			if (callNode->funcID == -1)
				return errInfo((*node)->lineN, (*node)->pos, "Internal error:stAnalyer_type got a non-function id when processing CALL_INTER node;Please contact developer");
			int funcID = callNode->funcID;
			funcItem &funcInfo = funcTable[funcID];
			if (funcInfo.argType.size() != callNode->args.size())
				return errInfo((*node)->lineN, (*node)->pos, str2cstr(std::string("Function doesn't accept ") + num2str(callNode->args.size()) + " arguments"));
			break;
		}
		case stnode::type::RETURN:
		{
			stnode::ret *retNode = static_cast<stnode::ret*>(*node);
			dataType valType;
			errInfo err = getNodeType(&(retNode->retVal), valType, retType);
			if (err.err != NULL)
				return err;
			if (valType.isConst == false)
			{
				if (retType.ptrLvl != valType.ptrLvl || (valType.dType == dataType::VOID && valType.ptrLvl == 0))
				{
					return errInfo(retNode->lineN, retNode->pos, str2cstr(std::string("Invalid cast from ") + type2Str(valType) + "to" + type2Str(retType)));
				}
				if (retType.dType != valType.dType)
				{
					retNode->retVal = new stnode::cast(retNode->retVal, retType);
				}
			}
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
			stnode::alloc_inter *allocNode = static_cast<stnode::alloc_inter *>(*node);
			std::list<stnode::allocUnit_inter>::iterator p, pEnd = allocNode->var.end();
			for (p = allocNode->var.begin(); p != pEnd; p++)
			{
				if (p->init)
				{
					if (p->subCount == 0)
					{
						errInfo err = stAnalyzer_type(p->val, retType);
						if (err.err != NULL)
							return err;
						dataType type1 = idTable[p->varID].type, type2;
						err = getNodeType(p->val, type2, retType);
						if (err.err != NULL)
							return err;
						if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
						{
							return errInfo((*(p->val))->lineN, (*(p->val))->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + "to" + type2Str(type1)));
						}
						if (type2.dType != type1.dType)
						{
							*(p->val) = new stnode::cast(*(p->val), type1);
						}
					}
					else
					{
						errInfo err = noErr;
						dataType type1, type2;
						for (size_t i = 0; i < p->subCount; i++)
						{
							err = stAnalyzer_type(p->val + i, retType);
							if (err.err != NULL)
								return err;
							type1 = idTable[p->varID].type;
							err = getNodeType(p->val + i, type2, retType);
							if (err.err != NULL)
								return err;
							if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
							{
								return errInfo(p->val[i]->lineN, p->val[i]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + "to" + type2Str(type1)));
							}
							if (type2.dType != type1.dType)
							{
								p->val[i] = new stnode::cast(p->val[i], type1);
							}
						}
					}
				}
			}
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

errInfo inter(stTree &sTree, iCodeSeq &ret)
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
		errInfo err = stAnalyzer_type(&pCur, dataType(dataType::SINT, 0));
		if (err.err != NULL)
			return err;
		*p = pCur;
	}

	return noErr;
}
