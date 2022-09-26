//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           EnumTool.H
//    @author         fanHong
//    @date           2017/11/3 14:20
//    @brief          枚举-文本转换接口
//
//*****************************************************

#pragma once

namespace CommBase
{
	//-------------------------------------------------------------------
	// 实现枚举值与字符串的转换
	//-------------------------------------------------------------------


	/****************************************************************************************
	*								** 使用例子 **
	*
	* 1、×定义枚举值：
	*
	*	enum FileCreateTypeEnum
	*	{
	*		fctStdProject = 1,
	*		fctUserProject,
	*		fctGroupwareLib,
	*	};
	*
	*	DECLARE_ENUMTYPENAME_CONVERSION (FileCreateTypeEnum);
	*
	* 2、×实现枚举与字符串的使用：
	*
	*	BEGIN_ENUMTYPENAME_CONVERSION (FileCreateTypeEnum)
	*	{
	*		{ fctStdProject, _T("标准工程") },		{ fctUserProject, _T("工程模板") },
	*		{ fctGroupwareLib, _T("线路组件库") },	{ (FileCreateTypeEnum) NULL, NULL }
	*	}
	*	END_ENUMTYPENAME_CONVERSION (FileCreateTypeEnum);
	*
	* 3、×使用转换函数：
	*
	*	// 枚举值转字符串
	*	ZJString EnumText = FileCreateTypeEnumToText (fctStdProject);
	*
	*	// 字符串转枚举值
	*	FileCreateTypeEnum EnumValue = TextToFileCreateTypeEnum (_T("标准工程"));
	*
	*	// 取得枚举值定义
	*	CEnumTextDefines EnumNameList;
	*	GetFileCreateTypeEnumDefines (EnumNameList);
	*
	****************************************************************************************/


	// 定义枚举值与字符串的转换函数声明
	//   注:如果从字符串转换成枚举失败,则返回0,否则返回具体的枚举值
	//		如果从枚举转换成字符串失败,则返回NULL,否则返回具体的名称
	//
	// 例如: 枚举类型 ProjectTypeEnum，可生成以下三函数
	//
	//		1. LPCTSTR ProjectTypeEnumToText (ProjectTypeEnum val);
	//		2. ProjectTypeEnum TextToProjectTypeEnum (LPCTSTR txt);
	//		3. int GetProjectTypeEnumDefines (CEnumTextDefines& arr);

	struct ENUM_YPET_EXT
	{
		int value;
		LPCTSTR text;
		int data;

		bool operator()(CString &strValue)
		{
			return strValue.Compare(text) == 0;
		}
	};
	typedef std::vector<ENUM_YPET_EXT> CEnumTypeTextArray;

	#define DECL_DYN_ENUM(ENUMNAME) \
	LPCTSTR ENUMNAME##ToText (ENUMNAME val);\
	ENUMNAME TextTo##ENUMNAME (LPCTSTR txt,int def=0);\
	size_t Get##ENUMNAME##Defines (CEnumTypeTextArray& arr);



	/** 定义定额枚举值值与字符串的转换函数 */
	#define BEGIN_DYN_ENUM(ENUMNAME) \
	ENUM_YPET_EXT ENUMNAME##TextList[] =

	#define END_DYN_ENUM(ENUMNAME) \
	;\
	LPCTSTR ENUMNAME##ToText (ENUMNAME val)\
	{\
		for (int i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i ++)\
		{\
			if (ENUMNAME##TextList[i].value == val)\
				return ENUMNAME##TextList[i].text;\
		}\
		return _T("");\
	}\
	ENUMNAME TextTo##ENUMNAME (LPCTSTR txt,int def)\
	{\
		if (txt == NULL || txt[0] == 0) return (ENUMNAME) def;\
		int i = 0;\
		for (i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i ++)\
		{\
			if (!_tcscmp (ENUMNAME##TextList[i].text, txt))\
				return (ENUMNAME) ENUMNAME##TextList[i].value;\
		}\
		return (ENUMNAME) ENUMNAME##TextList[i].value;\
	}\
	size_t Get##ENUMNAME##Defines (CEnumTypeTextArray& arr)\
	{\
		arr.clear ();\
		for (int i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i ++)\
		{\
			arr.push_back (ENUMNAME##TextList[i]);\
		}\
		return arr.size ();\
	}
}