#pragma once
#include <iostream>
#include "MyString.h"

class GameExecptions : public std::exception
{
protected:
    MyString _message;

public:
    explicit GameExecptions(const MyString& message) : _message(message) {}
    const char* what() const noexcept override { return _message.c_str(); }

};

class FileOperationException : public GameExecptions
{
public:
    explicit FileOperationException(const MyString& message)
        : GameExecptions(MyString("File operation failed: ") + message) {
    }
};

