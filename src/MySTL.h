#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>
namespace MySTL {
	template<typename T, size_t N>
	//Статический массив
	class Array {
		T m_data[N];
		size_t m_size;
	public:
		//Конструктор без параментров
		Array() : m_size(N) {}

		//get метод для возврата длины массива
		constexpr auto size() const { return m_size; }

		//Оператор квадратных скобок для получения элемента массива по ссылке
		T& operator[](size_t index) {
			if (index >= m_size) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}

		//Оператор квадратных скобок для получения элемента массива по константной ссылке
		//Для константных выражений
		const T& operator[](size_t index) const {
			if (index >= m_size) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}
	};

	template<typename T>
	//Динамический массив
	class Vector {
		//Массив
		std::unique_ptr<T[]> m_data;
		//Длина массива
		size_t m_size;
		//Фактический размер массива
		size_t m_capacity;
	public:
		//Конструктор
		Vector()
			: m_capacity(1)
			, m_size(0)
			, m_data(std::make_unique<T[]>(m_capacity)) {
		}

		//get метод, возвращающий длину массива
		constexpr auto size() const { return m_size; }
		constexpr auto capacity() const { return m_capacity; }

		//Оператор квадратных скобок, для доступа к элементу массива по ссылке
		T& operator[](size_t index) {
			if (index >= m_size || index < 0) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}

		//Оператор квадратных скобок, для доступа к элементу массива по константной ссылке
		const T& operator[](size_t index) const {
			if (index >= m_size || index < 0) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}

		//Метод добавляющий элемент в конец массива
		void push_back(const T& value) {
			//Если необходимо создание нового массива
			if (m_size >= m_capacity) {
				//установка нового фактического размера
				setNextCapacity();
				//создание нового массива
				auto new_data = std::make_unique<T[]>(m_capacity);
				//перемещение данных со старого массива в новый
				for (size_t i = 0; i < m_size; i++)
					new_data[i] = std::move(m_data[i]);
				//добавление нового значения
				new_data[m_size++] = value;
				m_data.release(); //Без принудительного очищения
				m_data = std::move(new_data); //ТУТ БЫЛА ОШИБКА(повреждение кучи)
			}
			//если нет необходимости выделять память под более большой массив
			else {
				m_data[m_size++] = value;
			}
		}

		//Удаление элемента на index позиции
		void erase(size_t index) {
			//Проверка на выход за границы массива
			if (index >= m_size || index < 0) {
				throw std::out_of_range("");
			}

			if (index + 1 < m_size) {
				//смещение(удаление) в сторону удалённого объекта
				for (size_t i = index; i < m_size - 1; i++)
				{
					m_data[i] = std::move(m_data[i + 1]);
				}
			}
			//если index равен последнему элементу в масиве
			//то элемент зануляется и к нему теряется доступ
			else {
				m_data[index] = T{};
			}
			--m_size;
		}

		//Вставляет value в index место массива
		void insert(const T& value, size_t index) {
			//Проверка на выход за границы массива
			if (index > m_size || index < 0) {
				throw std::out_of_range("");
			}
			//Если необходимо создание нового массива
			if (m_size >= m_capacity) {
				//установка нового фактического размера
				setNextCapacity();
				//создание нового массива
				auto new_data = std::make_unique<T[]>(m_capacity);
				m_data.release();
				//присваение нового массива
				m_data = std::move(new_data);
			}
			//смещение элементов в правую сторону до index
			for (size_t i = m_size; i > index; i--)
			{
				m_data[i] = std::move(m_data[i - 1]);
			}
			m_data[index] = value;
			++m_size;
		}

	private:
		//Метод позволяюзий оптимизировать количество создаваемых заново массивов
		void setNextCapacity() {
			const double k = 1.4;
			m_capacity += static_cast<size_t>(1 + k * m_capacity);
		}
	};

	template<typename T>
	//Двойная очередь
	class Deque {
		//Элементы очереди(узлы)
		struct Node {
			//указатель на следующий элемент
			std::shared_ptr<Node>next;
			//на предыдущий
			std::shared_ptr<Node>perv;
			//значение
			T value;
			//конструктор
			Node(const T& value)
				: value(value)
				, next(nullptr)
				, perv(nullptr) {
			}
		};
		//головной объект(первый)
		std::shared_ptr<Node> head;
		//хвостовой объект(последний)
		std::shared_ptr<Node> tail;
	public:
		Deque()
			: head(nullptr)
			, tail(nullptr) {
		}

