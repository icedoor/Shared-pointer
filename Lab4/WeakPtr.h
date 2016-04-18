#pragma once
#include "Counter.h"
template <class T>
class SharedPtr;

template <class T>
class WeakPtr {
private:		
	void reset();
public:
	T* ptr;
	Counter *cnt;

	WeakPtr();
	WeakPtr(WeakPtr<T>& wp);
	WeakPtr(SharedPtr<T>& sp);
	~WeakPtr();

	WeakPtr<T>& operator=(const WeakPtr<T>& rhs);
	WeakPtr<T>& operator=(const SharedPtr<T>& rhs);

	SharedPtr<T> lock();
	bool expired();
};

template<class T>
inline WeakPtr<T>::WeakPtr()
	:ptr(nullptr), cnt(nullptr)
{
}

template<class T>
inline WeakPtr<T>::WeakPtr(WeakPtr<T>& wp)
	: ptr(wp.ptr), cnt(wp.cnt)
{
	if(cnt)
		cnt->IncreaseWeak();
}

template<class T>
inline WeakPtr<T>::WeakPtr(SharedPtr<T>& sp)
	:ptr(sp.ptr), cnt(sp.cnt)
{
	if (cnt)
		cnt->IncreaseWeak();
}

template<class T>
inline WeakPtr<T>::~WeakPtr()
{
	reset();
}

template<class T>
inline void WeakPtr<T>::reset()
{
	if (!expired())
	{
		ptr = nullptr;
		if (cnt->DecreaseWeak() == 0 && cnt->shared == 0)
			delete cnt;
		cnt = nullptr;
	}
}

template<class T>
inline WeakPtr<T> & WeakPtr<T>::operator=(const WeakPtr<T> & rhs)
{
	if (this != &rhs)
	{
		reset();
		ptr = rhs.ptr;
		cnt = rhs.cnt;
		if(cnt)
			cnt->IncreaseWeak();
	}
	return *this;
}

template<class T>
inline WeakPtr<T> & WeakPtr<T>::operator=(const SharedPtr<T> & rhs)
{
	reset();
	ptr = rhs.ptr;
	cnt = rhs.cnt;
	if(cnt)
		cnt->IncreaseWeak();

	return *this;
}

template<class T>
inline SharedPtr<T> WeakPtr<T>::lock()
{
	return (expired() ? (SharedPtr<T>()) : (SharedPtr<T>(*this)));
}

template<class T>
inline bool WeakPtr<T>::expired()
{	
	if (cnt)
	{
		if (cnt->shared > 0)
			return false;
		if (cnt->DecreaseWeak() == 0) {
			delete cnt;			
		}
		cnt = nullptr;
	}
	return true;
}