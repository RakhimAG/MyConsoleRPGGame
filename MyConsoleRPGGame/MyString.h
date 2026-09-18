#ifndef MY_STRING
#define MY_STRING
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

class MyString
{
private:
	char* _str;
	int _size;

	static const int _MAX_SIZE;
public:
	// --------- SETTERS ------------

	void append(const MyString& other);

	void append(const char* raw_str);

	MyString substring(int start, int end) const;

	int find(const char* substr) const;

	void to_upper();

	// --------- GETTERS ------------

	const char* c_str() const;

	int length() const;

	static int get_str_capacity();

	// --------- CONSTRUCTORS ------------

	MyString();

	explicit MyString(const char* str);

	MyString(const MyString& other);

	MyString(MyString&& other) noexcept;

	MyString(std::initializer_list<const char*> init_list);

	// ---------- DESTRUCTORS -------------

	~MyString();

	// ---------- OPERATORS --------------

	MyString operator+(const MyString& other) const;

	bool operator >(const MyString& other) const;

	bool operator <(const MyString& other) const;

	bool operator ==(const MyString& other) const;

	bool operator!=(const MyString& other) const;

	bool operator !() const;

	MyString& operator +=(const MyString& other);

	MyString& operator =(const MyString& other);

	MyString& operator =(const char* raw_str);

	MyString operator *(const int integer) const;

	MyString operator /(const int integer) const;

	const char& operator[](const int index) const;

	char& operator[](const int index);

	const char& operator()(const int index) const;

	char& operator()(const int index);

	operator int() const;

	operator float() const;

	operator bool() const;

	// ----------- FRIENDS -----------

	friend std::ostream& operator <<(std::ostream& os, const MyString& argument);

	friend std::istream& operator >>(std::istream& is, MyString& obj);

private:
	// --------- HELPER METHODS ------------
	void append_without_allocating(const char* raw_str);

	void set_raw_string(const char* str);

	int calc_rawstr_length(const char* raw_str) const;

};
#endif // !MY_STRING