#pragma once

#ifndef _H_TYPE
#define _H_TYPE

namespace dataType
{
	enum type{ 
		ERROR, 
		VOID, SINT, S8, S16, S32, S64, UINT, U8, U16, U32, U64, 
		VOID_PTR, SINT_PTR, S8_PTR, S16_PTR, S32_PTR, S64_PTR, UINT_PTR, U8_PTR, U16_PTR, U32_PTR, U64_PTR, 
		VOID_PTR_2, SINT_PTR_2, S8_PTR_2, S16_PTR_2, S32_PTR_2, S64_PTR_2, UINT_PTR_2, U8_PTR_2, U16_PTR_2, U32_PTR_2, U64_PTR_2 
	};
}

dataType::type toPtr(dataType::type type);
int typeSize(dataType::type type);
bool isUnsigned(dataType::type type);
dataType::type higher(dataType::type a, dataType::type b);

#endif
