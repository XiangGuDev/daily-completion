//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           CriticalSection.H
//    @author         fanHong
//    @date           2017/11/3 11:17
//    @brief          互斥控件器
//
//*****************************************************

#pragma once
#include <atlsimpcoll.h>

namespace CommBase
{
	//! 定义互斥控件器
	class COMMBASE_API CCriticalSectionHandle
	{
	public:
		CCriticalSectionHandle(void);
		~CCriticalSectionHandle(void);
		void Enter(void);
		void Leave(void);

	protected:
		CRITICAL_SECTION	_Critical;	// 句柄
	};

	//! CCriticalSectionSync
	class COMMBASE_API CCriticalSectionSync
	{
	public:
		CCriticalSectionSync(CCriticalSectionHandle& csc);
		~CCriticalSectionSync(void);

	protected:
		CCriticalSectionHandle*	_csc;
	};

	//! 支持多线程的 long
	template<class T, T DefaultValue>
	class CSecurityNumberImpl
	{
	public:
		typedef CSecurityNumberImpl<T, DefaultValue> ThisDefines;

		CSecurityNumberImpl(void);
		CSecurityNumberImpl(const T& d);
		ThisDefines& operator = (const T& d);
		ThisDefines& operator ++ (int d);
		ThisDefines& operator -- (int d);
		operator T();
	protected:
		T						_value;
		CCriticalSectionHandle	_CSC;
	};


	template<class T, T DefaultValue>
	CSecurityNumberImpl<T, DefaultValue>::CSecurityNumberImpl(void)
	{
		_value = DefaultValue;
	}

	template<class T, T DefaultValue>
	CSecurityNumberImpl<T, DefaultValue>::CSecurityNumberImpl(const T& d)
	{
		_value = d;
	}

	template<class T, T DefaultValue>
	typename CSecurityNumberImpl<T, DefaultValue>::ThisDefines& CSecurityNumberImpl<T, DefaultValue>::operator = (const T& d)
	{
		CCriticalSectionSync cs(_CSC);

		_value = d;
		return *this;
	}

	template<class T, T DefaultValue>
	typename CSecurityNumberImpl<T, DefaultValue>::ThisDefines& CSecurityNumberImpl<T, DefaultValue>::operator ++ (int d)
	{
		CCriticalSectionSync cs(_CSC);

		_value++;
		return *this;
	}

	template<class T, T DefaultValue>
	typename CSecurityNumberImpl<T, DefaultValue>::ThisDefines& CSecurityNumberImpl<T, DefaultValue>::operator -- (int d)
	{
		CCriticalSectionSync cs(_CSC);

		_value--;
		return *this;
	}

	template<class T, T DefaultValue>
	CSecurityNumberImpl<T, DefaultValue>::operator T()
	{
		CCriticalSectionSync cs(_CSC);

		return _value;
	}

	//! 支持多线程的map
	template<class KeyType, class ValueType>
	class CSecurityMapImpl
	{
	public:

		CSecurityMapImpl(void);
		int FindKey(const KeyType& key);
		BOOL Add(const KeyType& key, const ValueType& val);
		KeyType& GetKeyAt(int nIndex);
		ValueType& GetValueAt(int nIndex);
		int GetSize();
		bool RemoveLast(KeyType& key, ValueType& value);
		bool RemoveTop(KeyType& key, ValueType& value);
		/** 取得最大峰值 */
		int GetPeakValue(void);
	protected:
		CCriticalSectionHandle			_CSC;
		CSimpleMap<KeyType, ValueType>	_Map;
		int								_PeakValue;
	};

	template<class KeyType, class ValueType>
	CSecurityMapImpl<KeyType, ValueType>::CSecurityMapImpl(void)
	{
		_PeakValue = 0;
	}
	template<class KeyType, class ValueType>
	int CSecurityMapImpl<KeyType, ValueType>::FindKey(const KeyType& key)
	{
		CCriticalSectionSync cs(_CSC);

		return _Map.FindKey(key);
	}
	template<class KeyType, class ValueType>
	BOOL CSecurityMapImpl<KeyType, ValueType>::Add(const KeyType& key, const ValueType& val)
	{
		CCriticalSectionSync cs(_CSC);

		_PeakValue++;
		return _Map.Add(key, val);
	}
	template<class KeyType, class ValueType>
	KeyType& CSecurityMapImpl<KeyType, ValueType>::GetKeyAt(int nIndex)
	{
		CCriticalSectionSync cs(_CSC);

		return _Map.GetKeyAt(nIndex);
	}
	template<class KeyType, class ValueType>
	ValueType& CSecurityMapImpl<KeyType, ValueType>::GetValueAt(int nIndex)
	{
		CCriticalSectionSync cs(_CSC);

		return _Map.GetValueAt(nIndex);
	}
	template<class KeyType, class ValueType>
	int CSecurityMapImpl<KeyType, ValueType>::GetSize()
	{
		CCriticalSectionSync cs(_CSC);

		return _Map.GetSize();
	}
	template<class KeyType, class ValueType>
	bool CSecurityMapImpl<KeyType, ValueType>::RemoveLast(KeyType& key, ValueType& value)
	{
		CCriticalSectionSync cs(_CSC);

		if (_Map.GetSize() == 0) return false;

		key = _Map.GetKeyAt(_Map.GetSize() - 1);
		value = _Map.GetValueAt(_Map.GetSize() - 1);
		return _Map.Remove(key) == TRUE ? true : false;
	}

	template<class KeyType, class ValueType>
	bool CSecurityMapImpl<KeyType, ValueType>::RemoveTop(KeyType& key, ValueType& value)
	{
		CCriticalSectionSync cs(_CSC);

		if (_Map.GetSize() == 0) return false;

		key = _Map.GetKeyAt(0);
		value = _Map.GetValueAt(0);
		return _Map.Remove(key) == TRUE ? true : false;
	}

	template<class KeyType, class ValueType>
	int CSecurityMapImpl<KeyType, ValueType>::GetPeakValue(void)
	{
		return _PeakValue;
	}

	typedef CSecurityNumberImpl<bool, false> SecurityBool;
	typedef CSecurityNumberImpl<long, 0> SecurityLong;
}