#pragma once
#include <iostream>
#include <utility>

//Блок контроля, хранящий число указателей, указывающих на объект, указатель на объект
//Указатель помещен в блок контроля для централизации хранения и удаления данных
template<typename T>
struct ControlBlock {
	//Кол-во использований
	size_t m_useCount;
	//Кол-во 
	size_t m_weakCount;
	//Указатель
	T* m_ptr;
	//Конструктор инициализации контрольного блока
	ControlBlock(T* ptr);
	//Деструктор(удаляет выделенную память)
	~ControlBlock();
};

namespace MyPointer
{
	template <typename T>
	//Устаревший указатель
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
	//Умный указатель, обеспечивающий эксклюзивное владение объектом
	class Unique_ptr {
		T* m_ptr{};
	public:
		//Конструктор инициализации указателя 
		Unique_ptr(T* ptr = nullptr);
		//Конструктор перемещения
		Unique_ptr(Unique_ptr&& other) noexcept;
		//Оператор присваивания реализующий семантику перемещения
		Unique_ptr& operator=(Unique_ptr&& other) noexcept;

		Unique_ptr(const Unique_ptr& other) = delete;
		Unique_ptr& operator=(const Unique_ptr& other) = delete;

		//Оператор разъименования
		T& operator*() const;
		//Оператор стрелочки(работает как get())
		T* operator->() const;
		//Отчищает выделеную память, инициализируется новым указателем
		void reset(T* other = nullptr);
		//Возвращает true если m_ptr равен nullptr
		bool isNull() const;
		//Возвращает указатель
		T* get() const { return m_ptr; }
		//Деструктор
		~Unique_ptr();
	};

	template <typename T>
	class Weak_ptr;

	template <typename T>
	//Умный указатель, обеспечивающий множественное владение объектом
	class Shared_ptr {
		//Указатель на экземпляр контрольного блока, небходим для того, чтобы другие
		//экземпляры Shared_ptr могли его хранить (множественное владение)
		ControlBlock<T>* m_control;
	public:
		//Конструктор (присваивает nullptr контрольному блоку)
		Shared_ptr();
		//Конструктор (выделяет память на новый контрольный блок)
		explicit Shared_ptr(T* ptr);
		//Конструктор на основе Weak_ptr
		explicit Shared_ptr(const Weak_ptr<T>& weak);
		//Конструктор копирования
		Shared_ptr(const Shared_ptr& other);
		//Конструктор перемещения
		Shared_ptr(Shared_ptr&& other) noexcept;
		//Возвращает указатель на объект
		T* get() const;
		//Возвращет число указателей, указывающих на объект
		size_t useCount() const;
		//Возвращает количесво weak_ptr, указывающих на объект
		size_t weakCount() const;
		//Отчищение старых данных, инициализация новыми
		void reset(T* ptr = nullptr);

		//Оператор разъименования(возвращает ссылку на объект)
		T& operator*() const;
		T* operator->()const;
		//Оператор копирования
		Shared_ptr& operator=(const Shared_ptr& other);
		//Оператор перемещения
		Shared_ptr& operator=(Shared_ptr&& other) noexcept;
		//Деструктор
		~Shared_ptr();
	private:
		//Реализация деструктора
		void release();
		friend class Weak_ptr<T>;
	};

	template<typename T>
	//Слабый Shared_ptr
	class Weak_ptr {
		//Блок контроля, общий с Shared_ptr
		ControlBlock<T>* m_control;
	public:
		//Может инициализироваться только через Shared_ptr
		Weak_ptr() = delete;
		Weak_ptr(const Shared_ptr<T>& shared);
		//Или через такой-же Weak_ptr
		Weak_ptr(const Weak_ptr& other);
		Weak_ptr(Weak_ptr&& other) noexcept;

		Weak_ptr& operator=(const Weak_ptr& other);
		Weak_ptr& operator=(Weak_ptr&& other) noexcept;

		~Weak_ptr();
		//Возвращает true, если все Shared_ptr удалены
		bool expired() const;
		//Фиксирует Weak_ptr(преобразует его в Shared_ptr)
		Shared_ptr<T> lock();

	private:
		void release();
		friend class Shared_ptr<T>;
	};

	namespace Help {
		//Создание экземпляра Unique_ptr
		template<typename T, typename... Args>
		Unique_ptr<T> make_unique(Args&&...args) {
			return Unique_ptr<T>(new T(std::forward<Args>(args)...));
		}
		//Создание экземпляра Shared_ptr
		template<typename T, typename... Args>
		Shared_ptr<T> make_shared(Args&&...args) {
			return Shared_ptr<T>(new T(std::forward<Args>(args)...));
		}

	}
}