#pragma once

// begin namespace
namespace BwProj{ namespace toolkit {

/** @addtogroup CriticalSectionHandle
* @{
*/

//! 单实例互斥体对象
class COnlyOneInstance
{
public:
	COnlyOneInstance (LPCTSTR MutexName);
	~COnlyOneInstance (void);
	/**
	* 返回设置单实例是否成功
	* @return 返回设置单实例是否成功
	* - true 成功， false 失败
	*/
	bool GetSuccess (void);
protected:
	/** 单实例互斥体句柄 */
	HANDLE _Mutex;
};


/** @} */
}}