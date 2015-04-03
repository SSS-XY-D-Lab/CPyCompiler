#pragma once

#ifndef _H_ST_INTER
#define _H_ST_INTER

#include "inter.h"

struct idItem
{
	idItem(dataType _type, bool _isConst){ type = _type; isFunc = false; funcID = 0; isConst = _isConst; };
	idItem(dataType _type, bool _isConst, size_t _shift){ type = _type; isFunc = false; funcID = 0; isConst = _isConst; shift = _shift; };
	idItem(size_t _funcID){ type = dataType(dataType::VOID, 1); isFunc = true; funcID = _funcID; isConst = true; };
	dataType type;
	bool isConst, isFunc;
	size_t funcID, shift;
};
typedef std::vector<idItem> idItemTableTp;
typedef std::unordered_map<std::string, size_t> idHashTableTp;
struct idLayerTp
{
	idItemTableTp table;
	idHashTableTp hash;
	size_t shift = 0;
};
extern idLayerTp idGlobal;
typedef std::list<idLayerTp*> idTableTp;

struct funcItem
{
	funcItem(dataType _retType, std::vector<dataType> &_argType){ retType = _retType; argType = _argType; };
	dataType retType;
	std::vector<dataType> argType;
	size_t labelNo;
};
typedef std::vector<funcItem> funcTableTp;
extern funcTableTp funcTable;

typedef std::vector<std::string> strTableTp;
extern strTableTp strTable;

typedef std::unordered_map<int, int> constTableTp;
//extern constTableTp constTable;

char* str2cstr(std::string arg);
std::string num2str(long long n);
char* getDbgLineNStr(const char* msg, const char* file, int line);

errInfo stAnalyzer_build(stnode::stnode **node, idTableTp &idHashTable);
errInfo stAnalyzer_type(stnode::stnode **node, dataType retType);

#endif
