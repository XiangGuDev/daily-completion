#pragma once

#include "Object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// สýื้
//
template<class T>
class Array2 : public Object2
{
public:
	Array2(size_t size)
	{
		ATLASSERT(size >= 0);
		_size = size;

		if (_size > 0)
			_buffer = new T[size];
		else
			_buffer = NULL;
	}

	~Array2()
	{
		if (_buffer)
			delete[] _buffer;
	}

	T* get()
	{
		return _buffer;
	}

	T& operator[](size_t pos)
	{
		return _buffer[pos];
	}

	size_t length()
	{
		return _size;
	}

	Array2<T>* clone()
	{
		Array2<T>* newArr = new Array2<T>(_size);
		for (size_t i = 0; i < _size; i++)
			(*newArr)[i] = (*this)[i];

		return newArr;
	}

private:
	T*	_buffer;
	size_t	_size;
};

typedef Array2<BYTE> ByteArray2;
typedef Array2<int> IntArray2;
typedef Array2<bool> BoolArray2;

typedef std::shared_ptr<ByteArray2> ByteArrayPtr;
typedef std::shared_ptr<IntArray2> IntArray2Ptr;
typedef std::shared_ptr<BoolArray2> BoolArrayPtr;