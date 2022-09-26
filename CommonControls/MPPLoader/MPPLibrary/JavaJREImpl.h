#pragma once

#include "mpxj\java\Format.h"
#include "mpxj\java\Variant.h"
#include "mpxj\java\Array.h"
#include "mpxj\java\Calendar.h"
#include "mpxj\java\Locale.h"
//
typedef Array2<DatePtr> DateArray;
typedef std::shared_ptr<DateArray> DateArrayPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////
// 兼容java的数组容器
//
template <class T>
class Iterator
{
public:
	typedef Iterator<T> _this_type;
	typedef CAtlArray<T> OwnerType;

	Iterator(Object2* list)
	{
		_index = 0;
		_list = dynamic_cast<OwnerType*> (list);
	}

	_this_type& operator = (_this_type& src)
	{
		_index = 0;
		_list = src._list;
		return *this;
	}

	bool hasNext()
	{
		size_t nxt = _index + 1;
		return nxt <= _list->GetCount();
	}

	T next()
	{
		if (hasNext())
		{
			T val = _list->GetAt(_index);
			_index++;
			return val;
		}
		return NULL;
	}

private:
	OwnerType* _list;
	int _index;
};

template<class T>
class List : public CAtlArray<T>, public Object2
{
public:
	typedef List<T> _this_type;
	typedef T VALUE_TYPE;

	bool contains(T obj)
	{
		return indexOf(obj) >= 0;
	}

	bool remove(T obj)
	{
		int idx = indexOf(obj);
		if (idx >= 0)
			RemoveAt(idx);
		return idx >= 0;
	}

	void removeLast()
	{
		if (GetCount() > 0)
		{
			RemoveAt(GetCount() - 1);
		}
	}

	bool isEmpty()
	{
		return GetCount() == 0;
	}

	int indexOf(T obj)
	{
		for (int i = 0; i < (int) GetCount(); i++)
		{
			if (GetAt(i) == obj)
				return i;
		}
		return -1;
	}

	Iterator<T> iterator()
	{
		return Iterator<T>(this);
	}
};

class ListIterator
{
public:
	virtual size_t get_count() = 0;
	virtual ObjectPtr get_item(size_t index) = 0;
};

typedef std::shared_ptr<ListIterator> ListIteratorPtr;

template<class T>
class ObjectList : public List<T>, public ListIterator
{
public:
	virtual size_t get_count()
	{
		return GetCount();
	}

	virtual ObjectPtr get_item(size_t index)
	{
		return std::dynamic_pointer_cast<Object2> (GetAt(index));
	}
};

class Collections
{
public:

	//// 列表排序
	//template<class T>
	//static void sort(T& list)
	//{
	//	//ATLASSERT(FALSE);
	//	// 传统排序算法
	//	for (size_t i = 0; i < list.GetCount(); i++)
	//	{
	//		T::VALUE_TYPE compVal = list.GetAt(i);

	//		for (size_t j = i + 1; j < list.GetCount(); j++)
	//		{
	//			T::VALUE_TYPE nxtVal = list.GetAt(j);

	//			Comparable* nxtVal_c = dynamic_cast<Comparable*>(nxtVal);
	//			ATLASSERT(nxtVal_c != NULL);
	//			if (nxtVal_c == NULL) continue;

	//			// 如果下一条小于上一条则交换
	//			if (nxtVal_c->compareTo(compVal) < 0)
	//			{
	//				list.SetAt(i, nxtVal);
	//				list.SetAt(j, compVal);
	//			}
	//		}
	//	}
	//}

	template<class T>
	static int compareObjectTo(const void *a, const void *b)
	{
		T::VALUE_TYPE* aa = (T::VALUE_TYPE*) a;
		T::VALUE_TYPE* bb = (T::VALUE_TYPE*) b;

		return (*aa)->compareTo((*bb).get());
	}

	// 排序具有Comparable接口的列表对象，如List<Task*>
	template<class T>
	static void sort_object_list(T& list)
	{
		// 先把列表转换成指针数组
		Array2<T::VALUE_TYPE> ValueArray(list.GetCount());
		for (int i = 0; i < (int) list.GetCount(); i++) {
			ValueArray[i] = list.GetAt(i);
		}

		std::qsort(ValueArray.get(), list.GetCount(), sizeof(T::VALUE_TYPE), compareObjectTo<T>);

		// 保存新的结果
		for (int i = 0; i < (int) list.GetCount(); i++) {
			list.SetAt(i, ValueArray[i]);
		}
	}

