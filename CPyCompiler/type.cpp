#include "stdafx.h"
#include "type.h"
#include <cmath>

const int uintLvl = static_cast<int>(log(intSize) / log(2)) * 4;

std::string type2Str(dataType type)
{
	std::string ret;
	switch (type.dType)
	{
		case dataType::VOID:
			ret = "VOID";
			break;
		case dataType::SINT:
			ret = "SINT";
			break;
		case dataType::S8:
			ret = "S8";
			break;
		case dataType::S16:
			ret = "S16";
			break;
		case dataType::S32:
			ret = "S32";
			break;
		case dataType::S64:
			ret = "S64";
			break;
		case dataType::UINT:
			ret = "UINT";
			break;
		case dataType::U8:
			ret = "U8";
			break;
		case dataType::U16:
			ret = "U16";
			break;
		case dataType::U32:
			ret = "U32";
			break;
		case dataType::U64:
			ret = "U64";
			break;
		default:
			return "";
	}
	for (int i = 0; i < type.ptrLvl; i++)
		ret.push_back('*');
	return ret;
}

dataType toRef(dataType type)
{
	type.ptrLvl++;
	return type;
}

dataType deref(dataType type)
{
	type.ptrLvl--;
	return type;
}

int typeSize(dataType type)
{
	if (type.ptrLvl > 0)
		return intSize;
	else
	{
		switch (type.dType)
		{
			case dataType::VOID:
				return 0;
			case dataType::SINT:
			case dataType::UINT:
				return intSize;
			case dataType::S8:
			case dataType::U8:
				return 1;
			case dataType::S16:
			case dataType::U16:
				return 2;
			case dataType::S32:
			case dataType::U32:
				return 4;
			case dataType::S64:
			case dataType::U64:
				return 8;
		}
	}
	return -1;
}

bool isUnsigned(dataType type)
{
	if (type.ptrLvl > 0)
		return true;
	else
	{
		switch (type.dType)
		{
			case dataType::SINT:
			case dataType::S8:
			case dataType::S16:
			case dataType::S32:
			case dataType::S64:
				return false;
			case dataType::UINT:
			case dataType::U8:
			case dataType::U16:
			case dataType::U32:
			case dataType::U64:
			case dataType::VOID:
				return true;
		}
	}
	return false;
}

int typeLvl(dataType type)
{
	if (type.ptrLvl > 0)
		return true;
	else
	{
		switch (type.dType)
		{
			case dataType::VOID:
				return 0;
			case dataType::S8:
				return 1;
			case dataType::U8:
				return 2;
			case dataType::S16:
				return 3;
			case dataType::U16:
				return 4;
			case dataType::S32:
				return 5;
			case dataType::U32:
				return 6;
			case dataType::S64:
				return 7;
			case dataType::U64:
				return 8;
			case dataType::SINT:
				return uintLvl - 1;
			case dataType::UINT:
				return uintLvl;
		}
	}
	return -1;
}

dataType::type minNum(long long num)
{
	if (num < 0)
	{
		if (num < INT_MIN)
			return dataType::S64;
		if (num < SHRT_MIN)
			return dataType::S32;
		if (num < SCHAR_MIN)
			return dataType::S16;
		return dataType::S8;
	}
	else
	{
		if (num > UINT_MAX)
			return dataType::S64;
		if (num > INT_MAX)
			return dataType::U32;
		if (num > USHRT_MAX)
			return dataType::S32;
		if (num > SHRT_MAX)
			return dataType::U16;
		if (num > UCHAR_MAX)
			return dataType::S16;
		if (num > SCHAR_MAX)
			return dataType::U8;
		return dataType::S8;
	}
}
