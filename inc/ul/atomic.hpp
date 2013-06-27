//=============================================================================
// Brief : Atomic Primitive
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2005-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_ATOMIC__HPP_
#define UL_ATOMIC__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/detail/atomic.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
namespace detail {

///////////////////////////////////////////////////////////////////////////
#ifdef __GNUC__
inline void atomic_barrier()
{
	__sync_synchronize();
}

template<class T>
inline T atomic_inc_and_fetch(T volatile* dst)
{
	return __sync_add_and_fetch(dst, 1);
}

template<class T>
inline T atomic_dec_and_fetch(T volatile* dst)
{
	return __sync_sub_and_fetch(dst, 1);
}

template<class T>
inline T atomic_add_and_fetch(T volatile* dst, T val)
{
	return __sync_add_and_fetch(dst, val);
}

template<class T>
inline T atomic_sub_and_fetch(T volatile* dst, T val)
{
	return __sync_sub_and_fetch(dst, val);
}

template<class T>
inline T atomic_or_and_fetch(T volatile* dst, T val)
{
	return __sync_or_and_fetch(dst, val);
}

template<class T>
inline T atomic_and_and_fetch(T volatile* dst, T val)
{
	return __sync_and_and_fetch(dst, val);
}

template<class T>
inline T atomic_xor_and_fetch(T volatile* dst, T val)
{
	return __sync_xor_and_fetch(dst, val);
}

template<class T>
inline T atomic_fetch_and_inc(T volatile* dst)
{
	return __sync_fetch_and_add(dst, 1);
}

template<class T>
inline T atomic_fetch_and_dec(T volatile* dst)
{
	return __sync_fetch_and_sub(dst, 1);
}

template<class T>
inline T atomic_fetch_and_add(T volatile* dst, T val)
{
	return __sync_fetch_and_add(dst, val);
}

template<class T>
inline T atomic_fetch_and_sub(T volatile* dst, T val)
{
	return __sync_fetch_and_sub(dst, val);
}

template<class T>
inline T atomic_fetch_and_or(T volatile* dst, T val)
{
	return __sync_fetch_and_or(dst, val);
}

template<class T>
inline T atomic_fetch_and_and(T volatile* dst, T val)
{
	return __sync_fetch_and_and(dst, val);
}

template<class T>
inline T atomic_fetch_and_xor(T volatile* dst, T val)
{
	return __sync_fetch_and_xor(dst, val);
}

template<class T>
inline T atomic_swap(T volatile* dst, T val)
{
	return __sync_lock_test_and_set(dst, val);
}

template<class T>
inline T atomic_compare_and_swap(T volatile* dst, T compare, T val)
{
	return __sync_val_compare_and_swap(dst, compare, val);
}

#else
#	error unsupported compiler
#endif

///////////////////////////////////////////////////////////////////////////////
} /* namespace detail */

///////////////////////////////////////////////////////////////////////////
template<class T>
class atomic {
	UL_UNDEFINED_BOOL;

public:
	T fetch_and_add(T val) { return detail::atomic_fetch_and_add(&_value, val); }
	T fetch_and_sub(T val) { return detail::atomic_fetch_and_sub(&_value, val); }
	T fetch_and_or(T val)  { return detail::atomic_fetch_and_or(&_value, val); }
	T fetch_and_and(T val) { return detail::atomic_fetch_and_and(&_value, val); }
	T fetch_and_xor(T val) { return detail::atomic_fetch_and_xor(&_value, val); }

	T swap(T val)                        { return detail::atomic_swap(&_value, val); }
	T compare_and_swap(T compare, T val) { return detail::atomic_compare_and_swap(&_value, compare, val); }

	atomic& operator=(T val) { detail::atomic_swap(&_value, val); return *this; }

	T operator+=(T val) { return detail::atomic_add_and_fetch(&_value, val); }
	T operator-=(T val) { return detail::atomic_sub_and_fetch(&_value, val); }

	T operator|=(T val) { return detail::atomic_or_and_fetch(&_value, val); }
	T operator&=(T val) { return detail::atomic_and_and_fetch(&_value, val); }
	T operator^=(T val) { return detail::atomic_xor_and_fetch(&_value, val); }

	T operator++()    { return detail::atomic_inc_and_fetch(&_value); }
	T operator--()    { return detail::atomic_dec_and_fetch(&_value); }
	T operator++(int) { return detail::atomic_fetch_and_inc(&_value); }
	T operator--(int) { return detail::atomic_fetch_and_dec(&_value); }

	operator undefined_bool()
	{
		return _value ? UL_UNDEFINED_BOOL_TRUE
		              : UL_UNDEFINED_BOOL_FALSE;
	}

	bool operator!() const                { return !_value; }
	bool operator!=(volatile T val) const { return (_value != val); }
	bool operator==(volatile T val) const { return (_value == val); }
	bool operator<=(volatile T val) const { return (_value <= val); }
	bool operator>=(volatile T val) const { return (_value >= val); }
	bool operator<(volatile T val) const  { return (_value < val); }
	bool operator>(volatile T val) const  { return (_value > val); }

private:
	T volatile _value;
};

///////////////////////////////////////////////////////////////////////////
template<class T>
class atomic<T*> {
	struct safe_bool_t {
		void true_() {}
	};
	typedef void (safe_bool_t::*safe_bool)();

public:
	T* swap(T* val)                         { return detail::atomic_swap(&_ptr, val); }
	T* compare_and_swap(T* compare, T* val) { return detail::atomic_compare_and_swap(&_ptr, compare, val); }

	atomic& operator=(T* p) { detail::atomic_swap(&_ptr, p); return *this; }

	T* operator->() const               { return _ptr; }
	T& operator[](size_t n)             { return _ptr[n]; }
	const T& operator[](size_t n) const { return _ptr[n]; }

	T* operator++()    { return detail::atomic_inc_and_fetch(&_ptr); }
	T* operator--()    { return detail::atomic_dec_and_fetch(&_ptr); }
	T* operator++(int) { return detail::atomic_fetch_and_inc(&_ptr); }
	T* operator--(int) { return detail::atomic_fetch_and_dec(&_ptr); }

	operator safe_bool()                 { return _ptr ? &safe_bool_t::true_ : 0; }
	bool operator!() const               { return !_ptr; }
	bool operator!=(volatile T* p) const { return (_ptr != p); }
	bool operator==(volatile T* p) const { return (_ptr == p); }
	bool operator<=(volatile T* p) const { return (_ptr <= p); }
	bool operator>=(volatile T* p) const { return (_ptr >= p); }
	bool operator<(volatile T* p) const  { return (_ptr < p); }
	bool operator>(volatile T* p) const  { return (_ptr > p); }

private:
	T* volatile _ptr;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_ATOMIC__HPP_ */
