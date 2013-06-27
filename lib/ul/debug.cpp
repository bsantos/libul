//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2008-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#include <ul/debug.hpp>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <signal.h>

///////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__)
#	define UL_RETURN_ADDRESS __builtin_return_address(0)
#elif defined(_MSC_VER)
#	include <intrin.h>
#	include <windows.h>
#	include <psapi.h>
#	include <dbghelp.h>
#	define UL_RETURN_ADDRESS _ReturnAddress()
#else
#	define UL_RETURN_ADDRESS 0
#endif

#if defined(__linux) || defined(__APPLE__)
#	define UL_HAS_DLADDR
#	include <dlfcn.h>
#endif

#if defined(__GNUC__) || defined(__APPLE__)
#	define UL_HAS_CXX_ABI
#	include <cxxabi.h>
#	include <unwind.h>
#endif

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
namespace detail {
#if defined(UL_HAS_CXX_ABI)

struct backtrace_ctx {
	uintptr* trace;
	uint     pos;
	uint     count;
};

static _Unwind_Reason_Code backtrace_handler(struct _Unwind_Context* context, void* ctx)
{
	backtrace_ctx* c = static_cast<backtrace_ctx*>(ctx);

	if (c->pos < c->count)
		c->trace[c->pos++] = _Unwind_GetIP(context);

	return _URC_NO_REASON;
}

struct auto_free {
	auto_free(void* p)
		: ptr(p)
	{ }

	~auto_free()
	{
		free(ptr);
	}

	void* ptr;
};

#elif defined(_MSC_VER)

typedef USHORT (WINAPI *backtrace_f)(ULONG, ULONG, PVOID*, PULONG);

static backtrace_f get_backtrace_f()
{
	return reinterpret_cast<backtrace_f>(GetProcAddress(GetModuleHandle("kernel32.dll"),
	                                                    "RtlCaptureStackBackTrace"));
}

static uint get_max_backtrace()
{
	OSVERSIONINFO info = { sizeof(OSVERSIONINFO), 0 };

	GetVersionEx(&info);
	if (info.dwMajorVersion < 6)
		return 62

	return MAXUSHORT;
}

static bool init_syms()
{
	static bool is_syms_init;

	if (!is_syms_init) {
		SymSetOptions(SYMOPT_DEFERRED_LOADS);

		if (SymInitialize(GetCurrentProcess(), NULL, TRUE))
			is_syms_init = true;
	}

	return is_syms_init;
}

static const backtrace_f backtrace = get_backtrace_f();
static const uint k_max_backtrace = get_max_backtrace();

#endif

#ifdef __linux__
void signal_handler(int, siginfo_t* si, void* context)
{
	struct sigcontext* ctx = (struct sigcontext*) &((ucontext_t*) context)->uc_mcontext;
	char expr[20];
	bug_info info = {
		expr,
		0,
		0,
		0
	};

	switch(si->si_signo) {
	case SIGFPE:
		info.expression = "SIGFPE: floating point exception";
		break;

	case SIGILL:
		info.expression = "SIGILL: invalid opcode";
		break;

	case SIGINT:
		info.expression = "SIGINT: terminal interrupt";
		break;

	case SIGSEGV:
		info.expression = "SIGSEGV: read/write page fault";
		break;

	case SIGTRAP:
		info.expression = "SIGTRAP: breakpoint";
		break;

	default:
		std::sprintf(expr, "SIG: %d", si->si_signo);
	}

#ifdef __i386__
	void* address = (void*) ctx->eip;
#elif __amd64__
	void* address = (void*) ctx->rip;
#endif

	panic(info, address, ctx);
}
#endif
} /* namespace detail */

///////////////////////////////////////////////////////////////////////////////
#if defined(UL_HAS_CXX_ABI)

uint backtrace(uintptr trace[], uint length)
{
	detail::backtrace_ctx ctx = { trace, 0, length };

	_Unwind_Backtrace(detail::backtrace_handler, &ctx);
	return ctx.pos;
}

#elif defined(_WIN32)

uint backtrace(uintptr trace[], uint length)
{
	if (!detail::backtrace)
		return 0;

	return detail::backtrace(0, std::min(length, detail::k_max_backtrace), trace, nullptr);
}

