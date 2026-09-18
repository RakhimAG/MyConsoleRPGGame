#include "MyString.h"

// --------- SETTERS ------------

void MyString::append(const MyString& other)
{
	// optimization
	if (!this->_str && !other._str)
		return;
	else if (this->_str && !other._str)
		return;

	if (this->_size + other._size > _MAX_SIZE)
		throw std::length_error("Can't append this string. Maximum length of string is 512");

	char* temp = new char[this->_size + other._size + 1];

	int i = 0;
	for (; i < this->_size; i++)
	{
		temp[i] = this->_str[i];
	}
	for (int i2 = 0; i2 < other._size; i++, i2++)
	{
		temp[i] = other._str[i2];
	}

	delete[] this->_str;
	this->_size += other._size;
	temp[this->_size] = '\0';
	this->_str = temp;
}

void MyString::append(const char* raw_str)
{
	if (!raw_str || raw_str[0] == '\0')
		return;
		
	int raw_str_size = calc_rawstr_length(raw_str);

	if (raw_str_size + this->_size > _MAX_SIZE)
		throw std::length_error("Can't append this string. Maximum length of string is 512");

	char* new_str = new char[this->_size + raw_str_size + 1];

	int index = 0;
	for (; index < this->_size; index++)
	{
		new_str[index] = this->_str[index];
	}
	for (int i2 = 0; i2 < raw_str_size; i2++)
	{
		new_str[index] = raw_str[i2];
	}
	delete[] _str;
	_size += raw_str_size;
	new_str[_size] = '\0';
	_str = new_str;
}

MyString MyString::substring(int start, int end) const
{
	if (start > end)
	{
		int temp = end;
		end = start;
		start = temp;
	}
	if (end > this->_size)		throw std::out_of_range("End index is out of range!");
	if (start < 0 || end < 0)	throw std::invalid_argument("Index can't be negative!");

	MyString sub_str;
	sub_str._size = end - start;
	sub_str._str = new char[sub_str._size + 1];
	for (int i = 0, i2 = start; i < sub_str._size; i++, i2++)
	{
		sub_str._str[i] = this->_str[i2];
	}
	sub_str[sub_str._size] = '\0';

	return sub_str;
}

int MyString::find(const char* substr) const
{
	if (!substr)
		throw std::invalid_argument("Null pointer is not considered as string!");

	bool not_substr = false;
	//-------------------------------------------------------------------------
	// 1) this flag will assume - when we start comparing _str and substr in loop -
	// it would not be the actual substr. When that statement is ACTUALLY true, 
	// it exits loop, to decrement 'i' for not loosing one byte of data
	//-------------------------------------------------------------------------
	// 2) if sub-string is in _str then return will be executed,
	// so flag will not cause problems
	//-------------------------------------------------------------------------
	// 3) Purpose - flag becomes true only when chars start to be equal in both sides,
	// it is for not decrementing 'i' extra times, only when while loop failed inside.
	//-------------------------------------------------------------------------
	for (int i = 0, i2 = 0; i < this->_size; i++)
	{
		while (this->_str[i] == substr[i2])
		{
			not_substr = true;
			i2++;
			i++;
			if (substr[i2] == '\0')
				return i - i2;
		}
		if (not_substr)
			i--;
		i2 = 0;
	}

	return -1;
}

void MyString::to_upper()
{
	if (!_str)
		return;

	for (int i = 0; i < _size; i++)
	{
		if (_str[i] >= 97 && _str[i] <= 122)
			_str[i] -= 32;
	}
}

// --------- GETTERS ------------

const char* MyString::c_str() const
{
	return _str;
}

int MyString::length() const { return _size; }

int MyString::get_str_capacity()
{
	return _MAX_SIZE;
}

// --------- CONSTRUCTORS ------------

MyString::MyString() : _str(nullptr), _size(0) {}

MyString::MyString(const char* str) : MyString()
{
	set_raw_string(str);
}

MyString::MyString(const MyString& other)
{
	this->_str = new char[other._size + 1];
	this->_size = other._size;
	for (int i = 0; i < this->_size + 1; i++)
	{
		this->_str[i] = other._str[i];
	}
}

MyString::MyString(MyString&& other) noexcept : _str(other._str), _size(other._size)
{
	other._str = nullptr;
}

