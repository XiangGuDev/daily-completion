#pragma once
#include <shellapi.h>

// begin namespace
namespace BwProj{ namespace toolkit{


/** @addtogroup CadreLibToolkit
* @{
*/

//! 字符串系统
class COMMONTOOLS_API CStringToolkit
{
public:
	/**
	* 去除左右两边的空行与回车符
	* @param [in] sString	字符串
	* @return	返回去除左右两边的空行和回车符后的字符串
	*/
	static CString Trim (CString sString);

	/**
	* 统计字符串中有多少个以 sParChr 分割的单词
	* @param [in] sString	字符串
	* @param [in] sParChr	分割字符
	* @return	返回字符串中有多少个以 sParChr 分割的单词的个数
	*/
	static int CountWord (CString sString, TCHAR sParChr = _T(' '));

	/**
	*	读取字符串中指定的单词
	* @param [in] sString	字符串
	* @param [in] WordIdx	起始位置
	* @param [in] sParChr	分割字符（默认为空）
	* @return 返回读取的字符串
	*/
	static CString ReadWord (CString sString, int WordIdx, TCHAR sParChr = _T(' '));

	/**
	* 查找指定的单词
	* @param [in] SourStr	源字符串
	* @param [in] Word		单词
	* @param [in] RigorFind	严格匹配（默认为true）
	* - true 按照输入， false 变成大写字母后严格查找
	* @param [in] StartPos	起始位置（默认为1）
	* @param [in] sParChr	分割字符（默认为空）
	* @return 返回指定单词的位置
	*/
	static int FindWord (CString SourStr, CString Word, bool RigorFind = true,
						int StartPos = 1, TCHAR sParChr = _T(' '));

	/**
	* 从最后指定位置开始倒数查找字符
	* @param [in] str	源字符串
	* @param [in] chr	待查找字符
	* @param [in] pos	查找起始位置（默认为0，即表示从最后开始）
	* @return 返回指定单词的位置（pos<0 时返回-1）
	*/
	static int FindLastChar (CString str, TCHAR chr, int pos = 0);

	/**
	* 删除浮点数字符串数据的 0
	* @param [in] NumStr	浮点数字符串
	* @param [in] keepDot	如果为真则保留最后一个.0，如12.00换成12.0
	* @return 返回删除数据0的字符串
	* - 例：12.34000 返回为 12.34；12.00 返回为 12
	*/
	static CString TrimNumberString (CString NumStr, bool keepDot = false);

	/**
	* 长整型转成 CString
	* 
	* long范围：-2,147,483,648 到 2,147,483,647 
	* @param [in] value	长整型
	* @return 返回转换成的字符串
	*/
	static CString IntToStr (long value);

	/**
	* 字符串转换成长整型
	*
	* long范围：-2,147,483,648 到 2,147,483,647 
	* @param [in] str	字符串
	* @return 返回转换成的长整型
	*/
	static long StrToInt (CString str);

	/**
	* 字符串转换成浮点型
	* @param [in] str	字符串
	* @return 返回转换成的浮点型
	*/
	static double StrToDouble (CString str);

	/**
	* 获取字符串汉语拼音的首字母
	* @param [in] HanZiText	字符串
	* @return 返回获取的汉语拼音首字母
	*/
	static CString GetPingYinFirstLetter (LPCTSTR HanZiText);

	/**
	* 比较两个版本号大小
	* @param [in] version1	版本号1
	* @param [in] version2	版本号2
	* @return 返回表示版本号大小的数字
	* - 1 版本号1 > 版本号2， 0 版本号1 = 版本号2， -1 版本号1 < 版本号2
	*/
	static int CompareVersion (CString version1, CString version2);

	/**
	* 判断是否为数字(空文本不是数字)
	* @param [in] strText
	* @return
	*/
	static bool IsNumber(CString strText);
};

/** * @} */
}}
