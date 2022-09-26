#pragma once

#include <exception>
// begin namespace
namespace BwProj {

/** @addtogroup exception
* @{
*/

//! 操作异常类
class COMMONTOOLS_API Exception
{
public:
	/**
	* 构造函数
	* @param [in] info		出错信息字符串
	* @param [in] detail	详细信息字符串（默认为NULL）
	*/
	Exception (LPCTSTR info, LPCTSTR detail = NULL);
	Exception(const Exception& e);
	virtual ~Exception ();

	/**
	* 取得出错信息
	*/
	virtual LPCTSTR	what (void) const;
	/**
	* 取得详细信息
	*/
	virtual LPCTSTR detail (void) const;
protected:
	/** 出错信息 */
	CString	_info;
	/** 详细信息 */
	CString _detail;
};

/** 定义抛出异常方法 */
#define RaiseError(info) throw BwProj::Exception (info)
#define RaiseError2(info,detail) throw BwProj::Exception (info,detail)
#define ThrowError(info) throw BwProj::Exception (info)
#define ThrowError2(info,detail) throw BwProj::Exception (info,detail)

/** 定义ASSERT 异常方法 */
#define ASSERT_THROW(ExpVal,info) if (!(ExpVal)) throw BwProj::Exception (info)

/** * @} */
// end namespace
}
