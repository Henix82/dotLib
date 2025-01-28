#pragma once
#include <iostream>
#include <utility>

//���� ��������, �������� ����� ����������, ����������� �� ������, ��������� �� ������
//��������� ������� � ���� �������� ��� ������������� �������� � �������� ������
template<typename T>
struct ControlBlock {
	//���-�� �������������
	size_t m_useCount;
	//���-�� 
	size_t m_weakCount;
	//���������
	T* m_ptr;
	//����������� ������������� ������������ �����
	ControlBlock(T* ptr);
	//����������(������� ���������� ������)
	~ControlBlock();
};

namespace MyPointer
{
	template <typename T>
	//���������� ���������
	class Auto_ptr {
		T* m_ptr{};
	public:
		Auto_ptr(T* ptr = nullptr);
		Auto_ptr(Auto_ptr& other);

		T& operator*() const;
		T* operator->() const;
		Auto_ptr& operator=(Auto_ptr& other);

		bool isNull() const;
		T* get() const { return m_ptr; }
		~Auto_ptr();
	};

	template <typename T>
	//����� ���������, �������������� ������������ �������� ��������
	class Unique_ptr {
		T* m_ptr{};
	public:
		//����������� ������������� ��������� 
		Unique_ptr(T* ptr = nullptr);
		//����������� �����������
		Unique_ptr(Unique_ptr&& other) noexcept;
		//�������� ������������ ����������� ��������� �����������
		Unique_ptr& operator=(Unique_ptr&& other) noexcept;

		Unique_ptr(const Unique_ptr& other) = delete;
		Unique_ptr& operator=(const Unique_ptr& other) = delete;

		//�������� ��������������
		T& operator*() const;
		//�������� ���������(�������� ��� get())
		T* operator->() const;
		//�������� ��������� ������, ���������������� ����� ����������
		void reset(T* other = nullptr);
		//���������� true ���� m_ptr ����� nullptr
		bool isNull() const;
		//���������� ���������
		T* get() const { return m_ptr; }
		//����������
		~Unique_ptr();
	};

	template <typename T>
	class Weak_ptr;

	template <typename T>
	//����� ���������, �������������� ������������� �������� ��������
	class Shared_ptr {
		//��������� �� ��������� ������������ �����, �������� ��� ����, ����� ������
		//���������� Shared_ptr ����� ��� ������� (������������� ��������)
		ControlBlock<T>* m_control;
	public:
		//����������� (����������� nullptr ������������ �����)
		Shared_ptr();
		//����������� (�������� ������ �� ����� ����������� ����)
		explicit Shared_ptr(T* ptr);
		//����������� �� ������ Weak_ptr
		explicit Shared_ptr(const Weak_ptr<T>& weak);
		//����������� �����������
		Shared_ptr(const Shared_ptr& other);
		//����������� �����������
		Shared_ptr(Shared_ptr&& other) noexcept;
		//���������� ��������� �� ������
		T* get() const;
		//��������� ����� ����������, ����������� �� ������
		size_t useCount() const;
		//���������� ��������� weak_ptr, ����������� �� ������
		size_t weakCount() const;
		//��������� ������ ������, ������������� ������
		void reset(T* ptr = nullptr);

		//�������� ��������������(���������� ������ �� ������)
		T& operator*() const;
		T* operator->()const;
		//�������� �����������
		Shared_ptr& operator=(const Shared_ptr& other);
		//�������� �����������
		Shared_ptr& operator=(Shared_ptr&& other) noexcept;
		//����������
		~Shared_ptr();
	private:
		//���������� �����������
		void release();
		friend class Weak_ptr<T>;
	};

	template<typename T>
	//������ Shared_ptr
	class Weak_ptr {
		//���� ��������, ����� � Shared_ptr
		ControlBlock<T>* m_control;
	public:
		//����� ������������������ ������ ����� Shared_ptr
		Weak_ptr() = delete;
		Weak_ptr(const Shared_ptr<T>& shared);
		//��� ����� �����-�� Weak_ptr
		Weak_ptr(const Weak_ptr& other);
		Weak_ptr(Weak_ptr&& other) noexcept;

		Weak_ptr& operator=(const Weak_ptr& other);
		Weak_ptr& operator=(Weak_ptr&& other) noexcept;

		~Weak_ptr();
		//���������� true, ���� ��� Shared_ptr �������
		bool expired() const;
		//��������� Weak_ptr(����������� ��� � Shared_ptr)
		Shared_ptr<T> lock();

	private:
		void release();
		friend class Shared_ptr<T>;
	};

	namespace Help {
		//�������� ���������� Unique_ptr
		template<typename T, typename... Args>
		Unique_ptr<T> make_unique(Args&&...args) {
			return Unique_ptr<T>(new T(std::forward<Args>(args)...));
		}
		//�������� ���������� Shared_ptr
		template<typename T, typename... Args>
		Shared_ptr<T> make_shared(Args&&...args) {
			return Shared_ptr<T>(new T(std::forward<Args>(args)...));
		}

	}
}