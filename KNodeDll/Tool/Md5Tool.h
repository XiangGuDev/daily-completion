//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           Md5Tool.H
//    @author         fanHong
//    @date           2017/11/2 17:17
//    @brief          MD5包装
//
//*****************************************************

#pragma once

namespace CommBase
{
	/**
	*  @class    MD5包装类
	*
	*  @brief
	*/
	class COMMBASE_API CMd5Tool
	{
	public:
		/**
		*  @brief    获取文件的Md5码;
		*
		*  @param    const CString & strFile 输入文件;
		*  @return   CString 文件的MD5码;
		*/
		static CString GetFileMd5(const CString& strFile);

		/**
		*  @brief    将字符串转为MD5
		*
		*  @param    const CString & strText
		*  @return   CString
		*/
		static CString StringToMd5(const CString& strText);
	};
}