		//добавляет элемент в конец очереди
		void push_back(const T& value) {
			//Если head == nullptr
			if (!head) {
				head = std::make_shared<Node>(value);
				tail = head;
			}
			//Иначе
			else {
				//Создаем следующий элемент
				tail->next = std::make_shared<Node>(value);
				//Замудрённо добавляем ссылку на предыдущий элемент следующему
				tail->next->perv = tail;
				//Теперь следующий является хвостом
				tail = tail->next;
			}
		}
		//удаляет и возвращает значение последнего элемента(узла)
		T pop_back() {
			//если tail == nullptr
			if (!tail) {
				throw std::out_of_range("");
			}
			//Достаем возвращаемое значение
			T result = tail->value;
			//Если существует предыдущий элемент(узел)
			if (tail->perv) {
				tail = tail->perv;
				tail->next = nullptr;
			}
			//Иначе зануляем голову и хвост
			else {
				tail = nullptr;
				head = nullptr;
			}
			return result;
		}

		//добавляет элемент в начало очереди
		void push_front(const T& value) {
			//Если head == nullptr
			if (!head) {
				head = std::make_shared<Node>(value);
				tail = head;
			}
			//Иначе
			else {
				head->perv = std::make_shared<Node>(value);
				head->perv->next = head;
				head = head->perv;
			}
		}

		//удаляет и возвращает значение первого элемента(узла)
		T pop_front() {
			if (!head) {
				throw std::out_of_range("");
			}
			T result = head->value;

			if (head->next) {
				head = head->next;
				head->perv = nullptr;
			}
			//Иначе зануляем голову и хвост
			else {
				tail = nullptr;
				head = nullptr;
			}
			return result;
		}
	};

	template<typename T>
	//Стэк (первый вошел - последний вышел)
	//Реализовал через свой Vector
	class Stack {
		Vector<T> m_data;
	public:
		Stack()
			: m_data(Vector<T>()) {
		}
		//Добавляет элемент в конец стека
		void push(const T& value) {
			m_data.push_back(value);
		}
		//Удаляет и возвращает элемент с конца стека
		T pop() {
			size_t length = m_data.size();
			if (length < 1) {
				throw std::out_of_range("");
			}
			T result = m_data[length - 1];
			m_data.erase(length - 1);
			return result;
		}

		//Есть метод просмотра последнего элемента(можно добавить)
	};

	template<typename T>

	class Set {
		//Элементы бинарного дерева(узлы)
		struct Node {
			//Элемент меньший по значению
			std::shared_ptr<Node>left;
			//Элемент больший по значению
			std::shared_ptr<Node>right;
			//Значение
			T value;

			Node(const T& value)
				: left(nullptr)
				, right(nullptr)
				, value(value) {
			}
		};
		//корень дерева
		std::shared_ptr<Node> m_root;
		size_t m_size;

		std::shared_ptr<Node> findMin(std::shared_ptr<Node>& node) const {
			while (node && node->left) {
				node = node->left;
			}
			return node;
		}

		//Рекурсивная вставка
		void insert(std::shared_ptr<Node>& node, const T& value) {
			if (!node) {
				node = std::make_shared<Node>(value);
				++m_size;
			}
			else if (value < node->value) {
				insert(node->left, value);
			}
			else if (value > node->value) {
				insert(node->right, value);
			}
		}

		std::shared_ptr<Node> erase(std::shared_ptr<Node> &node, const T& value) {
			if (!node) {
				return node; // Узел не найден
			}

			if (value < node->value) {
				node->left = erase(node->left, value);
			}
			else if (value > node->value) {
				node->right = erase(node->right, value);
			}
			else {
				if (!node->left && !node->right) {
					return nullptr;
				}
				else if (!node->left) {
					return node->right;
				}
				else if (!node->right) {
					return node->left;
				}
				else {
					const std::shared_ptr<Node>& minNode = findMin(node->right);
					node->value = minNode->value;
					node->right = erase(node->right, minNode->value);
				}
			}
			return node;
		}

		//Рекурсивное добаваление элементов в вектор
		void getVector(const std::shared_ptr<Node>& node, Vector<T>& arr) const {
			if (node) {
				getVector(node->left, arr);
				arr.push_back(node->value);
				getVector(node->right, arr);
			}
		}

	public:
		Set() :m_root(nullptr),m_size(0) {}

		constexpr auto size()const { return m_size; }

		//Вставить элемент в бинарное дерево
		void insert(const T& value) {
			insert(m_root, value);
		}

		void erase(const T& value) {
			erase(m_root, value);
		}

		//Получить set в виде vector
		Vector<T> getVector() const {
			auto result = Vector<T>();
			getVector(m_root, result);
			return result;
		}


	};
}