	template<class T>
	static int comparePtrTo(const void *a, const void *b)
	{
		T::VALUE_TYPE* aa = (T::VALUE_TYPE*) a;
		T::VALUE_TYPE* bb = (T::VALUE_TYPE*) b;

		return (*aa)->compareTo(*bb);
	}

	// 排序具有Comparable接口的列表对象，如List<Task*>
	template<class T>
	static void sort_ptr_list(T& list)
	{
		// 先把列表转换成指针数组
		Array2<T::VALUE_TYPE> ValueArray(list.GetCount());
		for (int i = 0; i < (int)list.GetCount(); i++) {
			ValueArray[i] = list.GetAt(i);
		}

		std::qsort(ValueArray.get(), list.GetCount(), sizeof(T::VALUE_TYPE), comparePtrTo<T>);

		// 保存新的结果
		for (int i = 0; i < (int)list.GetCount(); i++) {
			list.SetAt(i, ValueArray[i]);
		}
	}
};


template<class T>
class Set : public Object2
{
public:
	Set()
	{
	}

	Set(Set<T>& cpy)
	{
		ASSERT(FALSE);
	}

	bool contains(T val)
	{
		Map2<T, int>::CPair* pair = _arr.Lookup(val);
		return pair != NULL;
	}

	void add(T val)
	{
		_arr.SetAt(val, 0);
	}

	void clear()
	{
		_arr.RemoveAll();
	}

	void remove(T val)
	{
		_arr.RemoveKey(val);
	}

	size_t GetCount()
	{
		return _arr.GetCount();
	}

	std::shared_ptr<Array2<T>> toArray()
	{
		std::shared_ptr<Array2<T>> arr(new Array2<T>(GetCount()));
		
		POSITION pos = _arr.GetStartPosition();
		int index = 0;
		while (pos)
		{
			(*arr)[index] = _arr.GetNextKey(pos);
			index++;
		}

		return arr;
	}

protected:
	CAtlMap<T, int> _arr;
	bool _sort;
};

template <class T>
class CSharedPtrTraits : public CDefaultElementTraits<T>
{
public:
	static ULONG Hash(_In_ const T& element) throw()
	{
		return (ULONG)(element.get());
	}
};

template<class KEY, class VALUE, class KTraits = CElementTraits< KEY >, class VTraits = CElementTraits< VALUE >>
class Map2 : public CAtlMap<KEY, VALUE, KTraits, VTraits>, public Object2
{
	//Map(const Map<KEY, VALUE>& src){}
public:
	typedef Map2<KEY, VALUE> _this_class;

	//Map(){}

	void put(KEY key, VALUE value)
	{
		SetAt(key, value);
	}

	bool remove(KEY key)
	{
		_this_class::CPair* pair = Lookup(key);
		if (pair)
		{
			RemoveKey(key);
		}
		return pair != NULL;
	}

	std::shared_ptr<Set<KEY>> keySet()
	{
		std::shared_ptr<Set<KEY>> sets(new Set<KEY>());

		POSITION pos = this->GetStartPosition();
		while (pos)
		{
			KEY key = this->GetNextKey(pos);
			sets->add(key);
		}

		return sets;
	}

	bool containsKey(KEY key)
	{
		_this_class::CPair* pair = Lookup(key);
		return pair != NULL;
	}
};

template<class KEY, class VALUE>
class TreeMap : public Map2<KEY, VALUE>
{

};

template<class KEY, class VALUE>
class HashMap : public Map2<KEY, VALUE>
{
};

//std::map<std::string, std::vector<int>> map;
//std::vector<int> v;
//v.push_back(1);
//v.push_back(2);
//v.push_back(3);
//map["one"] = v;
//
//for (const auto& kvp : map)
//{
//	std::cout << kvp.first << std::endl;
//
//	for (auto v : kvp.second)
//	{
//		std::cout << v << std::endl;
//	}
//}
//
//int arr[] = { 1, 2, 3, 4, 5 };
//for (int& e : arr)
//{
//	e = e*e;
//}