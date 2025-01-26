#pragma once
#include "../src/MyPointer.h"
#include <iostream>
namespace MyPointer {
	template<typename T>
	inline Auto_ptr<T>::Auto_ptr(T* ptr)
		:m_ptr{ ptr }
	{
	}

	template<typename T>
	inline Auto_ptr<T>::Auto_ptr(Auto_ptr& other)
	{
		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
	}

	template<typename T>
	inline Auto_ptr<T>::~Auto_ptr()
	{
		delete m_ptr;
	}

	template<typename T>
	inline T& Auto_ptr<T>::operator*() const
	{
		return *m_ptr;
	}

	template<typename T>
	inline T* Auto_ptr<T>::operator->() const
	{
		return m_ptr;
	}

	template<typename T>
	inline Auto_ptr<T>& Auto_ptr<T>::operator=(Auto_ptr<T>& other)
	{
		if (this == &other) {
			return *this;
		}
		delete m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		return *this;
	}

	template<typename T>
	inline bool Auto_ptr<T>::isNull() const
	{
		return (m_ptr == nullptr);
	}


	template<typename T>
	inline Unique_ptr<T>::Unique_ptr(T* ptr) :
		m_ptr{ ptr }
	{
	}
	template<typename T>
	inline Unique_ptr<T>::Unique_ptr(Unique_ptr&& other) noexcept
		:m_ptr{ other.m_ptr }
	{
		other.m_ptr = nullptr;
	}
	template<typename T>
	inline Unique_ptr<T>& Unique_ptr<T>::operator=(Unique_ptr<T>&& other) noexcept
	{
		if (this == &other) {
			return *this;
		}
		delete m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		return *this;
	}
	template<typename T>
	inline T& Unique_ptr<T>::operator*() const
	{
		return *m_ptr;
	}
	template<typename T>
	inline T* Unique_ptr<T>::operator->() const
	{
		return m_ptr;
	}
	template<typename T>
	inline void Unique_ptr<T>::reset(T* other)
	{
		if (isNull()) return;
		delete m_ptr;
		m_ptr = other;
	}
	template<typename T>
	inline bool Unique_ptr<T>::isNull() const
	{
		return m_ptr == nullptr;
	}
	template<typename T>
	inline Unique_ptr<T>::~Unique_ptr()
	{
		delete m_ptr;
	}


	template<typename T>
	inline MyPointer::Shared_ptr<T>::ControlBlock::ControlBlock(T* ptr)
		:m_useCount(1), m_ptr(ptr)
	{
	}
	template<typename T>
	inline Shared_ptr<T>::ControlBlock::~ControlBlock()
	{
		delete m_ptr;
	}
	template<typename T>
	inline MyPointer::Shared_ptr<T>::Shared_ptr()
		:m_control(nullptr)
	{
	}
	template<typename T>
	inline Shared_ptr<T>::Shared_ptr(T* ptr)
		:m_control(new ControlBlock(ptr))
	{
	}
	template<typename T>
	inline Shared_ptr<T>::Shared_ptr(const Shared_ptr& other)
		:m_control(other.m_control)
	{
		if (m_control) {
			m_control->m_useCount++;
		}
	}
	template<typename T>
	inline Shared_ptr<T>::Shared_ptr(Shared_ptr&& other) noexcept
		:m_control(other.m_control)
	{
		other.m_control = nullptr;
	}
	template<typename T>
	inline T* Shared_ptr<T>::get() const
	{
		return m_control? m_control->m_ptr:nullptr;
	}
	template<typename T>
	inline size_t Shared_ptr<T>::useCount() const
	{
		return m_control ? m_control->m_useCount : 0;;
	}

	template<typename T>
	inline void Shared_ptr<T>::reset(T* ptr)
	{
		release();
		if (ptr) {
			m_control = new ControlBlock(ptr);
		}
		else {
			m_control = nullptr;
		}
	}

	template<typename T>
	inline T& Shared_ptr<T>::operator*() const
	{
		return *get();
	}
	template<typename T>
	inline T* Shared_ptr<T>::operator->() const
	{
		return get();
	}
	template<typename T>
	inline Shared_ptr<T>& Shared_ptr<T>::operator=(const Shared_ptr& other)
	{
		if (this != &other) {
			release();
			m_control = other.m_control;
			if (m_control) {
				m_control->m_useCount++;
			}
		}
		return *this;
	}
	template<typename T>
	inline Shared_ptr<T>& Shared_ptr<T>::operator=(Shared_ptr&& other) noexcept
	{
		if (this != &other) {
			release();
			m_control = other.m_control;
			other.m_control = nullptr;
		}
		return *this;
	}
	template<typename T>
	inline Shared_ptr<T>::~Shared_ptr()
	{
		release();
	}
	template<typename T>
	inline void Shared_ptr<T>::release()
	{
		if (m_control) {
			if (--m_control->m_useCount == 0) {
				delete m_control;
			}
			m_control = nullptr;
		}
	}
}
