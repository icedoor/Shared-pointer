#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "Counter.h"
#include "WeakPtr.h"

template <class T>
class SharedPtr
{
public:
	T* ptr; 
	Counter* cnt;

	SharedPtr();
	SharedPtr(std::nullptr_t np);
	template<class U>SharedPtr(U* p);
	template<class U>SharedPtr(SharedPtr<U>& sp);
	template<class U>SharedPtr(SharedPtr<U>&& sp);
	template<class U>SharedPtr(WeakPtr<U> wp);

	~SharedPtr();

	SharedPtr<T>& operator=(const SharedPtr<T>& rhs);
	SharedPtr<T>& operator=(SharedPtr<T>&& rhs);

	void reset(T* = nullptr);
	T* get() const;
	bool unique() const;

	T& operator*() const;
	T* operator->() const;
	explicit operator bool() const;

	//==
	bool operator==(const SharedPtr<T>& rhs) const;
	bool operator==(std::nullptr_t rhs) const;
	//<
	bool operator<(const SharedPtr<T>& rhs) const;
	bool operator<(std::nullptr_t rhs) const;
};

template<class T>
inline SharedPtr<T>::SharedPtr()
	:ptr(nullptr), cnt(nullptr)
{
}

template<class T>
inline SharedPtr<T>::SharedPtr(std::nullptr_t np)
	: ptr(nullptr), cnt(nullptr)
{
}

template<class T>
template<class U>
inline SharedPtr<T>::SharedPtr(U * p)
	: ptr(p), cnt(new Counter())
{
	cnt->IncreaseShared(); 
}

template<class T>
template<class U>
inline SharedPtr<T>::SharedPtr(SharedPtr<U>& sp)
	: ptr(sp.ptr), cnt(sp.cnt)
{
	if (cnt)
			cnt->IncreaseShared();
}

template<class T>
template<class U>
inline SharedPtr<T>::SharedPtr(SharedPtr<U>&& sp)
	: ptr(sp.ptr), cnt(sp.cnt)
{
	sp.ptr = nullptr;
	sp.cnt = nullptr;
}

template<class T>
template<class U>
inline SharedPtr<T>::SharedPtr(WeakPtr<U> wp)
	: ptr(wp.ptr), cnt(wp.cnt)
{
	if (cnt)
		cnt->IncreaseShared();
}

template<class T>
inline SharedPtr<T>::~SharedPtr()
{
	reset();
}

template<class T>
inline SharedPtr<T> & SharedPtr<T>::operator=(const SharedPtr<T> & rhs)
{
	if (ptr != rhs.ptr)
	{
		reset();
		ptr = rhs.ptr;
		cnt = rhs.cnt;
		cnt->IncreaseShared();
	}
	return *this;
}

template<class T>
inline SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<T> && rhs)
{	
	reset();
	ptr = rhs.ptr;
	cnt = rhs.cnt;

	rhs.ptr = nullptr;
	rhs.cnt = nullptr;

	return *this;
}

template<class T>
inline void SharedPtr<T>::reset(T * p)
{
	if(ptr)
	{ 
		if (cnt->DecreaseShared() == 0) 
			delete ptr;
		ptr = nullptr;
		if (cnt->weak == 0 && cnt->shared == 0)
			delete cnt;
		
			cnt = nullptr;

		if (p != nullptr) {
			ptr = p;
			cnt = new Counter(1,0);
		}
	}
}

template<class T>
inline T* SharedPtr<T>::get() const
{
	return ptr;
}

template<class T>
inline bool SharedPtr<T>::unique() const
{
	if(cnt)
		return (cnt->shared == 1); 
	return false;
}

template<class T>
inline T& SharedPtr<T>::operator*() const
{
	return *ptr;
}

template<class T>
inline T* SharedPtr<T>::operator->() const
{
	return ptr;
}

template<class T>
inline SharedPtr<T>::operator bool() const
{
	return (ptr != nullptr);
}

//==
template<class T>
inline bool SharedPtr<T>::operator==(const SharedPtr<T> & rhs) const
{
	return (ptr == rhs.ptr);
}

template<class T>
inline bool SharedPtr<T>::operator==(std::nullptr_t rhs) const
{
	return (ptr == nullptr);
}

//<
template<class T>
inline bool SharedPtr<T>::operator<(const SharedPtr<T> & rhs) const
{
	return (ptr < rhs.get());
}
template<class T>
inline bool SharedPtr<T>::operator<(std::nullptr_t rhs) const
{
	return (ptr < nullptr);
}