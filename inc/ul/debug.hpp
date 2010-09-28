//=============================================================================
// Brief : Debug Helpers
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2009-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_DEBUG__HPP_
#define UL_DEBUG__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
enum bug {
	bug_on,
	bug_assert,
	bug_assert_ptr,
	bug_not_implemented,
	bug_never_here,
	bug_floting_point_exception,
	bug_invalid_opcode,
	bug_terminal_interrupt,
	bug_page_fault,
	bug_breakpoint,
};

extern char const* k_bug_code_string[];

struct crash_ctx {
	bug         code;
	void*       address;
	const char* function;
	const char* file;
	uint        line;
	const char* expression;
	void*       context;
};

class checkpoint {
public:
	static checkpoint* top();

	checkpoint(const char* file, uint line, const char* exp);
	~checkpoint();

	checkpoint* previous() const   { return _prev; }
	const char* file() const       { return _file; }
	uint        line() const       { return _line; }
	const char* expression() const { return _exp; }

	operator bool() const { return false; }

private:
	checkpoint* _prev;
	const char* _file;
	uint        _line;
	const char* _exp;
};

void setup_crash_handler();

void crash(bug code, const char* function, const char* file, uint line, const char* expression);
void crash(const crash_ctx& ctx);

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#define UL_SCOPED_CHECKPOINT(doing) UL_SCOPED(ul::checkpoint cp = ul::checkpoint(__FILE__, __LINE__, doing))
#define UL_CHECKPOINT(doing)        ul::checkpoint BOOST_JOIN(cp_, __LINE__)(__FILE__, __LINE__, doing)
#define UL_BUG_ON(exp, reason)      do { if(UL_UNLIKELY(exp)) { ul::crash(ul::bug_on, __FUNCTION__, __FILE__, __LINE__, reason); } } while(0)
#define UL_NOT_IMPLEMENTED          do { ul::crash(ul::bug_not_implemented, __FUNCTION__, __FILE__, __LINE__, reason); } while(0)

// DEBUG BUILDS ///////////////////////////////////////////////////////////////
#ifndef NDEBUG
#	define UL_ASSERT(exp)              do { if(UL_UNLIKELY(!(exp))) { ul::crash(ul::bug_assert, __FUNCTION__, __FILE__, __LINE__, #exp); } } while(0)
#	define UL_ASSERT_PTR(exp)          do { if(UL_UNLIKELY((exp) == nullptr)) { ul::crash(ul::bug_assert_ptr, __FUNCTION__, __FILE__, __LINE__, #exp); } } while(0)
#	define UL_DBG_SCOPED_CHECKPOINT(x) UL_SCOPED_CHECKPOINT(x)
#	define UL_DBG_CHECKPOINT(x)        UL_CHECKPOINT(x)
#	define UL_NEVER_HERE               do { ul::crash(ul::bug_never_here, __FUNCTION__, __FILE__, __LINE__, nullptr); } while(0)
#else
#	define UL_ASSERT(x)                ((void) 0)
#	define UL_ASSERT_PTR(x)            ((void) 0)
#	define UL_DBG_SCOPED_CHECKPOINT(x) ((void) 0)
#	define UL_DBG_CHECKPOINT           ((void) 0)
#	ifdef BOOST_MSVC
#		define UL_NEVER_HERE           __assume(0)
#	else
#		define UL_NEVER_HERE           ((void) 0)
#	endif
#endif

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_DEBUG__HPP_ */
