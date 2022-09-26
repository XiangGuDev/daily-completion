#pragma once

// 动态类型转换方式
//std::shared_ptr<base> sp0(new derived);
//std::shared_ptr<derived> sp1 = std::dynamic_pointer_cast<derived>(sp0);

///////////////////////////////////////////////////////////////////////////////////////////////////
// java 对象基类
//

#include <memory>
class COMMONCONTROLS_API Object2
{
public:
	virtual ~Object2() {}

	virtual CString toString();

	virtual bool equals(Object2* arg0);
};
typedef std::shared_ptr<Object2> ObjectPtr;

class ObjectPtrArray : public Object2
{
public:
	ObjectPtrArray(int size);
	~ObjectPtrArray();
	ObjectPtr& operator[](int index);
	//const Object*& operator[](int index) const;
	ObjectPtr& get(size_t index);
	size_t length();
	void reset();
	void clear();

private:
	CAtlArray<ObjectPtr> _array;
	int _length;
};

typedef std::shared_ptr<ObjectPtrArray> ObjectArrayPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////
// 对象比较基类
//
class Comparable
{
public:
	virtual int compareTo(Object2* arg0) = 0;
};

template<class T>
class ComparableT : public Comparable
{
public:
	virtual int compareTo(Object2* arg0)
	{
		return compareTo(dynamic_cast<T*>(arg0));
	}

	virtual int compareTo(T* arg0) = 0;
};