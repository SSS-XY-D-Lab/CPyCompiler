#pragma once

#ifndef _H_ST_INTER
#define _H_ST_INTER

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
extern idTableTp idTable;

struct funcItem
{
	funcItem(dataType _retType, std::vector<dataType> &_argType){ retType = _retType; argType = _argType; };
	dataType retType;
	std::vector<dataType> argType;
};
typedef std::vector<funcItem> funcTableTp;
extern funcTableTp funcTable;

typedef std::unordered_map<int, int> constTableTp;
//extern constTableTp constTable;

typedef std::unordered_map<std::string, int> idHashLayerTp;
typedef std::list<idHashLayerTp*> idHashTableTp;
extern idHashTableTp idHashTable;

char* str2cstr(std::string arg);
std::string num2str(long long n);
char* getDbgLineNStr(const char* msg, int line);

int getID(std::string name);
int newID(std::string name, dataType type, bool isConst = false);
int newFuncID(std::string name, dataType retType, std::vector<dataType> &argType);

errInfo stAnalyzer_build(stnode::stnode **node);
errInfo stAnalyzer_type(stnode::stnode **node, dataType retType);

const int ERR_NEWID_NOLAYER = -1;
const int  ERR_NEWID_REDEFINE = -2;
extern const char* ERR_NEWID_MSG[3];

#endif
