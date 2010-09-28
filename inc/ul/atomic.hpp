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

///////////////////////////////////////////////////////////////////////////
template<class T>
class atomic {
	struct safe_bool_t {
		void true_() {}
	};
	typedef void (safe_bool_t::*safe_bool)();

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

	operator safe_bool()                  { return _value ? &safe_bool_t::true_ : 0; }
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
