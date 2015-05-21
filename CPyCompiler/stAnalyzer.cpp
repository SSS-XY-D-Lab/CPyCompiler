#include "stdafx.h"
#include "st_inter.h"

idLayerTp idGlobal;
funcTableTp funcTable;
std::mutex idMutex, funcMutex;

int getID(std::string name, idItem &ret, idTableTp &idTable)
{
	idTableTp::reverse_iterator pItr, pEnd = idTable.rend();
	idHashTableTp::iterator pRes;
	for (pItr = idTable.rbegin(); pItr != pEnd; pItr++)
	{
		pRes = (*pItr)->hash.find(name);
		if (pRes != (*pItr)->hash.end())
		{
			ret = (*pItr)->table[pRes->second];
			return 0;
		}
	}
	return -1;
}

int getGlobalID(std::string name, size_t &ret)
{
	idHashTableTp::iterator pRes;
	pRes = idGlobal.hash.find(name);
	if (pRes != idGlobal.hash.end())
	{
		ret = pRes->second;
		return 0;
	}
	return -1;
}

const int ERR_NEWID_NOLAYER = -1;
const int ERR_NEWID_REDEFINE = -2;
const char* ERR_NEWID_MSG[3] = {
	NULL,
	"Internal Error:No Hash Layer, Please report",
	"Redefinition",
};

int newID(std::string name, dataType type, size_t &ret, idTableTp &idTable, bool isConst = false)
{
	if (idTable.empty())
		return ERR_NEWID_NOLAYER;
	idLayerTp *topLayer = idTable.back();
	if (topLayer->hash.count(name) > 0)
		return ERR_NEWID_REDEFINE;
	ret = topLayer->table.size();
	topLayer->hash.emplace(name, ret);
	topLayer->table.push_back(idItem(type, isConst, topLayer->shift));
	topLayer->shift += typeSize(type);
	return 0;
}

int newGlobalID(std::string name, dataType type, size_t &ret, bool isConst = false)
{
	if (idGlobal.hash.count(name) > 0)
		return ERR_NEWID_REDEFINE;
	idMutex.lock();
	size_t id = idGlobal.table.size();
	idGlobal.hash.emplace(name, id);
	idGlobal.table.push_back(idItem(type, isConst));
	idMutex.unlock();
	return id;
}

int newGlobalID(std::string name, size_t funcID, size_t &ret)
{
	if (idGlobal.hash.count(name) > 0)
		return ERR_NEWID_REDEFINE;
	idMutex.lock();
	size_t id = idGlobal.table.size();
	idGlobal.hash.emplace(name, id);
	idGlobal.table.push_back(idItem(funcID));
	idMutex.unlock();
	return id;
}

int newFuncID(std::string name, dataType retType, std::vector<dataType> &argType, size_t &ret)
{
	funcMutex.lock();
	size_t funcID = funcTable.size();
	funcTable.push_back(funcItem(retType, argType));
	int err = newGlobalID(name, funcID, ret);
	funcMutex.unlock();
	return err;
}

