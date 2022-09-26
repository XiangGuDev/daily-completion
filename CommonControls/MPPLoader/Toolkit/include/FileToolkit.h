

#pragma once

#include <ShellAPI.h>

// begin namespace
namespace BwProj{ namespace toolkit{

/** @addtogroup FileToolkit
* @{
*/

//! 文件系统
class COMMONTOOLS_API CFileToolkit
{
public:
	// 选择目录
	//CString SelectDirectory (char *Title, HWND hWnd = NULL);
	/**
	* 新建目录 （支持网络、多级）
	* @param [in] Directory		目录名称
	* @return	返回新建目录是否成功
	* - true 新建成功， false 新建失败
	*/
	static bool CreateDirectory (CString Directory);

	/**
	* 测试文件是否存在
	* @param [in] Filename		文件名称
	* @return	返回文件是否存在
	* - true 文件存在， false 文件不存在
	*/
	static bool FileExist (CString Filename);

	/**
	* 测试目录是否存在
	* 
	* 不支持网络路径
	* @param [in] Directory		目录名称
	* @return	返回目录是否存在
	* - true 目录存在， false 目录不存在
	*/
	static bool DirectoryExist (CString Directory);
	
	/**
	* 获取特定目录下文件以及目录信息列表
	* @param [in] Directory		目录名称
	* @param [out] arr			目录以及目录信息列表
	* @return	返回是否成功获取特定目录下文件以及目录信息列表
	* - true 获取成功， false 获取失败
	*/
	static bool ReadDirectory (CString Directory, CSimpleArray<WIN32_FIND_DATA >& arr);

	/**
	* 获取特定目录下文件以及目录信息列表
	* @param [in] Directory		目录名称
	* @param [out] arr			目录以及目录信息列表
	* @param [in] Recursion		是否递归（默认为否）
	* @return	返回是否成功获取特定目录下文件以及目录信息列表
	* - true 获取成功， false 获取失败
	*/
	static bool ReadDirectory (CString Directory, CSimpleArray<CString>& arr, bool Recursion = false);
	
	/**
	* 取得当前目录
	* @return	返回当前目录字符串
	*/
	static CString GetCurrentDirectory ();

	/**
	* 格式化目录
	*
	* 函数无法准确区分文件和目录，因此传入的文件也将被作为目录处理
	* 如传入"C:\\Windows\\explorer.exe"会返回"C:\\Windows\\explorer.exe\\"
	* 相对路径只支持一个，类似“..\..\”只会处理第一个“..\”
	* @param [in] Directory		目录名称
	* @return	返回格式化后的目录字符串,结尾包含'\'
	* - 格式化后的目录如：C:\\hello\\
	*/
	static CString FormatDirectory (CString Directory);
	
	/**
	* 格式化文件名(如果Filename不带扩展名则将ExtendName作为返回值的扩展名，否则返回值等于Filename)
	* @param [in] Filename		文件名称
	* @param [in] ExtendName	扩展名称（默认为空）（Filename包含扩展名时该参数将被忽略.）
	* @return	返回格式化后的文件名称
	* - 假如Filename为C:\\hello, ExtendName为txt，则返回C:\\hello.txt
	*/
	static CString FormatFilename (CString Filename, CString ExtendName = _T(""));
	
	/**
	* 取得文件逻辑名
	* @param [in] Filename		文件名称
	* @return	返回文件逻辑名
	* - 假如Filename为C:\\hello.txt, 则返回hello.txt
	*/
	static CString GetFileLogicName (CString Filename);

	/**
	* 取得文件扩展名（包括“.”）
	* @param [in] Filename		文件名称
	* @return	返回文件扩展名
	* - 假如Filename为C:\\hello.txt, 则返回.txt
	*/
	static CString GetFileExtendName (CString Filename);
	
	/**
	* 取得文件基本名
	* @param [in] Filename		文件名称
	* @return	返回文件基本名
	* - 假如Filename为C:\\hello.txt, 则返回hello
	*/
	static CString GetFileBaseName (CString Filename);

	/**
	* 取得文件所在目录
	* @param [in] Filename		文件名称
	* @return	返回文件所在目录
	* - 假如Filename为C:\\hello\\hello.txt, 则返回C:\\hello
	*/
	static CString GetFileDirectory (CString Filename);

	/**
	* 取得文件所在驱动
	* 
	* 函数不会严格检查路径合法性
	* 前两位是"\\"就认为是网络路径：返回\\+下一个\之前内容
	* 第二位是‘:’就认为是磁盘路径：返回前两个字符
	* 第一位是‘\’,认为是从当前跟目录起始的路径，返回当前目录的前两个字符
	* 否则，返回空字符串
	* @param [in] Path		文件路径
	* @return	返回文件所在驱动
	*/
	static CString GetDrive (CString Path);
	
