
#pragma once
#include <math.h>

// begin namespace
namespace BwProj{ namespace toolkit{

/** @addtogroup DataConvert
* @{
*/

//! 数据转换类
class COMMONTOOLS_API DataConvert
{
public:
	static wchar_t* UTF8ToUnicode(const char* str);

	/** 
	* 比较两个浮点数是否相等
	* @param [in] var1	浮点数1
	* @param [in] var2	浮点数2
	* @param [in] dot	小数位数，默认为-1，表示不四舍五入（差不大于0.000001即认为相等）
	* @return	返回两个浮点数是否相等
	* - true 相等，false 不相等
	*/
	static bool FloatEqual (double var1, double var2, int dot = -1);

	/** 
	* 浮点数四舍五入
	* @param [in] num		要格式化的数值
	* @param [in] dot		小数位数，为 -1 表示是不四舍五入
	* @return	返回格式化后的数值
	*/
	static double FloatRound (double num, int dot);

	/** 
	* __int64 转换成字符串
	* -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 
	* @param [in] val		要格式化的数值
	* @param [in] buff		输出缓存
	* @return 返回__int64格式化后的字符串
	*/
	static LPCTSTR I642Text (__int64 val, TCHAR* buff);

	/** 
	* __int64 转换成字符串
	* -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 
	* @param [in] val		要格式化的数值
	* @return	返回__int64格式化后的字符串
	*/
	static CString I642Text (__int64 val);

	/** 
	* bool 转换成字符串
	* @param [in] val		要格式化的数值
	* @return	返回bool格式化后的字符串
	*/
	static LPCTSTR Bool2Text (BOOL val);

	/** 
	* 字符串 转换成bool
	* val转为大写情况下，val是“TRUE”或“T”返回true，否则返回false
	* @param [in] val		要格式化的数值
	* @return	返回字符串格式化后的bool
	*/
	static bool Text2Bool (LPCTSTR val);

	/**
	* 比较两个字符串是否相等 (不区分大小写)
	* @param [in] str1		字符串1
	* @param [in] str2		字符串2
	* @return	返回字符串是否相等
	* - true 相等， false 不相等
	*/
	static bool StringEqual (LPCTSTR str1, LPCTSTR str2);

	/**
	* 浮点数四舍五入成字符串
	* @param [in] num		要格式化的数值
	* @param [in] dot		小数位数，为 -1 表示是不四舍五入（实际由于C++库的原因只输出6位小数）
	* @param [in] buffer	输出缓存
	* @param [in] FullZero	如果小数位不够，是否用 0 填充
	* @return	返回浮点数格式化后的字符串
	*/
	static LPCTSTR Float2Text(double num, TCHAR* buffer, int dot = -1, bool FullZero = false);

	/**
	* 浮点数四舍五入成字符串
	* @param [in] num		要格式化的数值
	* @param [in] dot		小数位数，为 -1 表示是不四舍五入
	* @param [in] FullZero	如果小数位不够，是否用 0 填充
	* @return	返回浮点数格式化后的字符串
	*/
	static CString Float2Text(double num, int dot = -1, bool FullZero = false);

	/**
	* long 转换成字符串
	* long范围：-2,147,483,648 到 2,147,483,647
	* @param [in] val		要格式化的数值
	* @param [in] buff		输出缓存
	* @return	返回long格式化后的字符串
	*/
	static LPCTSTR Long2Text(long val, TCHAR* buff);

	/**
	* long 转换成字符串
	* long范围：-2,147,483,648 到 2,147,483,647
	* @param [in] val		要格式化的数值
	* @return	返回long格式化后的字符串
	*/
	static CString Long2Text(long val);
};

/** * @} */
// end namespace
}}
/** @} */