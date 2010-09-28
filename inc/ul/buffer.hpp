//=============================================================================
// Brief : Buffer
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2008-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_BUFFER__HPP_
#define UL_BUFFER__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/utility.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/throw_exception.hpp>
#include <cstdlib>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T>
class buffer : boost::noncopyable {
	UL_STATIC_ASSERT(boost::is_integral<T>::value, "T must be an integral type");

public:
	buffer()           : _ptr(nullptr) { }
	buffer(size_t len) : _ptr(nullptr) { size(len); }
	~buffer()                          { std::free(_ptr); }

	void size(size_t len)
	{
		if (len != _len) {
			void* p = std::realloc(_ptr, len * sizeof(T));

			if (!p && len)
				throw std::bad_alloc();

			_ptr = p;
			_len = len;
		}
	}

	T*       get()        { return reinterpret_cast<T*>(_ptr); }
	const T* get() const  { return reinterpret_cast<const T*>(_ptr); }
	size_t   size() const { return _len; }

private:
	void*  _ptr;
	size_t _len;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_BUFFER__HPP_ */
