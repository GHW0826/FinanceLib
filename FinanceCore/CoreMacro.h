#pragma once
#include "Error.h"
#include <source_location>
#include <iostream>
#include <sstream>

#define SF_ASSERTION_BEGIN do {
#define SF_ASSERTION_ASSERTION_END } while(false)


#define ASSERT_CRASH(expr)					\
SF_ASSERTION_BEGIN							\
	if (!(expr))							\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
SF_ASSERTION_ASSERTION_END


#define SF_ASSERT(condition, message) \
SF_ASSERTION_BEGIN \
if (!(condition)) { \
    std::ostringstream msg_stream; \
    msg_stream << message; \
    throw Error(__FILE__,__LINE__, std::source_location::current().function_name(), msg_stream.str()); \
} \
SF_ASSERTION_ASSERTION_END

#define SF_FAIL(message) \
SF_ASSERTION_BEGIN \
    std::ostringstream msg_stream; \
    msg_stream << message; \
    throw Error(__FILE__,__LINE__, std::source_location::current().function_name(), msg_stream.str()); \
SF_ASSERTION_ASSERTION_END