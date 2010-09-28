//=============================================================================
// Brief     : Atomic Primitive Details
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2005-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_DETAIL_ATOMIC__HPP_
#define UL_DETAIL_ATOMIC__HPP_

///////////////////////////////////////////////////////////////////////////////
namespace ul { namespace detail {

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
} /* namespace detail */ } /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_DETAIL_ATOMIC__HPP_ */
