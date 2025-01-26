#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>
namespace MySTL {
	template<typename T, size_t N>
	//����������� ������
	class Array {
		T m_data[N];
		size_t m_size;
	public:
		//����������� ��� �����������
		Array() : m_size(N) {}

		//get ����� ��� �������� ����� �������
		constexpr auto size() const { return m_size; }

		//�������� ���������� ������ ��� ��������� �������� ������� �� ������
		T& operator[](size_t index) {
			if (index >= m_size) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}

		//�������� ���������� ������ ��� ��������� �������� ������� �� ����������� ������
		//��� ����������� ���������
		const T& operator[](size_t index) const {
			if (index >= m_size) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}
	};

	template<typename T>
	//������������ ������
	class Vector {
		//������
		std::unique_ptr<T[]> m_data;
		//����� �������
		size_t m_size;
		//����������� ������ �������
		size_t m_capacity;
	public:
		//�����������
		Vector()
			: m_capacity(1)
			, m_size(0)
			, m_data(std::make_unique<T[]>(m_capacity)) {
		}

		//get �����, ������������ ����� �������
		constexpr auto size() const { return m_size; }
		constexpr auto capacity() const { return m_capacity; }

		//�������� ���������� ������, ��� ������� � �������� ������� �� ������
		T& operator[](size_t index) {
			if (index >= m_size || index < 0) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}

		//�������� ���������� ������, ��� ������� � �������� ������� �� ����������� ������
		const T& operator[](size_t index) const {
			if (index >= m_size || index < 0) {
				throw std::out_of_range("");
			}
			return m_data[index];
		}

		//����� ����������� ������� � ����� �������
		void push_back(const T& value) {
			//���� ���������� �������� ������ �������
			if (m_size >= m_capacity) {
				//��������� ������ ������������ �������
				setNextCapacity();
				//�������� ������ �������
				auto new_data = std::make_unique<T[]>(m_capacity);
				//����������� ������ �� ������� ������� � �����
				for (size_t i = 0; i < m_size; i++)
					new_data[i] = std::move(m_data[i]);
				//���������� ������ ��������
				new_data[m_size++] = value;
				m_data.release(); //��� ��������������� ��������
				m_data = std::move(new_data); //��� ���� ������(����������� ����)
			}
			//���� ��� ������������� �������� ������ ��� ����� ������� ������
			else {
				m_data[m_size++] = value;
			}
		}

		//�������� �������� �� index �������
		void erase(size_t index) {
			//�������� �� ����� �� ������� �������
			if (index >= m_size || index < 0) {
				throw std::out_of_range("");
			}

			if (index + 1 < m_size) {
				//��������(��������) � ������� ��������� �������
				for (size_t i = index; i < m_size - 1; i++)
				{
					m_data[i] = std::move(m_data[i + 1]);
				}
			}
			//���� index ����� ���������� �������� � ������
			//�� ������� ���������� � � ���� �������� ������
			else {
				m_data[index] = T{};
			}
			--m_size;
		}

		//��������� value � index ����� �������
		void insert(const T& value, size_t index) {
			//�������� �� ����� �� ������� �������
			if (index > m_size || index < 0) {
				throw std::out_of_range("");
			}
			//���� ���������� �������� ������ �������
			if (m_size >= m_capacity) {
				//��������� ������ ������������ �������
				setNextCapacity();
				//�������� ������ �������
				auto new_data = std::make_unique<T[]>(m_capacity);
				m_data.release();
				//���������� ������ �������
				m_data = std::move(new_data);
			}
			//�������� ��������� � ������ ������� �� index
			for (size_t i = m_size; i > index; i--)
			{
				m_data[i] = std::move(m_data[i - 1]);
			}
			m_data[index] = value;
			++m_size;
		}

	private:
		//����� ����������� �������������� ���������� ����������� ������ ��������
		void setNextCapacity() {
			const double k = 1.4;
			m_capacity += static_cast<size_t>(1 + k * m_capacity);
		}
	};

