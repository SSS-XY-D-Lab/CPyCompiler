#include "stdafx.h"
#include "inter.h"

stnode::varType toPtr(stnode::varType type)
{
    switch (type)
    {
		case stnode::varType::VOID:
			return stnode::varType::VOID_PTR;
		case stnode::varType::SINT:
			return stnode::varType::SINT_PTR;
		case stnode::varType::S8:
			return stnode::varType::S8_PTR;
		case stnode::varType::S16:
			return stnode::varType::S16_PTR;
		case stnode::varType::S32:
			return stnode::varType::S32_PTR;
		case stnode::varType::S64:
			return stnode::varType::S64_PTR;
		case stnode::varType::UINT:
			return stnode::varType::UINT_PTR;
		case stnode::varType::U8:
			return stnode::varType::U8_PTR;
		case stnode::varType::U16:
			return stnode::varType::U16_PTR;
		case stnode::varType::U32:
			return stnode::varType::U32_PTR;
		case stnode::varType::U64:
			return stnode::varType::U64_PTR;
		case stnode::varType::VOID_PTR:
			return stnode::varType::VOID_PTR_2;
		case stnode::varType::SINT_PTR:
			return stnode::varType::SINT_PTR_2;
		case stnode::varType::S8_PTR:
			return stnode::varType::S8_PTR_2;
		case stnode::varType::S16_PTR:
			return stnode::varType::S16_PTR_2;
		case stnode::varType::S32_PTR:
			return stnode::varType::S32_PTR_2;
		case stnode::varType::S64_PTR:
			return stnode::varType::S64_PTR_2;
		case stnode::varType::UINT_PTR:
			return stnode::varType::UINT_PTR_2;
		case stnode::varType::U8_PTR:
			return stnode::varType::U8_PTR_2;
		case stnode::varType::U16_PTR:
			return stnode::varType::U16_PTR_2;
		case stnode::varType::U32_PTR:
			return stnode::varType::U32_PTR_2;
		case stnode::varType::U64_PTR:
			return stnode::varType::U64_PTR_2;
    }
    return stnode::varType::_ERROR;
}

stnode::varType getIDType(stnode::id *id)
{
    if (id->subCount > 0)
		return toPtr(id->dtype);
	else
		return id->dtype;
}

struct idItem
{
	idItem(stnode::varType _type, bool _isConst = false){ type = _type; isConst = _isConst; };
	stnode::varType type;
	bool isConst;
};
typedef std::vector<idItem> idTableTp;
idTableTp idTable;
typedef std::unordered_map<int, int> constTableTp;
constTableTp constTable;

typedef std::unordered_map<std::string, int> idHashLayerTp;
typedef std::list<idHashLayerTp*> idHashTableTp;
idHashTableTp idHashTable;

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

#define ERR_NEWID_NOLAYER -1
#define ERR_NEWID_REDEFINE -2
const char* ERR_NEWID_MSG[3] = {
	NULL,
	"Internal Error:No Hash Layer, Please report",
	"Redefinition",
};

int newID(std::string name, stnode::varType type, bool isConst = false)
{
	if (idHashTable.empty())
		return ERR_NEWID_NOLAYER;
	idHashLayerTp *topLayer = idHashTable.back();
	if (topLayer->count(name) > 0)
		return ERR_NEWID_REDEFINE;
	int id = idTable.size();
	topLayer->emplace(name, id);
	idTable[id] = idItem(type, isConst);
	return id;
}