	/**
	* 设置文件扩展名
	* Filename的BaseName不允许为空
	* @param [in] Filename		文件名称
	* @param [in] ExtendName	扩展名称（默认为空）
	* @return	返回设置后的文件名
	*/
	static CString SetFileExtendName (CString Filename, CString ExtendName = _T(""));
	
	/**
	* 设置文件逻辑名
	* @param [in] Filename		文件名称
	* @param [in] LogicName		逻辑名称
	* @return	返回设置后的文件名
	*/
	static CString SetFileLogicName (CString Filename, CString LogicName);
	
	/**
	* 设置文件基本名
	* @param [in] Filename		文件名称
	* @param [in] BaseName		文件基本名(不允许为空)
	* @return	返回设置后的文件名
	*/
	static CString SetFileBaseName (CString Filename, CString BaseName);
	
	/**
	* 设置文件所在目录
	*
	* 函数不会对传入目录的合法性做校验，使用者需要自己确保参数有效
	* @param [in] Filename		文件名称
	* @param [in] Directory		目标目录,
	* @return	返回设置后的文件名
	*/
	static CString SetFileDirectory (CString Filename, CString Directory);

	/**
	* 删除文件或文件夹到回收站
	* @param [in] strFile		文件路径
	* @param [in] fFlag			状态值（默认为允许撤销）
	* @return	返回是否成功删除文件或文件夹到回收站
	* - true 文件不存在， false 删除失败
	*/
	static bool DeleteToRecycle (CString strFile, FILEOP_FLAGS fFlag = FOF_ALLOWUNDO);

	/**
	* 根据文件扩展名取得对应的系统图标
	* - 返回的hIcon句柄必须自己删除，否则会有资源泄露
	* @param [in] strExt		文件路径
	* @param [out] hIcon		图标对象
	* @param [out] nIndex		图标索引
	* @param [in] attrib		文件属性（默认为FILE_ATTRIBUTE_NORMAL）
	* @return	返回是否成功取得对应的系统图标
	* - true 获取成功， false 获取失败
	*/
	static bool GetIconByExtName (CString strExt, HICON& hIcon, int& nIndex, DWORD attrib = FILE_ATTRIBUTE_NORMAL);

	/**
	* 检查文件名是否为标准文件名
	* Windows文件命名规则：（摘自MSDN：ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.chs/fileio/fs/naming_a_file.htm）
	*   - 使用点(.)分割基本文件名与扩展名
	*   - 使用反斜杠(\)分割路径元素，如目录与目录之间、目录与文件名之间
	*   - 反斜杠(\)需要作为分区名的一部分，如 "C:\<path>\<file>" 中的"C:\"，反斜杠(\)不能用在实际的目录和文件名中
	*   - 可以使用当前代码页(包括Unicode)的任意字符，除了值为0-31和以下保留字符：
	*			< > : " / \ | ? *
	*   - 在路径中使用一个点号(.)作为路径元素时代表当前目录
	*   - 在路径中使用两个连续点号(..)作为路径元素时代表父目录
	*   - 不要使用保留设备名作为文件名：
	*		CON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, and LPT9
	*		同时也避免使用这些名称带有扩展名，如 NUL.tx7
	*   - 不要假定大小写敏感。例如 OSCAR, Oscar, 和 oscar认为是相同的
	*   - 不要使用空白字符和点号作为文件或目录的结尾，但可以以点号开头
	*
	* -不包含无效字符的文件名，不能包含路径
	* @param [in] strFile		文件名称
	* @param [out] pChar		返回检查不符合的字符是什么
	* @return	返回文件名是否为标准文件名
	* - true 标准，false 不标准
	*/
	static bool CheckFileNameValidate (CString strFile, TCHAR* pChar);


	/**根据文件句柄取得文件路径 */
	static BOOL GetFilePathFromHandle (HANDLE hFile, LPWSTR lpszPath, UINT cchMax);

	/**获取本地应用数据目录，例如：
		XP: C:\Document and Settings\Default Users\Application Data 
		WIN7: C:\Users\Default\AppData\Roaming
	*/
	static CString GetLocalAppDataPath();

	/**在本地应用数据目录下创建目录，并返回路径*/
	static CString GetLocalAppDataPath(LPCTSTR appName);

private:

	/** 
	* 分解出文件名中的所有目录名；用于重新组合文件名或目录名 
	* @param [in] Filename		文件名称
	* @param [out] arr			文件名或目录名数组
	* @return	返回文件名或目录名的数量
	*/
	static int DecDir (CString Filename, CAtlArray<CString> *arr);

	/** 根据文件句柄取得卷名 */
	static BOOL GetVolumeNameByHandle(HANDLE hFile, LPWSTR szVolumeName, UINT cchMax);
};


/** * @} */
}}
/** @} */