errInfo stAnalyzer_build(stnode::stnode **node, idTableTp &idTable)
{
	switch ((*node)->getType())
	{
		case stnode::type::ID:
		{
			stnode::id *oldNode = static_cast<stnode::id*>(*node);
			idItem idI(0);
			int err = getID(oldNode->name, idI, idTable);
			if (err == -1)
			{
				size_t id;
				err = getGlobalID(oldNode->name, id);
				if (err == -1)
					return errInfo(oldNode->lineN, oldNode->pos, "Undefined Variant");
				stnode::id_global *newNode = new stnode::id_global(id);
				newNode->lineN = oldNode->lineN;
				newNode->pos = oldNode->pos;
				delete oldNode;
				*node = newNode;
			}
			else
			{
				stnode::id_local *newNode = new stnode::id_local(idI.type, idI.shift);
				newNode->lineN = oldNode->lineN;
				newNode->pos = oldNode->pos;
				delete oldNode;
				*node = newNode;
			}
			break;
		}
		case stnode::type::OP:
		{
			stnode::op::op *opNode = static_cast<stnode::op::op*>(*node);
			for (int i = 0; i < opNode->argCount; i++)
			{
				errInfo err = stAnalyzer_build(&opNode->arg[i], idTable);
				if (err.err)
					return err;
			}
			break;
		}
		case stnode::type::FUNC:
		{
			stnode::func *oldNode = static_cast<stnode::func*>(*node);

			idTable.push_back(new idLayerTp);
			stnode::func_inter *newNode = new stnode::func_inter;
			newNode->lineN = oldNode->lineN;
			newNode->pos = oldNode->pos;
			newNode->retType = oldNode->retType;
			std::vector<dataType> argType;

			{
				std::list<stnode::id*>::iterator p, pEnd = oldNode->args.end();
				size_t argID;
				int err;
				for (p = oldNode->args.begin(); p != pEnd; p++)
				{
					err = newID((*p)->name, (*p)->dtype, argID, idTable, true);
					if (err < 0)
						return errInfo((*p)->lineN, (*p)->pos, ERR_NEWID_MSG[-err]);
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
					err = stAnalyzer_build(&ptr, idTable);
					if (err.err)
						return err;
					*p = ptr;
				}
				newNode->block = block;
			}

			size_t funcID;
			int errCode = newFuncID(oldNode->name, oldNode->retType, argType, funcID);
			if (errCode < 0)
				return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-errCode]);
			newNode->funcID = funcID;

			delete oldNode;
			delete idTable.back();
			idTable.pop_back();
			*node = newNode;
			break;
		}
		case stnode::type::CALL:
		{
			stnode::call *callNode = static_cast<stnode::call*>(*node);
			stnode::stnode *tmpPtr = callNode->funcID;
			errInfo err = stAnalyzer_build(&tmpPtr, idTable);
			if (err.err)
				return err;
			if (tmpPtr->getType() != stnode::type::ID_GLOBAL)
				return errInfo(tmpPtr->lineN, tmpPtr->pos, "Call of non-function");

			err = stAnalyzer_build(&(callNode->args), idTable);
			if (err.err)
				return err;
			std::list<stnode::stnode*> argList;
			stnode::stnode* ptr = callNode->args;
			while (ptr->getType() != stnode::type::TREE)
			{
				argList.push_front(static_cast<stnode::expTree*>(ptr)->exp);
				ptr = static_cast<stnode::expTree*>(ptr)->prev;
			}

			stnode::call_inter *newNode = new stnode::call_inter(static_cast<stnode::id_global*>(tmpPtr)->id, argList);
			newNode->lineN = callNode->lineN;
			newNode->pos = callNode->pos;
			delete callNode;
			*node = newNode;
			break;
		}
		case stnode::type::RETURN:
		{
			stnode::ret *retNode = static_cast<stnode::ret*>(*node);
			if (retNode->retVal != NULL)
			{
				errInfo err = stAnalyzer_build(&(retNode->retVal), idTable);
				if (err.err)
					return err;
			}
			break;
		}
		case stnode::type::IF:
		{
			stnode::ifelse *ifNode = static_cast<stnode::ifelse*>(*node);
			size_t shift = idTable.back()->shift;
			idTable.push_back(new idLayerTp);
			idTable.back()->shift = shift;

			errInfo err = stAnalyzer_build(&(ifNode->exp), idTable);
			if (err.err)
				return err;

			stTree::iterator p, pEnd;
			stnode::stnode *ptr;

			pEnd = ifNode->blockTrue->end();
			for (p = ifNode->blockTrue->begin(); p != pEnd; p++)
			{
				ptr = *p;
				err = stAnalyzer_build(&ptr, idTable);
				*p = ptr;
			}

			if (ifNode->blockFalse != NULL)
			{
				pEnd = ifNode->blockFalse->end();
				for (p = ifNode->blockFalse->begin(); p != pEnd; p++)
				{
					ptr = *p;
					err = stAnalyzer_build(&ptr, idTable);
					*p = ptr;
				}
			}

			delete idTable.back();
			idTable.pop_back();
			break;
		}
		case stnode::type::ALLOC:
		{
			stnode::alloc *oldNode = static_cast<stnode::alloc*>(*node);
			if (oldNode->isGlobal)
			{
				stnode::alloc_global *newNode = new stnode::alloc_global(oldNode->isConst);

				std::list<stnode::allocUnit>::iterator p, pEnd = oldNode->var.end();
				size_t varID;
				int errCode;
				for (p = oldNode->var.begin(); p != pEnd; p++)
				{
					if (p->subCount > 0)
						errCode = newGlobalID(p->var->name, toRef(p->var->dtype), varID);
					else
						errCode = newGlobalID(p->var->name, p->var->dtype, varID);
					if (errCode < 0)
						return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-errCode]);
					if (p->init)
						newNode->var.push_back(stnode::allocUnit_global(varID, p->subCount, p->val));
					else
						newNode->var.push_back(stnode::allocUnit_global(varID, p->subCount));
				}

				oldNode->convert = true;
				newNode->lineN = oldNode->lineN;
				newNode->pos = oldNode->pos;
				delete oldNode;
				*node = newNode;
			}
			else
			{
				stnode::alloc_local *newNode = new stnode::alloc_local(oldNode->isConst);

				std::list<stnode::allocUnit>::iterator p, pEnd = oldNode->var.end();
				size_t varID;
				dataType varType;
				int errCode;
				for (p = oldNode->var.begin(); p != pEnd; p++)
				{
					if (p->subCount > 0)
						varType = toRef(p->var->dtype);
					else
						varType = p->var->dtype;
					errCode = newID(p->var->name, varType, varID, idTable);
					if (errCode < 0)
						return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-errCode]);
					if (p->init)
						newNode->var.push_back(stnode::allocUnit_local(varType, p->subCount, p->val));
					else
						newNode->var.push_back(stnode::allocUnit_local(varType, p->subCount));
				}

				oldNode->convert = true;
				newNode->lineN = oldNode->lineN;
				newNode->pos = oldNode->pos;
				delete oldNode;
				*node = newNode;
			}
			
			break;
		}
		case stnode::type::TREE:
		{
			stnode::expTree *treeNode = static_cast<stnode::expTree*>(*node);
			errInfo err = stAnalyzer_build(&(treeNode->exp), idTable);
			if (err.err)
				return err;
			err = stAnalyzer_build(&(treeNode->prev), idTable);
			if (err.err)
				return err;
			break;
		}
	}
	return noErr;
}