errInfo stAnalyzer(stnode::stnode **node)
{
	switch ((*node)->getType())
	{
		case stnode::type::ID:
		{
			stnode::id *oldNode = dynamic_cast<stnode::id*>(*node);
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
			stnode::op::op *opNode = dynamic_cast<stnode::op::op*>(*node);
			switch (opNode->getOpType())
			{
				case stnode::op::opType::TRIPLE:
				{
					stnode::op::opTriple *opNodeT = dynamic_cast<stnode::op::opTriple*>(opNode);
					errInfo err = noErr;
					err = stAnalyzer(&opNodeT->arg3);
					if (err.err != NULL)
						return err;
					err = stAnalyzer(&opNodeT->arg2);
					if (err.err != NULL)
						return err;
					err = stAnalyzer(&opNodeT->arg1);
					if (err.err != NULL)
						return err;
					break;
				}
				case stnode::op::opType::DOUBLE:
				{
					stnode::op::opDouble *opNodeD = dynamic_cast<stnode::op::opDouble*>(opNode);
					errInfo err = noErr;
					err = stAnalyzer(&opNodeD->arg2);
					if (err.err != NULL)
						return err;
					err = stAnalyzer(&opNodeD->arg1);
					if (err.err != NULL)
						return err;
					break;
				}
				case stnode::op::opType::SINGLE:
				{
					stnode::op::opSingle *opNodeS = dynamic_cast<stnode::op::opSingle*>(opNode);
					errInfo err = noErr;
					err = stAnalyzer(&opNodeS->arg1);
					if (err.err != NULL)
						return err;
					break;
				}
				default:
					return errInfo(opNode->lineN, opNode->pos, "Invalid Operator");
			}
			break;
		}
		case stnode::type::FUNC:
		{
			stnode::func *oldNode = dynamic_cast<stnode::func*>(*node);

			int funcID = newID(oldNode->name, oldNode->retType, true);
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
					argID = newID((*p)->name, getIDType(*p), true);
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
					err = stAnalyzer(&ptr);
					*p = ptr;
				}
				newNode->block = block;
			}

			delete oldNode;
			idHashTable.pop_back();
			*node = newNode;
			break;
		}
		case stnode::type::CALL:
		{
			stnode::call *callNode = dynamic_cast<stnode::call*>(*node);
			errInfo err = stAnalyzer(&(callNode->id));
			if (err.err != NULL)
				return err;
			err = stAnalyzer(&(callNode->args));
			if (err.err != NULL)
				return err;
			break;
		}
		case stnode::type::RETURN:
		{
			stnode::ret *retNode = dynamic_cast<stnode::ret*>(*node);
			errInfo err = stAnalyzer(&(retNode->retVal));
			if (err.err != NULL)
				return err;
			break;
		}
		case stnode::type::IF:
		{
			stnode::ifelse *ifNode = dynamic_cast<stnode::ifelse*>(*node);

			errInfo err = stAnalyzer(&(ifNode->exp));
			if (err.err != NULL)
				return err;

			stTree::iterator p, pEnd;
			stnode::stnode *ptr;

			pEnd = ifNode->blockTrue->end();
			for (p = ifNode->blockTrue->begin(); p != pEnd; p++)
			{
				ptr = *p;
				err = stAnalyzer(&ptr);
				*p = ptr;
			}

			if (ifNode->blockFalse != NULL)
			{
				pEnd = ifNode->blockFalse->end();
				for (p = ifNode->blockFalse->begin(); p != pEnd; p++)
				{
					ptr = *p;
					err = stAnalyzer(&ptr);
					*p = ptr;
				}
			}
			break;
		}
		case stnode::type::ALLOC:
		{
			stnode::alloc *oldNode = dynamic_cast<stnode::alloc*>(*node);
            stnode::alloc_inter *newNode = new stnode::alloc_inter(oldNode->readOnly);

            std::list<stnode::allocUnit>::iterator p, pEnd = oldNode->var.end();
            int varID;
            for (p = oldNode->var.begin(); p != pEnd; p++)
			{
				varID = newID(p->var->name, getIDType(p->var));
				if (varID < 0)
					return errInfo(oldNode->lineN, oldNode->pos, ERR_NEWID_MSG[-varID]);
				if (p->init)
					newNode->var.push_back(stnode::allocUnit_inter(varID, p->var->subCount, p->val));
				else
					newNode->var.push_back(stnode::allocUnit_inter(varID, p->var->subCount));
			}

			oldNode->convert = true;
			delete oldNode;
			*node = newNode;
			break;
		}
		case stnode::type::TREE:
		{
			stnode::expTree *treeNode = dynamic_cast<stnode::expTree*>(*node);
            errInfo err = stAnalyzer(&(treeNode->exp));
            if (err.err != NULL)
				return err;
			err = stAnalyzer(&(treeNode->son));
            if (err.err != NULL)
				return err;
			break;
		}
	}
	return noErr;
}
