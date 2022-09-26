//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           Exception.H
//    @author         fanHong
//    @date           2017/11/3 13:14
//    @brief          异常操作
//
//*****************************************************

#pragma once

namespace CommBase
{
	//! 操作异常类
	class COMMBASE_API Exception
	{
	public:
		/**
		* 构造函数
		* @param [in] info		出错信息字符串
		* @param [in] detail	详细信息字符串（默认为NULL）
		*/
		Exception(LPCTSTR info, LPCTSTR detail = NULL);
		virtual ~Exception();

		/**
		* 取得出错信息
		*/
		virtual LPCTSTR	what(void);
		/**
		* 取得详细信息
		*/
		virtual LPCTSTR detail(void);
	protected:
		/** 出错信息 */
		CString	_info;
		/** 详细信息 */
		CString _detail;
	};

	/** 定义抛出异常方法 */
	/** 定义抛出异常方法 */
#ifndef RaiseError
#define RaiseError(info) throw toolkit::Exception (info)
#endif

#ifndef RaiseError2
#define RaiseError2(info,detail) throw toolkit::Exception (info,detail)
#endif

#ifndef ThrowError
#define ThrowError(info) throw toolkit::Exception (info)
#endif

#ifndef ThrowError2
#define ThrowError2(info,detail) throw toolkit::Exception (info,detail)
#endif

	/** 定义ASSERT 异常方法 */
#ifndef ASSERT_THROW
#define ASSERT_THROW(ExpVal,info) if (!(ExpVal)) throw toolkit::Exception (info)
#endif
}