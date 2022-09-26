//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           Any.H
//    @author         fanHong
//    @date           2017/11/2 16:01
//    @brief          可变数据类型
//
//*****************************************************

#pragma once

#include "Guid.h"

namespace CommBase
{
	/*
	* 变型数据类型
	*/
	typedef enum
	{
		ANY_EMPTY = 0,		///< 空类型(没有赋值)
		ANY_BOOL = 1,		///< bool
		ANY_INT = 2,		///< int
		ANY_DOUBLE = 5,		///< double
		ANY_STRING = 6,		///< WCHAR*
		ANY_POINT = 7,		///< CGePoint3d
		ANY_GUID = 8,		///< GUID
		ANY_STREAM = 9,		///< BYTE*
		ANY_PTARRAY = 10,	///< osg::Vec3d Array
		ANY_INT64 = 11,		///< int64
		ANY_MATRIX = 12		///< Matrix
	}EAnyType;

	/**
	*  @class    可变数据类型类
	*
	*  @brief
	*/
	class COMMBASE_API CAny
	{
	public:
		CAny();
		~CAny();

		/**
		*  @brief    判断是否为空
		*
		*  @return   bool 没有进行赋值则为false
		*/
		bool IsEmpty() const;

		/**
		*  @brief    获取数据类型
		*
		*  @return   EAnyType 返回对应数据类型
		*/
		EAnyType GetType() const;

		/**
		*  @brief    绑定其它对象数据(特定场景使用,用于提高赋值性能)
		*
		*  @param   const CAny& other(此对象调用后数据会清空)
		*/
		void Attach(const CAny& other) const;

	public:
		/**
		*  @brief    获取字符串值(内部进行转换)
		*
		*  @param    int nDotNum 小数点位数
		*  @return   CString 返回字符串值
		*/
		CString GetStringValue(int nDotNum = 2) const;

		/**
		*  @brief    置字符串值转成指定类型值
		*
		*  @param    LPWSTR lpValue 字符串值输入值
		*  @param    EAnyType vType 转换的数据类型
		*/
		void SetStringValue(LPCWSTR lpValue, EAnyType vType);

		/**
		*  @brief    判断两个CAny是否相等
		*
		*  @param    const CAny& value
		*/
		bool operator == (const CAny& value) const;

		/**
		*  @brief    判断两个CAny是否不相等
		*
		*  @param    const CAny& value
		*/
		bool operator != (const CAny& value) const;

	public:
		/**
		*  @brief    拷贝构造函数
		*
		*  @param    const CAny & value
		*/
		CAny(const CAny& value);
		CAny& operator = (const CAny& value);

		/**
		*  @brief    bool值构造
		*
		*  @param    const bool & value
		*  @return
		*/
		CAny(const bool& value);
		CAny& operator = (const bool& value);
		operator bool() const;

		/**
		*  @brief    int值构造
		*
		*  @param    const int & value
		*/
		CAny(const int& value);
		CAny& operator = (const int& value);
		operator int() const;

		/**
		*  @brief    int64值构造
		*
		*  @param    const INT64 & value
		*/
		CAny(const INT64& value);
		CAny& operator = (const INT64& value);
		operator INT64() const;

		/**
		*  @brief    size_t值构造
		*
		*  @param    const size_t & value
		*/
		CAny(const size_t& value);
		CAny& operator = (const size_t& value);
		operator size_t() const;

		/**
		*  @brief    long值构造
		*
		*  @param    const long & value
		*/
		CAny(const long& value);
		CAny& operator = (const long& value);
		operator long() const;

		/**
		*  @brief    DWORD值构造
		*
		*  @param    const DWORD & value
		*/
		CAny(const DWORD& value);
		CAny& operator = (const DWORD& value);
		operator DWORD() const;


		/**
		*  @brief    double值构造
		*
		*  @param    const double & value
		*/
		CAny(const double& value);
		CAny& operator = (const double& value);
		operator double() const;

		/**
		*  @brief    字符串值构造
		*			 BuildString方法不明白的请不要使用
		*
		*  @param    const LPCTSTR & value
		*/
		CAny(const LPCWSTR& value);
		CAny& operator = (const LPCWSTR& value);
		operator LPCWSTR() const;
		LPWSTR BuildString(UINT nLen);

		/**
		*  @brief    字符串值构造
		*
		*  @param    const CString & value
		*/
		CAny(const CString& value);
		CAny& operator = (const CString& value);
		operator CString() const;

		/**
		*  @brief    GUID构造
		*
		*  @param    const GUID & value
		*/
		CAny(const GUID& value);
		CAny& operator = (const GUID& value);
		operator GUID() const;

		/**
		*  @brief    CGuid构造
		*
		*  @param    const CGuid & value
		*/
		CAny(const CGuid& value);
		CAny& operator = (const CGuid& value);
		operator CGuid() const;

		/**
		*  @brief    字节流构造(使用请注意)
		*			 构造/设置时传入的BYTE*会进行拷贝。
		*			 获取时BYTE*&为指向CAny内部内存，不需要自己释放，但需要注意CAny生命周期。
		*			 以免BYTE*&内存过早释放
		*
		*  @param    const BYTE * value
		*  @param    int len 数据长度
		*/
		CAny(const BYTE* value, int len);
		void SetStreamValue(const BYTE* value, int len);
		int GetStreamValue(BYTE*& value) const;
		BYTE* BuildStreamValue(int len);

		/**
		*  @brief    将一个常规的CVariantData变量转为内存流(使用请注意析构)
		*
		*  @param    const void * value
		*/
		CAny(const void* value);
		void SetVoidStreamValue(const void* value);
		int GetVoidStreamValue(void*&) const;

	protected:
		/**
		*  @brief    清理内存
		*
		*/
		void ClearTypeMemory();

	private:
		union vdValue
		{
			bool			valBool;	///< bool
			int				valInt;		///< int/BOOL/size_t
			double			valDouble;	///< double
			INT64			valInt64;	///< int64
			DWORD			valDword;	///< DWORD
			WCHAR			*valString;	///< WCHAR*
			GUID			*valGuid;	///< GUID
			BYTE			*valStream;	///< BYTE*
		}_vdValue;						///< Data值
		char _vdType;					///< Data类型
	};
}