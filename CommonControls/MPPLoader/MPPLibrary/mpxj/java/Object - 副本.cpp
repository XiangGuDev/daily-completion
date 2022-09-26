#include "stdafx.h"
#include "Object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Object
//
CString Object::toString()
{
	//ATLASSERT(FALSE);
	static CString nullText;
	return nullText;
};

bool Object::equals(Object* obj)
{
	return this == obj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// ObjectPtrArray
//
ObjectPtrArray::ObjectPtrArray(int size)
{
	_array.SetCount(size);
	//_length = size;
	//_array = (Object**) new Object*[size];
	//memset(_array, 0, sizeof(Object*)* size);
}

ObjectPtrArray::~ObjectPtrArray()
{
	clear();
}

ObjectPtr& ObjectPtrArray::operator[](int index)
{
	return _array[index];
}

//const Object*& ObjectPtrArray::operator[](int index) const
//{
//	return _array[index];
//}

ObjectPtr& ObjectPtrArray::get(size_t index)
{
	return _array[index];
}

size_t ObjectPtrArray::length()
{
	return _array.GetCount();
}

void ObjectPtrArray::reset()
{
	for (size_t i = 0; i < _array.GetCount(); i++)
	{
		_array[i].reset();
	}
}

void ObjectPtrArray::clear()
{
	reset();
	_array.RemoveAll();
}