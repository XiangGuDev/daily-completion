//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           DynEnum.H
//    @author         fanHong
//    @date           2017/11/3 15:32
//    @brief          动态枚举
//
//*****************************************************

#pragma once

namespace CommBase
{
	/**
	*  @class    枚举信息结构
	*
	*  @brief
	*/
	typedef struct tag_EnumTextInfo
	{
		int Value;			///< 枚举值
		CString Text;		///< 枚举字符串
		CString Show;		///< 枚举显示字符串
	}EnumTextInfo;

	/*
	* 枚举信息结构数组定义
	*/
	typedef std::vector<EnumTextInfo> EnumTextInfoArray;
	typedef EnumTextInfoArray::iterator EnumTextInfoArrayIter;

	/*
	* 枚举信息Map结构定义<枚举名，枚举信息结构数组>
	*/
	typedef std::map<CString, EnumTextInfoArray> EnumTextInfoMap;
	typedef EnumTextInfoMap::iterator EnumTextInfoMapIter;

	/**
	*  @class    动态枚举类
	*
	*  @brief
	*/
	class COMMBASE_API CDynEnum
	{
	public:
		CDynEnum();
		~CDynEnum();

	public:
		/**
		*  @brief    枚举值(转)枚举字符串
		*
		*  @param    const CString & strEnumName 枚举名
		*  @param    int nEnumValue 枚举值
		*  @return   CString 返回枚举对应的字符串
		*/
		static CString Enum2Text(const CString& strEnumName, int nEnumValue);

		/**
		*  @brief    枚举值(转)枚举显示字符串
		*
		*  @param    const CString & strEnumName 枚举名
		*  @param    int nEnumValue 枚举值
		*  @return   CString 返回枚举对应的显示字符串
		*/
		static CString Enum2Show(const CString& strEnumName, int nEnumValue);

		/**
		*  @brief    枚举字符串(转)枚举值
		*
		*  @param    const CString & strEnumName 枚举名
		*  @param    const CString & strEnumText 枚举字符串
		*  @return   int 返回枚举值
		*/
		static int Text2Enum(const CString& strEnumName, const CString& strEnumText);

		/**
		*  @brief    枚举显示字符串(转)枚举值
		*
		*  @param    const CString & strEnumName 枚举名
		*  @param    const CString & strEnumShow 枚举字符串
		*  @return   int 返回枚举值
		*/
		static int Show2Enum(const CString& strEnumName, const CString& strEnumShow);

		/**
		*  @brief    枚举显示字符串(转)枚举字符串
		*
		*  @param    const CString & strEnumName 枚举名
		*  @param    const CString & strEnumShow 枚举字符串
		*  @return   CString 返回枚举字符串
		*/
		static CString Show2Text(const CString& strEnumName, const CString& strEnumShow);

		/**
		*  @brief   枚举字符串(转)枚举显示字符串
		*
		*  @param    const CString & strEnumName 枚举名
		*  @param    const CString & strEnumText 枚举字符串
		*  @return   CString 返回枚举显示字符串
		*/
		static CString Text2Show(const CString& strEnumName, const CString& strEnumText);

		/**
		*  @brief    获取枚举信息数组(数组中的顺序与枚举定义不同)
		*
		*  @param    const CString & strEnumName 枚举名
		*  @return   EnumTextInfos 枚举信息数组
		*/
		static EnumTextInfoArray GetEnumInfoArray(const CString& strEnumName);

		/**
		*  @brief    加载枚举信息配置文件
		*
		*  @param    const CString & strXmlFile 动态枚举配置文件
		*  @return   bool 成功返回true
		*/
		static bool LoadEnumConfig(const CString& strXmlFile);

		/**
		*  @brief    获取出错信息
		*
		*  @return   CString 返回出错信息
		*/
		static CString GetLastError();
	};

#ifndef DISABLE_DYNAMIC_ENUM

	/*
	* 定义枚举转换宏
	*/
#define DECLARE_ENUM_CONVERSION(EnumName) \
	/*
	* 枚举与整型值转换
	*/ \
		static  __inline int EnumName##ToInt(EnumName e) { return int(e); } \
	static  __inline EnumName IntTo##EnumName(int e) { return EnumName((int)e); } \
	/*
	* 枚举值(转)枚举字符串
	*/ \
		static __inline CString EnumName##2Text(EnumName nEnumValue) \
	{\
		return CDynEnum::Enum2Text(L###EnumName, int(nEnumValue)); \
	}\
	/*
	* 枚举值(转)枚举显示字符串
	*/ \
		static  __inline CString EnumName##2Show(EnumName nEnumValue) \
	{\
		return CDynEnum::Enum2Show(L###EnumName, int(nEnumValue)); \
	}\
	/*
	* 枚举字符串(转)枚举值
	*/ \
		static  __inline EnumName Text2##EnumName(const CString& strEnumText) \
	{\
		return EnumName(CDynEnum::Text2Enum(L###EnumName, strEnumText)); \
	}\
	/*
	* 枚举显示字符串(转)枚举值
	*/ \
		static  __inline EnumName Show2##EnumName(const CString& strEnumShow) \
	{\
		return EnumName(CDynEnum::Show2Enum(L###EnumName, strEnumShow)); \
	}\
	/*
	* 枚举字符串(转)枚举显示字符串
	*/ \
		static  __inline CString EnumName##Text2Show(const CString& strEnumText) \
	{\
		return CDynEnum::Text2Show(L###EnumName,strEnumText); \
	}\
	/*
	* 枚举显示字符串(转)枚举字符串
	*/ \
		static  __inline CString EnumName##Show2Text(const CString& strEnumShow) \
	{\
		return CDynEnum::Show2Text(L###EnumName, strEnumShow); \
	}\
	/*
	* 获取枚举信息数组
	*/ \
		static  __inline EnumTextInfoArray Get##EnumName##InfoArray() \
	{\
		return CDynEnum::GetEnumInfoArray(L###EnumName); \
	}
#else

#define DECLARE_ENUM_CONVERSION(EnumName)

#endif
}