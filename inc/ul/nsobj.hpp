#if !defined(UL_NSOBJ__HPP_) && defined(__APPLE__)
#define UL_NSOBJ__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/exception.hpp>
#include <type_traits>
#include <utility>
#include <memory>
#include <CoreFoundation/CFBase.h>

////////////////////////////////////////////////////////////////////////////////
namespace ul {

////////////////////////////////////////////////////////////////////////////////
static constexpr struct nsobj_retain_opt { } nsobj_retain = { };

struct nsobj_release {
	void operator()(CFTypeRef obj)
	{
		CFRelease(obj);
	}
};

template<class T>
using nsobj_ref = std::unique_ptr<T, nsobj_release>;

template<class T>
class nsobj;

template<>
class nsobj<void> {
public:
	typedef nsobj<void> base;

public:
	constexpr nsobj()
		: _obj(nullptr)
	{ }

	~nsobj()
	{
		if (_obj)
			CFRelease(_obj);
	}

	nsobj(nsobj&& obj) noexcept
		: _obj(obj.release())
	{ }

	nsobj& operator=(nsobj&& obj) noexcept
	{
		if (&obj != this)
			reset(obj.release());
		return *this;
	}

	nsobj(nsobj const& obj)
		: _obj(CFRetain(obj._obj))
	{ }

	nsobj& operator=(nsobj const& obj)
	{
		if (&obj != this)
			reset(CFRetain(obj._obj));
		return *this;
	}

	nsobj& operator=(std::nullptr_t)
	{
		reset();
		return *this;
	}

	CFTypeRef get() const
	{
		return _obj;
	}

	void reset()
	{
		reset(nullptr);
	}

	explicit operator bool() const { return _obj; }

protected:
	explicit nsobj(CFTypeRef obj)
		: _obj(obj)
	{ }

	nsobj(CFTypeRef obj, nsobj_retain_opt)
		: _obj(CFRetain(obj))
	{ }

	void reset(CFTypeRef obj)
	{
		if (_obj)
			CFRelease(_obj);
		_obj = obj;
	}

	CFTypeRef release()
	{
		CFTypeRef tmp = _obj;

		_obj = nullptr;
		return tmp;
	}

private:
	CFTypeRef _obj;
};

#ifdef __OBJC__

template<class T>
class nsobj : public nsobj<void> {
	static_assert(std::is_convertible<T*, id>::value, "T must be an Objective-C type");

public:
	nsobj() = default;
	nsobj(nsobj&&) = default;
	nsobj& operator=(nsobj&&) = default;
	nsobj(nsobj const&) = default;
	nsobj& operator=(nsobj const&) = default;

	explicit nsobj(T* obj)
		: base(obj)
	{ }

	nsobj(T* obj, nsobj_retain_opt)
		: base(obj, nsobj_retain)
	{ }

	T* operator-() const
	{
		return static_cast<T*>(base::get());
	}

	T* get() const
	{
		return static_cast<T*>(base::get());
	}

	void reset(T* obj)
	{
		base::reset(obj);
	}

	T* release()
	{
		return static_cast<T*>(base::release());
	}
};

template<class T>
inline bool operator==(nsobj<T> const& lhs, id rhs)
{
	return lhs.get() == rhs;
}

template<class T>
inline bool operator==(id lhs, nsobj<T> const& rhs)
{
	return lhs == rhs.get();
}

template<class T>
inline bool operator!=(nsobj<T> const& lhs, id rhs)
{
	return lhs.get() != rhs;
}

template<class T>
inline bool operator!=(id lhs, nsobj<T> const& rhs)
{
	return lhs != rhs.get();
}

template<class T>
inline nsobj<T> make_nsobj()
{
	return nsobj<T>([[T alloc] init]);
}

//template<class T, class... Args>
//inline nsobj<T> make_nsobj(SEL init, Args&&... args)
//{
//	return nsobj<T>((T*) objc_msgSend([T alloc], init, std::forward<Args>(args)...));
//}

#endif /* __OBJC__ */

template<class T, class U>
inline bool operator==(nsobj<T> const& lhs, nsobj<U> const& rhs)
{
	return lhs.get() == rhs.get();
}

template<class T, class U>
inline bool operator!=(nsobj<T> const& lhs, nsobj<U> const& rhs)
{
	return lhs.get() != rhs.get();
}

template<class T>
inline nsobj<T> retain_nsobj(T* obj)
{
	return nsobj<T>(obj, nsobj_retain);
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_NSOBJ__HPP_ && __APPLE__ */
