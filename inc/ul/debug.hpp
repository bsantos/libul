//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2009-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_DEBUG__HPP_
#define UL_DEBUG__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <string>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
struct symbol_info {
	char name[1024];
	char module[1024];
	uint offset;
};

struct bug_info {
	const char* expression;
	const char* function;
	const char* file;
	uint        line;
};

uint        backtrace(uintptr trace[], uint length);
bool        get_symbol_info(uintptr address, symbol_info& info);
std::string get_symbol_string(symbol_info const& info);
std::string get_symbol_string(uintptr address);

void setup_crash_handler();
void panic(bug_info const& info);
void panic(bug_info const& info, void* address, void* context);

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#define UL_BUG_ON(exp, reason)                                     \
	do {                                                           \
		static const ::ul::bug_info UL_CONCAT(bug_, __LINE__) = {  \
			#exp,                                                  \
			__FUNCTION__,                                          \
			__FILE__,                                              \
			__LINE__                                               \
		};                                                         \
		if (exp)                                                   \
			::ul::panic(UL_CONCAT(bug_, __LINE__));                \
    } while(0)

#ifndef NDEBUG
#	define UL_ASSERT(exp) UL_BUG_ON(!(exp), #exp)
#else
#	define UL_ASSERT(exp) ((void) 0)
#endif

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_DEBUG__HPP_ */
