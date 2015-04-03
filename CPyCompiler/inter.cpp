#include "stdafx.h"
#include "st_inter.h"

strTableTp strTable;
//constTableTp constTable;

std::mutex strMutex, labelMutex;
int labelCount = 0;

size_t newStr(const std::string &str)
{
	strMutex.lock();
	size_t id = strTable.size();
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

struct retValTp
{
	iCode::arg *val;
	dataType type;
};

errInfo inter_gen(stnode::stnode*, iCodeSeq &, retValTp &);

errInfo inter_if(stnode::stnode* node, iCodeSeq &ret, size_t yesLabel, size_t noLabel)
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
				err = inter_if(exp->arg[0], ret, noLabel, yesLabel);
				if (err.err)
					return err;
				break;
			case stnode::op::ops::LGAND:
				if (exp->argCount != 2) return errInfo(exp->lineN, exp->pos, "Need 2 args");
				err = inter_if(exp->arg[0], ret, NULL, noLabel);
				if (err.err)
					return err;
				err = inter_if(exp->arg[1], ret, NULL, noLabel);
				if (err.err)
					return err;
				ret.push_back(new iCode::jump(yesLabel));
				break;
			case stnode::op::ops::LGOR:
				if (exp->argCount != 2) return errInfo(exp->lineN, exp->pos, "Need 2 args");
				err = inter_if(exp->arg[0], ret, yesLabel, NULL);
				if (err.err)
					return err;
				err = inter_if(exp->arg[1], ret, yesLabel, NULL);
				if (err.err)
					return err;
				ret.push_back(new iCode::jump(noLabel));
				break;
			case stnode::op::ops::BIG:
				ifOp = iCode::IFG;
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
						err = inter_gen(exp->arg[i], ret, argRet);
						if (err.err)
							return err;
						args[i] = argRet.val;
						if (argRet.val->getType() == iCode::argType::TEMP)
							static_cast<iCode::temp*>(argRet.val)->ref++;
					}
					ret.push_back(new iCode::jump_if(ifOp, yesLabel, args[0], args[1]));
					ret.push_back(new iCode::jump(noLabel));
				}
				break;
			default:
			{
				retValTp retVal;
				errInfo err = inter_gen(node, ret, retVal);
				if (err.err)
					return err;
				ret.push_back(new iCode::jump_if(iCode::IFN, yesLabel, retVal.val, new iCode::con(0)));
				ret.push_back(new iCode::jump(noLabel));
			}
		}
	}
	else
	{
		retValTp retVal;
		errInfo err = inter_gen(node, ret, retVal);
		if (err.err)
			return err;
		ret.push_back(new iCode::jump_if(iCode::IFN, yesLabel, retVal.val, new iCode::con(0)));
		ret.push_back(new iCode::jump(noLabel));
	}
	return noErr;
}

errInfo inter_gen(stnode::stnode* node, iCodeSeq &ret, retValTp &retVal)
{
	if (node == NULL)
		return errInfo(0, 0, "NULL Pointer");
	switch (node->getType())
	{
		case stnode::type::NUMBER:
		{
			retVal.val = new iCode::con(static_cast<stnode::number *>(node)->val);
			retVal.type = dataType(dataType::ANY, 0);
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
			iCode::id_local *newIDNode = new iCode::id_local(idNode->shift);
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
					errInfo err = inter_gen(opNode->arg[i], ret, argRet);
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
						tmpArg->pCode = code;
						retVal.val = tmpArg;
						break;
					}
					case stnode::op::ops::NEGA:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(iCode::opType::SUB, tmpArg, new iCode::con(0), args[0]);
						ret.push_back(code);
						tmpArg->pCode = code;
						retVal.val = tmpArg;
						break;
					}
					case stnode::op::ops::INC_POST:
					case stnode::op::ops::DEC_POST:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(op, tmpArg, args[0], new iCode::con(1));
						ret.push_back(code);
						tmpArg->pCode = code;
						retVal.val = args[0];
						break;
					}
					case stnode::op::ops::INC_PRE:
					case stnode::op::ops::DEC_PRE:
					{
						iCode::temp *tmpArg = new iCode::temp;
						code = new iCode::code(op, tmpArg, args[0], new iCode::con(1));
						ret.push_back(code);
						tmpArg->pCode = code;
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
							tmpArg->pCode = code;
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
								retVal.val = args[0];
							}
							else
							{
								iCode::temp *tmpArg = new iCode::temp;
								code = new iCode::code(op, tmpArg, args[0], args[1]);
								ret.push_back(code);
								tmpArg->pCode = code;
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
			stTree *block = funcNode->block;
			stTree::iterator p = block->begin(), pEnd = block->end();
			retValTp retVar;
			ret.push_back(new iCode::label(funcNode->funcID));
			for (; p != pEnd; p++)
			{
				errInfo err = inter_gen(*p, ret, retVar);
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
			
			break;
		}
		case stnode::type::CAST:
		{
			stnode::cast *castNode = static_cast<stnode::cast *>(node);

			retValTp argRet;
			errInfo err = inter_gen(castNode->node, ret, argRet);
			if (err.err)
				return err;
			if (argRet.val->getType()==iCode::argType::TEMP)
				static_cast<iCode::temp*>(argRet.val)->ref++;
			
			iCode::temp *tmpArg = new iCode::temp;
			iCode::code *code = new iCode::code(iCode::opType::SET, tmpArg, argRet.val);
			code->argType = argRet.type;
			code->retType = castNode->vtype;
			tmpArg->pCode = code;
			ret.push_back(code);
			
			retVal.val = tmpArg;
			retVal.type = castNode->vtype;
			break;
		}
		case stnode::type::IF:
		{

		}
		case stnode::type::RETURN:
		{

		}
		case stnode::type::TREE:
		{

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
	for (p = sTree.begin(); p != pEnd; p++)
	{
		errInfo err = inter_gen(*p, ret, retVal);
		if (err.err)
			return err;
	}

	return noErr;
}
