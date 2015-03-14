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

errInfo inter_gen(stnode::stnode* node, iCodeSeq &ret, retValTp &retVal)
{
	if (node == NULL)
		return errInfo(0, 0, "NULL Pointer");
	switch (node->getType())
	{
		case stnode::type::NUMBER:
		{
			retVal.val = new iCode::con(static_cast<stnode::number *>(node)->val);
			break;
		}
		case stnode::type::CHARA:
		{
			retVal.val = new iCode::con(static_cast<stnode::chara *>(node)->ch);
			break;
		}
		case stnode::type::STR:
		{
			size_t id = newStr(static_cast<stnode::str *>(node)->strr);
			retVal.val = new iCode::str(id);
			break;
		}
		/*
		case stnode::type::ID_INTER:
		{
			retVal.val = new iCode::id(static_cast<stnode::id_inter *>(node)->id);
			break;
		}
		*/
		case stnode::type::OP:
		{
			stnode::op::op *opNode = static_cast<stnode::op::op *>(node);
			int needArg = 3;
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
				case stnode::op::ops::LGNOT:
					needArg = 1;
					op = iCode::opType::LGAND;
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
				case stnode::op::ops::LGAND:
					needArg = 2;
					op = iCode::opType::LGAND;
					break;
				case stnode::op::ops::LGOR:
					needArg = 2;
					op = iCode::opType::LGOR;
					break;
				case stnode::op::ops::ASSIGN:
					needArg = 2;
					op = iCode::opType::NUL;
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
					op = iCode::opType::NUL;
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

			switch (opNode->opVal)
			{
				case stnode::op::ops::POSI:
				case stnode::op::ops::NEGA:
					
				case stnode::op::ops::INC_POST:
				case stnode::op::ops::DEC_POST:
				case stnode::op::ops::INC_PRE:
				case stnode::op::ops::DEC_PRE:
					break;
			}
		}
		case stnode::type::FUNC_INTER:
		{
			stnode::func_inter *funcNode = static_cast<stnode::func_inter *>(node);
			stTree *block = funcNode->block;
			stTree::iterator p = block->begin(), pEnd = block->end();
			retValTp retVal;
			ret.push_back(new iCode::label(funcNode->funcID));
			for (; p != pEnd; p++)
			{
				errInfo err = inter_gen(*p, ret, retVal);
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

	return noErr;
}
