#pragma once
#include <iostream>	// for std::move and exceptions

template <typename T>
class MyDynamicArray
{
private:
	T* _arr;
	size_t _size;
	size_t _capacity;

public:

	// -------- CONSTRUCTORS ---------

	MyDynamicArray();

	MyDynamicArray(size_t capacity);

	MyDynamicArray(const MyDynamicArray& other);

	MyDynamicArray(MyDynamicArray&& other) noexcept;

	MyDynamicArray(std::initializer_list<T> args);

	// --------- DESTRUCTOR ----------

	inline ~MyDynamicArray() { delete[] _arr; }

	// -------- OPERATORS ------------

	MyDynamicArray& operator =(const MyDynamicArray& other);

	MyDynamicArray& operator =(MyDynamicArray&& other) noexcept;

	T& operator[](size_t index);

	const T& operator[](size_t index) const;

	// ------- METHODS ---------

	void push_back(const T& element);

	void pop_back();

	void insert(size_t index, const T& element);

	void remove_at(size_t index);

	void clear();

	void resize();

	bool is_empty() const;

	// ------- SAVING INTO STREAM -------

	void save(std::ostream& out) const;

	// ===============================================================
	// No load - because it is not his job to care about this.
	// The owner of this container should do the loading job instead.
	// Why? - because of polymorphic objects. This dude does not know about them.
	// ===============================================================

	// ------- GETTERS ---------

	size_t get_size() const;

	size_t get_capacity() const;
};

// ========= CONSTRUCTORS ==========
template<typename T>
inline MyDynamicArray<T>::MyDynamicArray() : _arr(nullptr), _size(0), _capacity(0) {}

template<typename T>
inline MyDynamicArray<T>::MyDynamicArray(size_t capacity)
{
	if (capacity <= 0)
	{
		_arr = nullptr;
		_size = 0;
		_capacity = 0;
		return;
	}

	_arr = new T[capacity];
	_size = 0;
	_capacity = capacity;
}

template<typename T>
MyDynamicArray<T>::MyDynamicArray(const MyDynamicArray& other)
	: _arr(nullptr), _size(other._size), _capacity(other._capacity)
{
	if (_capacity == 0)
	{
		_arr = nullptr;
		return;
	}

	_arr = new T[_capacity];

	for (size_t i = 0; i < _size; i++)
	{
		_arr[i] = other._arr[i];
	}
}

template<typename T>
MyDynamicArray<T>::MyDynamicArray(MyDynamicArray&& other) noexcept
{
	this->_arr = new T[other._capacity];

	for (size_t i = 0; i < other._size; i++)
	{
		this->_arr[i] = std::move(other._arr[i]);
	}

	this->_size = other._size;
	this->_capacity = other._capacity;

	other._arr = nullptr;
	other._capacity = 0;
	other._size = 0;
}

template<typename T>
MyDynamicArray<T>::MyDynamicArray(std::initializer_list<T> args)
{
	if (args.size() == 0)
	{
		_arr = nullptr;
		_size = 0;
		_capacity = 0;
		return;
	}

	_arr = new T[args.size()];
	_size = _capacity = args.size();

	auto first_elem = args.begin();

	for (size_t i = 0; i < args.size(); i++)
	{
		_arr[i] = *(first_elem + i);
	}
}

// ========== OPERATORS ===========
template <typename T>
MyDynamicArray<T>& MyDynamicArray<T>::operator=(const MyDynamicArray& other)
{
	if (this == &other)
		return *this;

	T* new_arr = nullptr;

	if (other._capacity > 0)
	{
		new_arr = new T[other._capacity];

		for (size_t i = 0; i < other._size; i++)
			new_arr[i] = other._arr[i];
	}

	delete[] _arr;

	_arr = new_arr;
	_size = other._size;
	_capacity = other._capacity;

	return *this;
}

template <typename T>
MyDynamicArray<T>& MyDynamicArray<T>::operator =(MyDynamicArray&& other) noexcept
{
	if (this->_arr)		delete[] this->_arr;

	this->_arr = new T[other._capacity];
	for (size_t i = 0; i < other._size; i++)
	{
		this->_arr[i] = std::move(other._arr[i]);
	}
	this->_size = other._size;
	this->_capacity = other._capacity;

	other._arr = nullptr;
	other._capacity = 0;
	other._size = 0;

	return *this;
}

template <typename T>
T& MyDynamicArray<T>::operator[](size_t index)
{
	if (index < 0)				return *this->_arr;	// throw invalid arg
	if (index >= this->_size)	return *(this->_arr + (this->_size - 1));	// throw length err

	return *(this->_arr + index);
}

template <typename T>
const T& MyDynamicArray<T>::operator[](size_t index) const
{
	if (index < 0)				return *this->_arr;	// throw invalid arg
	if (index >= this->_size)	return *(this->_arr + (this->_size - 1));	// throw length err

	return *(this->_arr + index);
}

// ========== GETTERS ============
template <typename T>
size_t MyDynamicArray<T>::get_size() const
{
	return _size;
}

template <typename T>
size_t MyDynamicArray<T>::get_capacity() const
{
	return _capacity;
}

// =========== METHODS =============

template <typename T>
void MyDynamicArray<T>::push_back(const T& element)
{
	if (_size == _capacity)
		resize();

	_arr[_size++] = element;
}
	
template <typename T>
void MyDynamicArray<T>::pop_back()
{
	if (_size == 0)	return;

	--_size;
}

template <typename T>
void MyDynamicArray<T>::insert(size_t index, const T& element)
{
	if (index > _size)		throw std::out_of_range("Index is out of range!");

	if (index == _size)
	{
		push_back(element);
		return;
	}

	if (_size >= _capacity)	resize();

	for (size_t i = _size; i > index; i--)
	{
		_arr[i] = _arr[i - 1];
	}
	_arr[index] = element;
	_size++;
}

template <typename T>
void MyDynamicArray<T>::remove_at(size_t index)
{
	if (index < 0)			throw std::invalid_argument("Index was given as negative number!");
	if (index >= _size)		throw std::out_of_range("Index is out of range!");
	if (index == _size - 1)
	{
		pop_back();
		return;
	}

	for (size_t i = index; i < _size - 1; i++)
	{
		_arr[i] = _arr[i + 1];
	}
	--_size;
}

template <typename T>
void MyDynamicArray<T>::clear()
{
	_size = 0;
}

template <typename T>
void MyDynamicArray<T>::resize()
{
	size_t new_capacity = (_capacity == 0) ? 1 : _capacity * 2;

	T* new_arr = new T[new_capacity];

	for (size_t i = 0; i < _size; i++)
	{
		new_arr[i] = _arr[i];
	}

	delete[] _arr;

	_arr = new_arr;
	_capacity = new_capacity;
}

template<typename T>
inline bool MyDynamicArray<T>::is_empty() const
{
	if (!_size)		return true;

	return false;
}

// ============= FILE MANAGING ===========

template<typename T>
void MyDynamicArray<T>::save(std::ostream& out) const
{
	size_t size = _size;
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));

	for (size_t i = 0; i < size; i++)
	{
		_arr[i]->save(out);
	}
}