errInfo getNodeType(stnode::stnode **node, dataType &type, dataType retType)
{
	stnode::stnode *ptr = *node;
	if (ptr == NULL)
		type = dataType(dataType::VOID, 0);
	else
	{
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
			case stnode::type::ID_GLOBAL:
			{
				type = idGlobal.table[static_cast<stnode::id_global*>(ptr)->id].type;
				break;
			}
			case stnode::type::ID_LOCAL:
			{
				type = static_cast<stnode::id_local*>(ptr)->dtype;
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
					if (err.err)
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
				idItem funcItem = idGlobal.table[static_cast<stnode::call_inter*>(ptr)->funcID];
				if (!funcItem.isFunc)
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
						if (err.err)
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
								return errInfo(opNode->arg[i]->lineN, opNode->arg[i]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(types[i]) + " to " + type2Str(type)));
							}
							if (typeLvl(types[i]) < typeLvl(type) && types[i].isConst == false)
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
					if (err.err)
						return err;
					if (type2.dType == dataType::VOID && type2.ptrLvl == 0)
						return errInfo(opNode->arg[1]->lineN, opNode->arg[1]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + " to " + type2Str(type1)));
					if (type2.isConst == false)
					{
						err = getNodeType(&opNode->arg[0], type1, retType);
						if (err.err)
							return err;
						if (type1.ptrLvl != type2.ptrLvl || (type1.dType == dataType::VOID && type1.ptrLvl == 0))
						{
							return errInfo(opNode->arg[1]->lineN, opNode->arg[1]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + " to " + type2Str(type1)));
						}
						if (type2.dType != type1.dType && type2.isConst == false)
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
							if (err.err)
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
							if (err.err)
								return err;
							if (type.ptrLvl < 1)
								return errInfo(opNode->arg[1]->lineN, opNode->arg[1]->pos, "Need a pointer");
							opNode->resType = deref(type);
							break;
						}
						case stnode::op::ops::REF:
						{
							if (opNode->argCount != 1)
								return errInfo(opNode->lineN, opNode->pos, "Operator must have 1 arg");
							errInfo err = getNodeType(&opNode->arg[0], type, retType);
							if (err.err)
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
				if (err.err)
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
			size_t funcID = callNode->funcID;
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
			if (err.err)
				return err;
			if ((valType.dType == dataType::VOID && valType.ptrLvl == 0) ^ (retType.dType == dataType::VOID && retType.ptrLvl == 0))
				return errInfo(retNode->retVal->lineN, retNode->retVal->pos, str2cstr(std::string("Invalid cast from ") + type2Str(valType) + " to " + type2Str(retType)));
			if (valType.isConst == false)
			{
				if (retType.ptrLvl != valType.ptrLvl)
				{
					return errInfo(retNode->retVal->lineN, retNode->retVal->pos, str2cstr(std::string("Invalid cast from ") + type2Str(valType) + " to " + type2Str(retType)));
				}
				if (retType.dType != valType.dType && valType.isConst == false)
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
			if (err.err)
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
		case stnode::type::ALLOC_GLOBAL:
		{
			stnode::alloc_global *allocNode = static_cast<stnode::alloc_global *>(*node);
			std::list<stnode::allocUnit_global>::iterator p, pEnd = allocNode->var.end();
			for (p = allocNode->var.begin(); p != pEnd; p++)
			{
				if (p->init)
				{
					if (p->subCount == 0)
					{
						errInfo err = stAnalyzer_type(p->val, retType);
						if (err.err)
							return err;
						dataType type1 = idGlobal.table[p->varID].type, type2;
						err = getNodeType(p->val, type2, retType);
						if (err.err)
							return err;
						if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
						{
							return errInfo((*(p->val))->lineN, (*(p->val))->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + " to " + type2Str(type1)));
						}
						if (type2.dType != type1.dType && type2.isConst == false)
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
							if (err.err)
								return err;
							type1 = idGlobal.table[p->varID].type;
							err = getNodeType(p->val + i, type2, retType);
							if (err.err)
								return err;
							if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
							{
								return errInfo(p->val[i]->lineN, p->val[i]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + " to " + type2Str(type1)));
							}
							if (type2.dType != type1.dType && type2.isConst == false)
							{
								p->val[i] = new stnode::cast(p->val[i], type1);
							}
						}
					}
				}
			}
			break;
		}
		case stnode::type::ALLOC_LOCAL:
		{
			stnode::alloc_local *allocNode = static_cast<stnode::alloc_local *>(*node);
			std::list<stnode::allocUnit_local>::iterator p, pEnd = allocNode->var.end();
			for (p = allocNode->var.begin(); p != pEnd; p++)
			{
				if (p->init)
				{
					if (p->subCount == 0)
					{
						errInfo err = stAnalyzer_type(p->val, retType);
						if (err.err)
							return err;
						dataType type1 = p->dtype, type2;
						err = getNodeType(p->val, type2, retType);
						if (err.err)
							return err;
						if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
						{
							return errInfo((*(p->val))->lineN, (*(p->val))->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + " to " + type2Str(type1)));
						}
						if (type2.dType != type1.dType && type2.isConst == false)
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
							if (err.err)
								return err;
							type1 = p->dtype;
							err = getNodeType(p->val + i, type2, retType);
							if (err.err)
								return err;
							if (type1.ptrLvl != type2.ptrLvl || (type2.dType == dataType::VOID && type2.ptrLvl == 0))
							{
								return errInfo(p->val[i]->lineN, p->val[i]->pos, str2cstr(std::string("Invalid cast from ") + type2Str(type2) + " to " + type2Str(type1)));
							}
							if (type2.dType != type1.dType && type2.isConst == false)
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
			if (err.err)
				return err;
			err = stAnalyzer_type(&(treeNode->prev), retType);
			if (err.err)
				return err;
			break;
		}
	}
	return noErr;
}
