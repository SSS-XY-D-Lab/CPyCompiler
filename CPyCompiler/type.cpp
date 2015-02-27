#include "stdafx.h"
#include "type.h"

const int intSize = 2;

dataType::type toPtr(dataType::type type)
{
	switch (type)
	{
		case dataType::VOID:
			return dataType::VOID_PTR;
		case dataType::SINT:
			return dataType::SINT_PTR;
		case dataType::S8:
			return dataType::S8_PTR;
		case dataType::S16:
			return dataType::S16_PTR;
		case dataType::S32:
			return dataType::S32_PTR;
		case dataType::S64:
			return dataType::S64_PTR;
		case dataType::UINT:
			return dataType::UINT_PTR;
		case dataType::U8:
			return dataType::U8_PTR;
		case dataType::U16:
			return dataType::U16_PTR;
		case dataType::U32:
			return dataType::U32_PTR;
		case dataType::U64:
			return dataType::U64_PTR;
		case dataType::VOID_PTR:
			return dataType::VOID_PTR_2;
		case dataType::SINT_PTR:
			return dataType::SINT_PTR_2;
		case dataType::S8_PTR:
			return dataType::S8_PTR_2;
		case dataType::S16_PTR:
			return dataType::S16_PTR_2;
		case dataType::S32_PTR:
			return dataType::S32_PTR_2;
		case dataType::S64_PTR:
			return dataType::S64_PTR_2;
		case dataType::UINT_PTR:
			return dataType::UINT_PTR_2;
		case dataType::U8_PTR:
			return dataType::U8_PTR_2;
		case dataType::U16_PTR:
			return dataType::U16_PTR_2;
		case dataType::U32_PTR:
			return dataType::U32_PTR_2;
		case dataType::U64_PTR:
			return dataType::U64_PTR_2;
	}
	return dataType::ERROR;
}

int typeSize(dataType::type type)
{
	switch (type)
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
		case dataType::VOID_PTR:
		case dataType::SINT_PTR:
		case dataType::S8_PTR:
		case dataType::S16_PTR:
		case dataType::S32_PTR:
		case dataType::S64_PTR:
		case dataType::UINT_PTR:
		case dataType::U8_PTR:
		case dataType::U16_PTR:
		case dataType::U32_PTR:
		case dataType::U64_PTR:
		case dataType::VOID_PTR_2:
		case dataType::SINT_PTR_2:
		case dataType::S8_PTR_2:
		case dataType::S16_PTR_2:
		case dataType::S32_PTR_2:
		case dataType::S64_PTR_2:
		case dataType::UINT_PTR_2:
		case dataType::U8_PTR_2:
		case dataType::U16_PTR_2:
		case dataType::U32_PTR_2:
		case dataType::U64_PTR_2:
			return intSize;
	}
	return -1;
}

bool isUnsigned(dataType::type type)
{
	switch (type)
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
		case dataType::VOID_PTR:
		case dataType::SINT_PTR:
		case dataType::S8_PTR:
		case dataType::S16_PTR:
		case dataType::S32_PTR:
		case dataType::S64_PTR:
		case dataType::UINT_PTR:
		case dataType::U8_PTR:
		case dataType::U16_PTR:
		case dataType::U32_PTR:
		case dataType::U64_PTR:
		case dataType::VOID_PTR_2:
		case dataType::SINT_PTR_2:
		case dataType::S8_PTR_2:
		case dataType::S16_PTR_2:
		case dataType::S32_PTR_2:
		case dataType::S64_PTR_2:
		case dataType::UINT_PTR_2:
		case dataType::U8_PTR_2:
		case dataType::U16_PTR_2:
		case dataType::U32_PTR_2:
		case dataType::U64_PTR_2:
			return true;
	}
	return false;
}

dataType::type higher(dataType::type a, dataType::type b)
{
	if (typeSize(a) == typeSize(b))
	{
		if (isUnsigned(a))
			return a;
		else
			return b;
	}
	else
		return typeSize(a) > typeSize(b) ? a : b;
}
