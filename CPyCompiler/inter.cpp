#include "stdafx.h"
#include "st_inter.h"

strTableTp strTable;
//constTableTp constTable;

std::mutex strMutex, labelMutex;
size_t labelCount = 0;

size_t newStr(const std::string &str)
{
	strMutex.lock();
	size_t id = strTable.size();
	strTable.push_back(str);
	strMutex.unlock();
	return id;
}

size_t newLbl()
{
	labelMutex.lock();
	size_t id = labelCount;
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

struct retValTp
{
	iCode::arg *val;
	dataType type;
};

errInfo inter_gen(stnode::stnode*, iCodeSeq &, size_t &, size_t, retValTp &);

struct label_if
{
	label_if(){ valid = false; }
	label_if(size_t _id){ valid = true; id = _id; }
	bool valid;
	size_t id;
};

iCode::ifType operator~(iCode::ifType arg)
{
	switch (arg)
	{
		case iCode::ifType::IFE:
			return iCode::ifType::IFN;
		case iCode::ifType::IFN:
			return iCode::ifType::IFE;
		case iCode::ifType::IFG:
			return iCode::ifType::IFLE;
		case iCode::ifType::IFGE:
			return iCode::ifType::IFL;
		case iCode::ifType::IFL:
			return iCode::ifType::IFGE;
		case iCode::ifType::IFLE:
			return iCode::ifType::IFG;
	}
	return iCode::ifType::IFE;
}

errInfo inter_if(stnode::stnode* node, iCodeSeq &ret, size_t &sp, size_t spOrigin, label_if yesLabel, label_if noLabel)
{
	if (node == NULL)
		return errInfo(0, 0, "NULL Pointer");
	if (node->getType() == stnode::OP)
	{
		stnode::op::op* exp = static_cast<stnode::op::op*>(node);
		errInfo err;
		iCode::ifType ifOp;
		switch (exp->opVal)
		{
			case stnode::op::ops::LGNOT:
				if (exp->argCount != 1) return errInfo(exp->lineN, exp->pos, "Need 1 arg");
				err = inter_if(exp->arg[0], ret, sp, spOrigin, noLabel, yesLabel);
				if (err.err)
					return err;
				break;
			case stnode::op::ops::LGAND:
				if (exp->argCount != 2) return errInfo(exp->lineN, exp->pos, "Need 2 args");
				err = inter_if(exp->arg[0], ret, sp, spOrigin, label_if(), noLabel);
				if (err.err)
					return err;
				err = inter_if(exp->arg[1], ret, sp, spOrigin, label_if(), noLabel);
				if (err.err)
					return err;
				if (yesLabel.valid)
					ret.push_back(new iCode::jump(yesLabel.id));
				break;
			case stnode::op::ops::LGOR:
				if (exp->argCount != 2) return errInfo(exp->lineN, exp->pos, "Need 2 args");
				err = inter_if(exp->arg[0], ret, sp, spOrigin, yesLabel, label_if());
				if (err.err)
					return err;
				err = inter_if(exp->arg[1], ret, sp, spOrigin, yesLabel, label_if());
				if (err.err)
					return err;
				if (noLabel.valid)
					ret.push_back(new iCode::jump(noLabel.id));
				break;
			case stnode::op::ops::BIG:
				ifOp = iCode::ifType::IFG;
				goto process_if;
			case stnode::op::ops::BIGEQU:
				ifOp = iCode::IFGE;
				goto process_if;
			case stnode::op::ops::LES:
				ifOp = iCode::IFL;
				goto process_if;
			case stnode::op::ops::LESEQU:
				ifOp = iCode::IFLE;
				goto process_if;
			case stnode::op::ops::EQU:
				ifOp = iCode::IFE;
				goto process_if;
			case stnode::op::ops::NEQU:
				ifOp = iCode::IFN;
			process_if:
				{
					if (exp->argCount != 2) return errInfo(exp->lineN, exp->pos, "Need 2 args");
					iCode::arg *args[2];
					retValTp argRet;
					for (int i = 0; i < 2; i++)
					{
						err = inter_gen(exp->arg[i], ret, sp, spOrigin, argRet);
						if (err.err)
							return err;
						args[i] = argRet.val;
						if (argRet.val->getType() == iCode::argType::TEMP)
							static_cast<iCode::temp*>(argRet.val)->ref++;
					}
					if (yesLabel.valid)
					{
						ret.push_back(new iCode::jump_if(ifOp, yesLabel.id, args[0], args[1]));
						if (noLabel.valid)
							ret.push_back(new iCode::jump(noLabel.id));
					}
					else if (noLabel.valid)
					{
						ret.push_back(new iCode::jump_if(~ifOp, noLabel.id, args[0], args[1]));
					}
				}
				break;
			default:
			{
				retValTp retVal;
				errInfo err = inter_gen(node, ret, sp, spOrigin, retVal);
				if (err.err)
					return err;
				if (retVal.val->getType() == iCode::argType::TEMP)
					static_cast<iCode::temp*>(retVal.val)->ref++;
				if (yesLabel.valid)
				{
					ret.push_back(new iCode::jump_if(iCode::IFN, yesLabel.id, retVal.val, new iCode::con(0)));
					if (noLabel.valid)
						ret.push_back(new iCode::jump(noLabel.id));
				}
				else if (noLabel.valid)
				{
					ret.push_back(new iCode::jump_if(iCode::IFE, noLabel.id, retVal.val, new iCode::con(0)));
				}
			}
		}
	}
	else
	{
		retValTp retVal;
		errInfo err = inter_gen(node, ret, sp, spOrigin, retVal);
		if (err.err)
			return err;
		if (retVal.val->getType() == iCode::argType::TEMP)
			static_cast<iCode::temp*>(retVal.val)->ref++;
		if (yesLabel.valid)
		{
			ret.push_back(new iCode::jump_if(iCode::IFN, yesLabel.id, retVal.val, new iCode::con(0)));
			if (noLabel.valid)
				ret.push_back(new iCode::jump(noLabel.id));
		}
		else if (noLabel.valid)
		{
			ret.push_back(new iCode::jump_if(iCode::IFE, noLabel.id, retVal.val, new iCode::con(0)));
		}
	}
	return noErr;
}

errInfo inter_gen(stnode::stnode* node, iCodeSeq &ret, size_t &sp, size_t spOrigin, retValTp &retVal)
{
	if (node == NULL)
		return errInfo(0, 0, "NULL Pointer");
	switch (node->getType())
	{
		case stnode::type::NUMBER:
		{
			long long val = static_cast<stnode::number *>(node)->val;
			retVal.val = new iCode::con(val);
			retVal.type = dataType(minNum(val), 0, true);;
			break;
		}
		case stnode::type::CHARA:
		{
			retVal.val = new iCode::con(static_cast<stnode::chara *>(node)->ch);
			retVal.type = dataType(dataType::S8, 0);
			break;
		}
		case stnode::type::STR:
		{
			size_t id = newStr(static_cast<stnode::str *>(node)->strr);
			retVal.val = new iCode::str(id);
			retVal.type = dataType(dataType::S8, 1);
			break;
		}
		case stnode::type::ID_GLOBAL:
		{
			size_t id = static_cast<stnode::id_global *>(node)->id;
			retVal.val = new iCode::id_global(id);
			retVal.type = idGlobal.table[id].type;
			break;
		}
		case stnode::type::ID_LOCAL:
		{
			stnode::id_local *idNode = static_cast<stnode::id_local *>(node);
			iCode::id_local *newIDNode = new iCode::id_local(sp - idNode->shift - typeSize(idNode->dtype) + 1);
			retVal.val = newIDNode;
			retVal.type = idNode->dtype;
			break;
		}
		case stnode::type::OP:
		{
			stnode::op::op *opNode = static_cast<stnode::op::op *>(node);
			int needArg = 3;

			if (opNode->getOpType() == stnode::op::opType::LOGICAL)
			{
				iCode::temp *tmpArg = new iCode::temp;
				iCode::code *code = new iCode::code(iCode::SET, tmpArg, new iCode::con(0));
				tmpArg->pCode.push_back(code);
				ret.push_back(code);
				size_t noLabel = newLbl();
				errInfo err = inter_if(node, ret, sp, spOrigin, label_if(), noLabel);
				if (err.err)
					return err;
				code = new iCode::code(iCode::SET, tmpArg, new iCode::con(-1));
				tmpArg->pCode.push_back(code);
				ret.push_back(code);
				ret.push_back(new iCode::label(noLabel));
			}
			else
			{
				iCode::opType op = iCode::opType::NUL;
				switch (opNode->opVal)
				{
					case stnode::op::ops::ARRAY_SUB:
						needArg = 2;
						op = iCode::opType::R_ADD;
						break;
					case stnode::op::ops::POSI:
						needArg = 1;
						op = iCode::opType::ADD;
						break;
					case stnode::op::ops::NEGA:
						needArg = 1;
						op = iCode::opType::SUB;
						break;
					case stnode::op::ops::INC_POST:
						needArg = 1;
						op = iCode::opType::ADD;
						break;
					case stnode::op::ops::DEC_POST:
						needArg = 1;
						op = iCode::opType::SUB;
						break;
					case stnode::op::ops::INC_PRE:
						needArg = 1;
						op = iCode::opType::ADD;
						break;
					case stnode::op::ops::DEC_PRE:
						needArg = 1;
						op = iCode::opType::SUB;
						break;
					case stnode::op::ops::REF:
						needArg = 1;
						op = iCode::opType::G_ADD;
						break;
					case stnode::op::ops::DEREF:
						needArg = 1;
						op = iCode::opType::I_ADD;
						break;
					case stnode::op::ops::NOT:
						needArg = 1;
						op = iCode::opType::NOT;
						break;
					case stnode::op::ops::DIV:
						needArg = 2;
						op = iCode::opType::DIV;
						break;
					case stnode::op::ops::MUL:
						needArg = 2;
						op = iCode::opType::MUL;
						break;
					case stnode::op::ops::MOD:
						needArg = 2;
						op = iCode::opType::MOD;
						break;
					case stnode::op::ops::ADD:
						needArg = 2;
						op = iCode::opType::ADD;
						break;
					case stnode::op::ops::SUB:
						needArg = 2;
						op = iCode::opType::SUB;
						break;
					case stnode::op::ops::SHL:
						needArg = 2;
						op = iCode::opType::SHL;
						break;
					case stnode::op::ops::SHR:
						needArg = 2;
						op = iCode::opType::SHR;
						break;
					case stnode::op::ops::AND:
						needArg = 2;
						op = iCode::opType::AND;
						break;
					case stnode::op::ops::XOR:
						needArg = 2;
						op = iCode::opType::XOR;
						break;
					case stnode::op::ops::BOR:
						needArg = 2;
						op = iCode::opType::BOR;
						break;
					case stnode::op::ops::ASSIGN:
						needArg = 2;
						op = iCode::opType::SET;
						break;
					case stnode::op::ops::MODASS:
						needArg = 2;
						op = iCode::opType::MOD;
						break;
					case stnode::op::ops::DIVASS:
						needArg = 2;
						op = iCode::opType::DIV;
						break;
					case stnode::op::ops::MULASS:
						needArg = 2;
						op = iCode::opType::MUL;
						break;
					case stnode::op::ops::ADDASS:
						needArg = 2;
						op = iCode::opType::ADD;
						break;
					case stnode::op::ops::SUBASS:
						needArg = 2;
						op = iCode::opType::SUB;
						break;
					case stnode::op::ops::SHLASS:
						needArg = 2;
						op = iCode::opType::SHL;
						break;
					case stnode::op::ops::SHRASS:
						needArg = 2;
						op = iCode::opType::SHR;
						break;
					case stnode::op::ops::ANDASS:
						needArg = 2;
						op = iCode::opType::AND;
						break;
					case stnode::op::ops::XORASS:
						needArg = 2;
						op = iCode::opType::XOR;
						break;
					case stnode::op::ops::BORASS:
						needArg = 2;
						op = iCode::opType::BOR;
						break;
				}

				if (opNode->argCount != needArg)
					return errInfo(opNode->lineN, opNode->pos, str2cstr("Need " + num2str(needArg) + " arg"));

				iCode::arg *args[3];
				retValTp argRet;
				for (int i = 0; i < needArg; i++)
				{
					errInfo err = inter_gen(opNode->arg[i], ret, sp, spOrigin, argRet);
					if (err.err)
						return err;
					args[i] = argRet.val;
					if (argRet.val->getType() == iCode::argType::TEMP)
						static_cast<iCode::temp*>(argRet.val)->ref++;
				}
				retVal.type = argRet.type;
				iCode::code *code = NULL;

				switch (opNode->opVal)
				{
					case stnode::op::ops::POSI:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(iCode::opType::SET, tmpArg, args[0]);
						ret.push_back(code);
						tmpArg->pCode.push_back(code);
						retVal.val = tmpArg;
						break;
					}
					case stnode::op::ops::NEGA:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(iCode::opType::SUB, tmpArg, new iCode::con(0), args[0]);
						ret.push_back(code);
						tmpArg->pCode.push_back(code);
						retVal.val = tmpArg;
						break;
					}
					case stnode::op::ops::INC_POST:
					case stnode::op::ops::DEC_POST:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(op, tmpArg, args[0], new iCode::con(1));
						ret.push_back(code);
						tmpArg->pCode.push_back(code);
						retVal.val = args[0];
						break;
					}
					case stnode::op::ops::INC_PRE:
					case stnode::op::ops::DEC_PRE:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(op, tmpArg, args[0], new iCode::con(1));
						ret.push_back(code);
						tmpArg->pCode.push_back(code);
						retVal.val = tmpArg;
						break;
					}
					default:
					{
						if (needArg == 1)
						{
							iCode::temp *tmpArg = new iCode::temp;
							code = new iCode::code(op, tmpArg, args[0]);
							ret.push_back(code);
							tmpArg->pCode.push_back(code);
							retVal.val = tmpArg;
						}
						else if (needArg == 2)
						{
							if (opNode->getOpType() == stnode::op::opType::ASSIGNMENT)
							{
								if (op == iCode::opType::SET)
								{
									code = new iCode::code(iCode::opType::SET, args[0], args[1]);
									ret.push_back(code);
								}
								else
								{
									code = new iCode::code(op, args[0], args[0], args[1]);
									ret.push_back(code);
								}
								if (args[0]->getType() == iCode::argType::TEMP)
								{
									iCode::temp *tmpArg = static_cast<iCode::temp*>(args[0]);
									tmpArg->ref--;
									tmpArg->pCode.push_back(code);
								}
								retVal.val = args[0];
							}
							else
							{
								iCode::temp *tmpArg = new iCode::temp;
								code = new iCode::code(op, tmpArg, args[0], args[1]);
								ret.push_back(code);
								tmpArg->pCode.push_back(code);
								retVal.val = tmpArg;
							}
						}
					}
				}
				code->argType = argRet.type;
				code->retType = opNode->resType;
			}
			break;
		}
		case stnode::type::FUNC_INTER:
		{
			stnode::func_inter *funcNode = static_cast<stnode::func_inter *>(node);
			size_t funcLabel = newLbl();
			funcTable[funcNode->funcID].labelNo = funcLabel;
			stTree *block = funcNode->block;
			stTree::iterator p = block->begin(), pEnd = block->end();
			retValTp retVar;
			ret.push_back(new iCode::label(funcLabel));
			size_t spO = sp;
			for (; p != pEnd; p++)
			{
				errInfo err = inter_gen(*p, ret, sp, spO, retVar);
				if (err.err)
					return err;
			}
			retVal.val = NULL;
			retVal.type = dataType(dataType::VOID, 0);
			break;
		}
		case stnode::type::CALL_INTER:
		{
			stnode::call_inter *callNode = static_cast<stnode::call_inter *>(node);
			funcItem &funcInfo = funcTable[callNode->funcID];

			retValTp argRet;
			iCode::code *newCode;
			errInfo err;
			size_t i = 0, argSize = 0;
			std::for_each(callNode->args.begin(), callNode->args.end(), [&](stnode::stnode *node){
				err = inter_gen(node, ret, sp, spOrigin, argRet);
				if (argRet.val->getType() == iCode::argType::TEMP)
					static_cast<iCode::temp*>(argRet.val)->ref++;
				newCode = new iCode::code(iCode::opType::PUSH, NULL, argRet.val);
				newCode->argType = funcInfo.argType[i];
				ret.push_back(newCode);
				argSize += typeSize(funcInfo.argType[i]);
				i++;
			});
			if (funcInfo.retType.dType == dataType::VOID && funcInfo.retType.ptrLvl == 0)
				ret.push_back(new iCode::jump(funcInfo.labelNo));
			else
			{
				iCode::temp *tmpNode = new iCode::temp;
				iCode::call *code = new iCode::call(funcInfo.labelNo, tmpNode);
				ret.push_back(code);
				tmpNode->pCode.push_back(code);
			}

			ret.push_back(new iCode::code(iCode::opType::SP_SUB, NULL, new iCode::con(argSize)));
			break;
		}
		case stnode::type::RETURN:
		{
			stnode::ret *retNode = static_cast<stnode::ret *>(node);
			retValTp argRet;

			if (retNode->retVal == NULL)
				argRet.val = NULL;
			else
			{
				errInfo err = inter_gen(retNode->retVal, ret, sp, spOrigin, argRet);
				if (err.err)
					return err;
				if (argRet.val->getType() == iCode::argType::TEMP)
					static_cast<iCode::temp*>(argRet.val)->ref++;
			}

			ret.push_back(new iCode::code(iCode::opType::SP_SUB, NULL, new iCode::con(sp - spOrigin)));
			ret.push_back(new iCode::ret(argRet.val));

			break;
		}
		case stnode::type::ALLOC_GLOBAL:
		{
			stnode::alloc_global *allocNode = static_cast<stnode::alloc_global *>(node);
			std::list<stnode::allocUnit_global>::iterator p = allocNode->var.begin(), pEnd = allocNode->var.end();
			size_t i;
			for (; p != pEnd; p++)
			{
				if (p->init)
				{
					retValTp argRet;
					errInfo err;
					if (p->subCount == 0)
					{
						err = inter_gen(*(p->val), ret, sp, spOrigin, argRet);
						if (err.err)
							return err;
						if (argRet.val->getType() == iCode::argType::TEMP)
							static_cast<iCode::temp*>(argRet.val)->ref++;
						ret.push_back(new iCode::code(iCode::opType::SET, new iCode::id_global(p->varID), argRet.val));
					}
					else
					{
						int tSize = typeSize(idGlobal.table[p->varID].type);
						size_t shift = 0;
						for (i = 0; i < p->subCount; i++)
						{
							if (p->val[i] == NULL)
								break;
							err = inter_gen(p->val[i], ret, sp, spOrigin, argRet);
							if (err.err)
								return err;
							if (argRet.val->getType() == iCode::argType::TEMP)
								static_cast<iCode::temp*>(argRet.val)->ref++;
							ret.push_back(new iCode::code(iCode::opType::SET, new iCode::cshift(new iCode::id_global(p->varID), shift), argRet.val));
							shift += tSize;
						}
					}
				}
			}
			break;
		}
		case stnode::type::ALLOC_LOCAL:
		{
			stnode::alloc_local *allocNode = static_cast<stnode::alloc_local *>(node);
			std::list<stnode::allocUnit_local>::iterator p = allocNode->var.begin(), pEnd = allocNode->var.end();
			size_t i, sp_add = 0;
			for (; p != pEnd; p++)
			{
				retValTp argRet;
				errInfo err;
				if (p->subCount == 0)
				{
					if (p->init)
					{
						if (sp_add != 0)
						{
							ret.push_back(new iCode::code(iCode::opType::SP_ADD, NULL, new iCode::con(sp_add)));
							sp_add = 0;
						}
						err = inter_gen(*p->val, ret, sp, spOrigin, argRet);
						if (err.err)
							return err;
						if (argRet.val->getType() == iCode::argType::TEMP)
							static_cast<iCode::temp*>(argRet.val)->ref++;
						iCode::code *newCode = new iCode::code(iCode::opType::PUSH, NULL, argRet.val);
						newCode->argType = p->dtype;
						ret.push_back(newCode);
					}
					else
						sp_add += typeSize(p->dtype);
					sp += typeSize(p->dtype);
				}
				else
				{
					int tSize = typeSize(p->dtype);
					size_t shift = sp;
					sp += tSize * p->subCount;
					if (p->init)
					{
						if (sp_add != 0)
						{
							ret.push_back(new iCode::code(iCode::opType::SP_ADD, NULL, new iCode::con(sp_add)));
							sp_add = 0;
						}
						for (i = 0; i < p->subCount; i++)
						{
							if (p->val[i] == NULL)
								break;
							err = inter_gen(p->val[i], ret, sp, spOrigin, argRet);
							if (err.err)
								return err;
							if (argRet.val->getType() == iCode::argType::TEMP)
								static_cast<iCode::temp*>(argRet.val)->ref++;
							iCode::code *newCode = new iCode::code(iCode::opType::PUSH, NULL, argRet.val);
							newCode->argType = p->dtype;
							ret.push_back(newCode);
							shift += tSize;
						}
					}
					else
						sp_add += tSize * p->subCount;
				}
			}
			if (sp_add != 0)
				ret.push_back(new iCode::code(iCode::opType::SP_ADD, NULL, new iCode::con(sp_add)));
			break;
		}
		case stnode::type::CAST:
		{
			stnode::cast *castNode = static_cast<stnode::cast *>(node);

			retValTp argRet;
			errInfo err = inter_gen(castNode->node, ret, sp, spOrigin, argRet);
			if (err.err)
				return err;
			if (argRet.val->getType()==iCode::argType::TEMP)
				static_cast<iCode::temp*>(argRet.val)->ref++;
			
			iCode::temp *tmpArg = new iCode::temp;
			iCode::code *code = new iCode::code(iCode::opType::SET, tmpArg, argRet.val);
			code->argType = argRet.type;
			code->retType = castNode->vtype;
			tmpArg->pCode.push_back(code);
			ret.push_back(code);
			
			retVal.val = tmpArg;
			retVal.type = castNode->vtype;
			break;
		}
		case stnode::type::IF:
		{
			stnode::ifelse *ifNode = static_cast<stnode::ifelse *>(node);

			size_t yesLabel = newLbl(), noLabel = newLbl();
			errInfo err = inter_if(ifNode->exp, ret, sp, spOrigin, yesLabel, noLabel);
			if (err.err)
				return err;
			if (ifNode->blockFalse != NULL)
			{
				size_t endLabel = newLbl();
				ret.push_back(new iCode::label(yesLabel));

				stTree *block = ifNode->blockTrue;
				stTree::iterator p = block->begin(), pEnd = block->end();
				retValTp retVar;
				for (; p != pEnd; p++)
				{
					errInfo err = inter_gen(*p, ret, sp, spOrigin, retVar);
					if (err.err)
						return err;
				}

				ret.push_back(new iCode::jump(endLabel));
				ret.push_back(new iCode::label(noLabel));

				block = ifNode->blockFalse;
				p = block->begin(), pEnd = block->end();
				for (; p != pEnd; p++)
				{
					errInfo err = inter_gen(*p, ret, sp, spOrigin, retVar);
					if (err.err)
						return err;
				}

				ret.push_back(new iCode::label(endLabel));
			}
			else
			{
				ret.push_back(new iCode::label(yesLabel));

				stTree *block = ifNode->blockTrue;
				stTree::iterator p = block->begin(), pEnd = block->end();
				retValTp retVar;
				for (; p != pEnd; p++)
				{
					errInfo err = inter_gen(*p, ret, sp, spOrigin, retVar);
					if (err.err)
						return err;
				}

				ret.push_back(new iCode::label(noLabel));
			}

			break;
		}
		case stnode::type::TREE:
		{
			stnode::expTree *treeNode = static_cast<stnode::expTree *>(node);
			retValTp retVar;
			errInfo err = inter_gen(treeNode->prev, ret, sp, spOrigin, retVar);
			if (err.err)
				return err;
			err = inter_gen(treeNode->exp, ret, sp, spOrigin, retVar);
			if (err.err)
				return err;
			retVal = retVar;
		}
	}
	return noErr;
}

errInfo inter(stTree &sTree, iCodeSeq &ret, dataType retType)
{
	stTree::iterator p, pEnd = sTree.end();
	stnode::stnode *pCur;

	idTableTp idTable;
	idTable.push_back(new idLayerTp);
	for (p = sTree.begin(); p != pEnd; p++)
	{
		pCur = *p;
		errInfo err = stAnalyzer_build(&pCur, idTable);
		if (err.err)
			return err;
		*p = pCur;
	}
	delete idTable.back();
	idTable.pop_back();

	for (p = sTree.begin(); p != pEnd; p++)
	{
		pCur = *p;
		errInfo err = stAnalyzer_type(&pCur, retType);
		if (err.err)
			return err;
		*p = pCur;
	}

	retValTp retVal;
	size_t sp = -1;
	for (p = sTree.begin(); p != pEnd; p++)
	{
		errInfo err = inter_gen(*p, ret, sp, 0, retVal);
		if (err.err)
			return err;
	}

	return noErr;
}