MyString::MyString(std::initializer_list<const char*> init_list) : MyString()
{
	size_t init_list_size = init_list.size();

	if (init_list_size == 0)	return;

	auto start = init_list.begin();

	int new_size = 0;
	for (size_t i = 0; i < init_list_size; i++)
	{
		if (*(start + i) == nullptr)
			throw std::invalid_argument("Null pointer was provided istead of string!");
		new_size += calc_rawstr_length(*(start + i));
	}

	if (new_size <= _MAX_SIZE)
		this->_str = new char[new_size + 1];
	else
		throw std::length_error("Can't initialize string from given arguments. Maximum length of string is 512");
			
		
	for (size_t i = 0; i < init_list_size; i++)
	{
		this->append_without_allocating(*(start + i));
	}
	this->_size = new_size;
}

// ---------- DESTRUCTORS -------------

MyString::~MyString()
{
	delete[] this->_str;
}

// ---------- OPERATORS --------------

MyString MyString::operator+(const MyString& other) const
{
	MyString result;
	result._size = this->_size + other._size;
	if (result._size > _MAX_SIZE)
		throw std::length_error("Can't append this string to other. Maximum length of string is 512");

	result._str = new char[result._size + 1];

	int i = 0;
	for (; i < this->_size; i++)
	{
		result._str[i] = this->_str[i];
	}
	for (int i2 = 0; i2 < other._size; i++, i2++)
	{
		result._str[i] = other._str[i2];
	}
	result._str[result._size] = '\0';

	return result;
}

bool MyString::operator >(const MyString& other) const
{
	// iterations should depend on min size of
	// one of the strings
	int n = ( this->length() > other.length() ? other.length() : this->length() );

	for (size_t i = 0; i < n; i++)
	{
		if (this->_str[i] > other._str[i])	return true;
		if (this->_str[i] < other._str[i])	return false;
	}

	return this->length() > other.length();
}

bool MyString::operator <(const MyString& other) const
{
	return !(this->_str > other._str);
}

bool MyString::operator ==(const MyString& other) const
{
	if (this->_size != other._size)
		return false;
	else if (!this->_str || !other._str)
		return false;

	for (int i = 0; i < this->_size; i++)
	{
		if (this->_str[i] != other._str[i])
			return false;
	}
	return true;
}

bool MyString::operator!=(const MyString& other) const
{
	return !(this->_str == other._str);
}

bool MyString::operator !() const
{
	if (_size == 0)		return true;

	return false;
}

MyString& MyString::operator +=(const MyString& other)
{
	this->append(other);
	return *this;
}

MyString& MyString::operator =(const MyString& other)
{
	delete[] this->_str;

	this->_str = new char[other._size + 1];
	this->_size = other._size;

	for (int i = 0; i < other._size; i++)
	{
		this->_str[i] = other._str[i];
	}
	this->_str[this->_size] = '\0';

	return *this;
}

MyString& MyString::operator =(const char* raw_str)
{
	set_raw_string(raw_str);
	return *this;
}

MyString MyString::operator *(const int integer) const
{
	if (integer <= 0)
		throw std::invalid_argument("String can't be multiplyed by zero or below!");
	if (this->_size * integer > _MAX_SIZE)
		throw std::length_error("Tried to create string with length more than 512 characters!");

	MyString result;
	result._size = this->_size * integer;
	result._str = new char[result._size + 1];

	int current_index = 0;
	for (int i = 0; i < integer; i++)
	{
		for (int i2 = 0; i2 < this->_size; i2++, current_index++)
		{
			result._str[current_index] = this->_str[i2];
		}
	}
	result._str[result._size] = '\0';
	return result;
}

MyString MyString::operator /(const int integer) const
{
	if (integer < 0)
		throw std::invalid_argument("You can't remove negative count of characters!");

	else if (integer > this->_size)
		throw std::invalid_argument("You can't remove more than the length of string!");

	else if (integer == 0)
		return *this;

	MyString result;
	result._size = this->_size - integer;
	result._str = new char[result._size + 1];

	for (int i = 0; i < result._size; i++)
	{
		result._str[i] = this->_str[i];
	}
	result._str[result._size] = '\0';

	return result;
}

const char& MyString::operator [](const int index) const
{
	if (index >= this->_size)
		throw std::out_of_range("Index is out of range!");
	else if (index < 0)
		throw std::invalid_argument("Index can't be negative!");

	return this->_str[index];
}

char& MyString::operator[](const int index)
{
	if (index >= this->_size)
		throw std::out_of_range("Index is out of range!");
	else if (index < 0)
		throw std::invalid_argument("Index can't be negative!");

	return this->_str[index];
}

