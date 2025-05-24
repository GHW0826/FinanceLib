#pragma once
#include <memory>

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;


template<typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}


#ifndef SF_INTEGER
	#define SF_INTEGER int16
#endif
#ifndef SF_REAL
	#define SF_REAL double
#endif


using Natural = SF_INTEGER;
using Integer = SF_INTEGER;
using Real = SF_REAL;
using Size = std::size_t;