	template<typename T>
	//������� �������
	class Deque {
		//�������� �������(����)
		struct Node {
			//��������� �� ��������� �������
			std::shared_ptr<Node>next;
			//�� ����������
			std::shared_ptr<Node>perv;
			//��������
			T value;
			//�����������
			Node(const T& value)
				: value(value)
				, next(nullptr)
				, perv(nullptr) {
			}
		};
		//�������� ������(������)
		std::shared_ptr<Node> head;
		//��������� ������(���������)
		std::shared_ptr<Node> tail;
	public:
		Deque()
			: head(nullptr)
			, tail(nullptr) {
		}

		//��������� ������� � ����� �������
		void push_back(const T& value) {
			//���� head == nullptr
			if (!head) {
				head = std::make_shared<Node>(value);
				tail = head;
			}
			//�����
			else {
				//������� ��������� �������
				tail->next = std::make_shared<Node>(value);
				//��������� ��������� ������ �� ���������� ������� ����������
				tail->next->perv = tail;
				//������ ��������� �������� �������
				tail = tail->next;
			}
		}
		//������� � ���������� �������� ���������� ��������(����)
		T pop_back() {
			//���� tail == nullptr
			if (!tail) {
				throw std::out_of_range("");
			}
			//������� ������������ ��������
			T result = tail->value;
			//���� ���������� ���������� �������(����)
			if (tail->perv) {
				tail = tail->perv;
				tail->next = nullptr;
			}
			//����� �������� ������ � �����
			else {
				tail = nullptr;
				head = nullptr;
			}
			return result;
		}

		//��������� ������� � ������ �������
		void push_front(const T& value) {
			//���� head == nullptr
			if (!head) {
				head = std::make_shared<Node>(value);
				tail = head;
			}
			//�����
			else {
				head->perv = std::make_shared<Node>(value);
				head->perv->next = head;
				head = head->perv;
			}
		}

		//������� � ���������� �������� ������� ��������(����)
		T pop_front() {
			if (!head) {
				throw std::out_of_range("");
			}
			T result = head->value;

			if (head->next) {
				head = head->next;
				head->perv = nullptr;
			}
			//����� �������� ������ � �����
			else {
				tail = nullptr;
				head = nullptr;
			}
			return result;
		}
	};

	template<typename T>
	//���� (������ ����� - ��������� �����)
	//���������� ����� ���� Vector
	class Stack {
		Vector<T> m_data;
	public:
		Stack()
			: m_data(Vector<T>()) {
		}
		//��������� ������� � ����� �����
		void push(const T& value) {
			m_data.push_back(value);
		}
		//������� � ���������� ������� � ����� �����
		T pop() {
			size_t length = m_data.size();
			if (length < 1) {
				throw std::out_of_range("");
			}
			T result = m_data[length - 1];
			m_data.erase(length - 1);
			return result;
		}

		//���� ����� ��������� ���������� ��������(����� ��������)
	};

	template<typename T>

	class Set {
		//�������� ��������� ������(����)
		struct Node {
			//������� ������� �� ��������
			std::shared_ptr<Node>left;
			//������� ������� �� ��������
			std::shared_ptr<Node>right;
			//��������
			T value;

			Node(const T& value)
				: left(nullptr)
				, right(nullptr)
				, value(value) {
			}
		};
		//������ ������
		std::shared_ptr<Node> m_root;
		size_t m_size;

		std::shared_ptr<Node> findMin(std::shared_ptr<Node>& node) const {
			while (node && node->left) {
				node = node->left;
			}
			return node;
		}

		//����������� �������
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
				return node; // ���� �� ������
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

		//����������� ����������� ��������� � ������
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

		//�������� ������� � �������� ������
		void insert(const T& value) {
			insert(m_root, value);
		}

		void erase(const T& value) {
			erase(m_root, value);
		}

		//�������� set � ���� vector
		Vector<T> getVector() const {
			auto result = Vector<T>();
			getVector(m_root, result);
			return result;
		}


	};
}