const char& MyString::operator()(const int index) const
{
	if (index >= this->_size)
		throw std::out_of_range("Index is out of range!");
	else if (index < 0)
		throw std::invalid_argument("Index can't be negative!");

	return this->_str[index];
}

char& MyString::operator()(const int index)
{
	if (index >= this->_size)
		throw std::out_of_range("Index is out of range!");
	else if (index < 0)
		throw std::invalid_argument("Index can't be negative!");

	return this->_str[index];
}

MyString::operator int() const
{
	int index = 0;
	bool should_be_negative = false;
	if (!this->_str)							return 0;
	else if (this->_str[index] == '0')			return 0;
	else if (this->_str[index] == '+')			index++;
	else if (this->_str[index] == '-')
	{
		index++;
		should_be_negative = true;
	}

	int integer = 0;
	for (int digit_place = 1; index < this->_size; index++)
	{
		if (this->_str[index] >= '0' && this->_str[index] <= '9')
		{
			digit_place = 1;
			for (int i = 0; i < (this->_size - 1) - index; i++)
			{
				digit_place *= 10;
			}
			// 48 - index of '0' in ASCII table
			// doing minus 48 gives us exact integer
			integer += (this->_str[index] - 48) * digit_place;
		}
		else
			return 0;
	}

	return (should_be_negative) ? integer * -1 : integer;
}

MyString::operator float() const
{
	int dot_index = this->find(".");
	float whole_part = 0.0f;
	float fractional_part = 0.0f;

	int index = 0;
	bool should_be_negative = false;
	if (this->_str[index] == '+')	index++;
	else if (this->_str[index] == '-')
	{
		index++;
		should_be_negative = true;
	}

	float digit_place = 1.0f, digit_place_buffer;
	for (int i = 0; i < dot_index - index - 1; i++)
	{
		digit_place *= 10.0f;
	}

	digit_place_buffer = digit_place;

	for (; index < dot_index; index++)
	{
		if (this->_str[index] >= '0' && this->_str[index] <= '9')
		{
			whole_part += (this->_str[index] - 48) * digit_place;
			digit_place /= 10.0f;
		}
		else
			return 0;
	}

	index++; // skipping over '.'
	digit_place = digit_place_buffer / 10.0f;

	for (; index < this->_size; index++)
	{
		if (this->_str[index] >= '0' && this->_str[index] <= '9')
		{
			fractional_part += (this->_str[index] - 48) * digit_place;
			digit_place /= 10.0f;
		}
		else
			return 0;
	}

	fractional_part /= digit_place_buffer;
	return (should_be_negative) ? -(whole_part + fractional_part) : whole_part + fractional_part;
}

MyString::operator bool() const
{
	if (this->_str == nullptr || this->_size == 0)     return false;

	return true;

}

// --------- HELPER METHODS ------------
void MyString::append_without_allocating(const char* raw_str)
{
	if (!raw_str || raw_str[0] == '\0')
		return;

	int raw_str_size = calc_rawstr_length(raw_str);

	for (int index = 0; index < raw_str_size; index++)
	{
		_str[_size++] = raw_str[index];
	}
}

void MyString::set_raw_string(const char* str)
{
	if (str)
	{
		_size = calc_rawstr_length(str);

		// this is for case when str argument was allocated by 0 bytes
		if (_size == 0)
		{
			_str = nullptr;
			return;
		}
		else if (_size > _MAX_SIZE)
		{
			throw std::length_error("This string has reached beyond maximum length!");
		}

		if (_str)
			delete[] _str;

		_str = new char[_size + 1];

		for (size_t i = 0; i < _size; i++)
		{
			_str[i] = str[i];
		}

		_str[_size] = '\0';
	}
	else
		_str = nullptr;
}

int MyString::calc_rawstr_length(const char* raw_str) const
{
	int size = 0;
	while (raw_str[size] != '\0')
	{
		size++;
	}

	return size;
}

// ------------- STATIC MEMBERS -----------

const int MyString::_MAX_SIZE = 1024;

// ------------- FRIENDS -------------

std::istream& operator >>(std::istream& is, MyString& obj)
{
	char buffer[256];
	is.getline(buffer, 256);

	obj.set_raw_string(buffer);

	return is;
}

std::ostream& operator<<(std::ostream& os, const MyString& obj)
{
	os << obj.c_str();
	return os;
}


