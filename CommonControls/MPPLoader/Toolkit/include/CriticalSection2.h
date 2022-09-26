
#pragma once

// begin namespace
namespace BwProj{ namespace toolkit{

/** @addtogroup CriticalSectionHandle
* @{
*/



//! 定义互斥控件器
class COMMONCONTROLS_API CCriticalSectionHandle
{
public:
	CCriticalSectionHandle(void);
	~CCriticalSectionHandle(void);
	void Enter (void);
	void Leave (void);

protected:
	CRITICAL_SECTION	_Critical;	// 句柄
};

//! CCriticalSectionSync
class COMMONCONTROLS_API CCriticalSectionSync
{
public:
	CCriticalSectionSync (CCriticalSectionHandle& csc);
	~CCriticalSectionSync (void);

protected:
	CCriticalSectionHandle*	_csc;
};

//! 支持多线程的堆栈
template<class Data>
class CSecurityStackImpl
{
public:
	CSecurityStackImpl (void);
	void Append (Data val);
	bool RemoveTop (Data& val);
	bool RemoveLast (Data& val);
	/** 删除所有 */
	void RemoveAll (void);
	/** 取得最大峰值 */
	int GetPeakValue (void);
	/** 取得当前数量 */
	int GetCount (void);
	/** 获取值 */
	Data& GetAt (int nIndex);
protected:
	CCriticalSectionHandle	_CSC;
	CSimpleArray<Data>		_Array;
	int						_PeakValue;
};

//! 支持多线程的map
template<class KeyType, class ValueType>
class CSecurityMapImpl
{
public:

	CSecurityMapImpl (void);
	int FindKey (const KeyType& key);
	BOOL Add (const KeyType& key, const ValueType& val);
	KeyType& GetKeyAt (int nIndex);
	ValueType& GetValueAt (int nIndex);
	int GetSize ();
	bool RemoveLast (KeyType& key, ValueType& value);
	bool RemoveTop (KeyType& key, ValueType& value);
	/** 取得最大峰值 */
	int GetPeakValue (void);
protected:
	CCriticalSectionHandle			_CSC;
	CSimpleMap<KeyType, ValueType>	_Map;
	int								_PeakValue;
};

//! 支持多线程的 long
template<class T, T DefaultValue>
class CSecurityNumberImpl
{
public:
	typedef CSecurityNumberImpl<T,DefaultValue> ThisDefines;

	CSecurityNumberImpl (void);
	CSecurityNumberImpl (const T& d);
	ThisDefines& operator = (const T& d);
	ThisDefines& operator ++ (int d);
	ThisDefines& operator -- (int d);
	operator T();
protected:
	T						_value;
	CCriticalSectionHandle	_CSC;
};

#include "CriticalSection.hpp"

typedef CSecurityNumberImpl<bool, false> SecurityBool;
typedef CSecurityNumberImpl<long, 0> SecurityLong;


/** * @} */
// end namespace
}}
/** @} */