#else

uint backtrace(uintptr* /*trace*/, uint /*length*/)
{
	return 0;
}

#endif

///////////////////////////////////////////////////////////////////////////////
#if defined(UL_HAS_DLADDR)

bool get_symbol_info(uintptr address, symbol_info& info)
{
	Dl_info di;

	if (!dladdr((void*) address, &di))
		return false;

#if defined(UL_HAS_CXX_ABI)
	int status;
	size_t len;
	char* sym = abi::__cxa_demangle(di.dli_sname, nullptr, &len, &status);
	detail::auto_free af(sym);

	std::strncpy(info.name, sym ? sym : di.dli_sname, sizeof(info.name));
#else
	std::strncpy(info.name, di.dli_sname, sizeof(info.name));
#endif
	std::strncpy(info.module, di.dli_fname, sizeof(info.module));
	info.offset = address - (uintptr) di.dli_saddr;

	return true;
}

#elif defined(_WIN32)

bool get_symbol_info(uintptr address, symbol_info& info)
{
	if (detail::init_syms()) {
		char buffer[sizeof(SYMBOL_INFO) + sizeof(info.name)];
		SYMBOL_INFO* sym = reinterpret_cast<SYMBOL_INFO*>(buffer);
		DWORD64 disp = 0;

		sym->SizeOfStruct = sizeof(SYMBOL_INFO);
		sym->MaxNameLen = sizeof(info.name);

		if (!SymFromAddr(GetCurrentProcess(), address, &disp, sym))
			return false;

		std::strncpy(info.name, sym->Name, sym->NameLen);
		info.offset = (uint) disp;

		if (!GetModuleFileName((HMODULE) sym->ModBase, info.module, sizeof(info.module)))
			info.module[0] = 0;

		return true;
	}

	return false;
}

#else

bool get_symbol_info(uintptr address, symbol_info& info)
{
	return false;
}

#endif

std::string get_symbol(symbol_info const& info)
{
	std::string sym(info.name, strnlen(info.name, sizeof(info.name)));

	if (info.offset) {
		char buff[sizeof(uint) * 2 + 6];

		std::sprintf(buff, " + 0x%x", info.offset);
		sym += buff;
	}

	return sym;
}

std::string get_symbol(uintptr address)
{
	symbol_info info;

	if (!get_symbol_info(address, info))
		return std::string();

	return get_symbol(info);
}

///////////////////////////////////////////////////////////////////////////////
void setup_crash_handler()
{
#ifdef __linux__
	struct sigaction handler;

	handler.sa_sigaction = detail::signal_handler;
	handler.sa_flags = SA_SIGINFO;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGFPE, &handler, nullptr);
	sigaction(SIGILL, &handler, nullptr);
	sigaction(SIGINT, &handler, nullptr);
	sigaction(SIGSEGV, &handler, nullptr);
	sigaction(SIGTRAP, &handler, nullptr);
#endif
}

void panic(bug_info const& info)
{
	panic(info, UL_RETURN_ADDRESS, nullptr);
}

void panic(bug_info const& info, void* address, void* context)
{
	static const uint k_btrace_size = 100;
	uintptr btrace[k_btrace_size];
	uint btcnt = backtrace(btrace, k_btrace_size);

	std::fprintf(stderr,	 "\n"
							 "================================= Crash Dump ==================================\n"
							 );

	if (info.function)
		std::fprintf(stderr, "func: %s\n", info.function);

	if (info.file) {
		std::fprintf(stderr, "file: %s\n"
							 "line: %u\n",
							 info.file,
							 info.line);
	}

	if (info.expression) {
		std::fprintf(stderr, "expr: %s\n",
							 info.expression);
	}

	std::fprintf(stderr, "\n\n");

	for (uint i = 0; i < btcnt; ++i) {
		symbol_info si;

		if (!get_symbol_info(btrace[i], si))
			fprintf(stderr, "%u. 0x%016llx\n", i, ullong(btrace[i]));
		else
			fprintf(stderr, "%u. %s + 0x%x (%s)\n", i, si.name, si.offset, si.module);
	}

	std::fprintf(stderr,	 "===============================================================================\n"
							 "\n");
	std::abort();
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
