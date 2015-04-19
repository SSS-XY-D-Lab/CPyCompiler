#pragma once

#ifndef _H_TYPE
#define _H_TYPE

struct dataType
{
	enum type{
		ERROR,
		VOID, SINT, S8, S16, S32, S64, UINT, U8, U16, U32, U64,
	};
	dataType(){ dType = dataType::ERROR; ptrLvl = 0; isConst = false; };
	dataType(type _dType, int _ptrLvl, bool _isConst = false){ dType = _dType; ptrLvl = _ptrLvl; if (ptrLvl == 0){ isConst = _isConst; } else{ isConst = false; } };
	type dType;
	int ptrLvl;
	bool isConst;
};

std::string type2Str(dataType type);
dataType toRef(dataType type);
dataType deref(dataType type);
int typeSize(dataType type);
int typeLvl(dataType type);
bool isUnsigned(dataType type);
dataType::type minNum(long long num);
const int intSize = 2;

#endif
