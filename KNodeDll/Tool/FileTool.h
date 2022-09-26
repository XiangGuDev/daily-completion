//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           FileTool.H
//    @author         fanHong
//    @date           2017/11/2 18:10
//    @brief          文件工具集
//
//*****************************************************

#pragma once

#include "Type/TypeDefine.h"

namespace CommBase
{
	//! 文件系统
	class COMMBASE_API CFileTool
	{
	public:
		/**
		* 新建目录 （支持多级）
		* @param [in] Directory		目录名称
		* @return	返回新建目录是否成功
		* - true 新建成功， false 新建失败
		*/
		static bool CreateDirectory(CString Directory);

		/**
		* 测试文件是否存在
		* @param [in] Filename		文件名称
		* @return	返回文件是否存在
		* - true 文件存在， false 文件不存在
		*/
		static bool FileExist(CString Filename);

		/**
		* 测试目录是否存在
		*
		* 不支持网络路径
		* @param [in] Directory		目录名称
		* @return	返回目录是否存在
		* - true 目录存在， false 目录不存在
		*/
		static bool DirectoryExist(CString Directory);

		/**
		* 获取特定目录下文件以及目录信息列表
		* @param [in] Directory		目录名称
		* @param [out] arr			目录以及目录信息列表
		* @param [in] Recursion		是否递归（默认为否）
		* @param [fileExt		    读取的扩展名
		* @return	返回是否成功获取特定目录下文件以及目录信息列表
		* - true 获取成功， false 获取失败
		*/
		static bool ReadDirectory(CString Directory , StringArray& arr,
			bool Recursion = false, CString fileExt = L"*.*");

		/**
		* 获取特定目录下的文件夹
		* @param [in] Directory		目录名称
		* @param [out] arr			目录以及目录信息列表
		* @param [in] Recursion		是否递归（默认为否）
		* @param [fileExt		    读取的扩展名
		* @return	返回是否成功获取特定目录下文件以及目录信息列表
		* - true 获取成功， false 获取失败
		*/
		static bool ReadDirectoryDir(CString Directory, StringArray& arr,
								  bool Recursion = false);

		/**
		* 取得当前目录
		* @return	返回当前目录字符串
		*/
		static CString GetCurrentDirectory();

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
		static CString FormatDirectory(CString Directory);

		/**
		* 格式化文件名(如果Filename不带扩展名则将ExtendName作为返回值的扩展名，否则返回值等于Filename)
		* @param [in] Filename		文件名称
		* @param [in] ExtendName	扩展名称（默认为空）（Filename包含扩展名时该参数将被忽略.）
		* @return	返回格式化后的文件名称
		* - 假如Filename为C:\\hello, ExtendName为txt，则返回C:\\hello.txt
		*/
		static CString FormatFilename(CString Filename , CString ExtendName = _T(""));

		/**
		* 取得文件逻辑名
		* @param [in] Filename		文件名称
		* @return	返回文件逻辑名
		* - 假如Filename为C:\\hello.txt, 则返回hello.txt
		*/
		static CString GetFileLogicName(CString Filename);

		/**
		* 取得文件扩展名（包括“.”）
		* @param [in] Filename		文件名称
		* @return	返回文件扩展名
		* - 假如Filename为C:\\hello.txt, 则返回.txt
		*/
		static CString GetFileExtendName(CString Filename);

		/**
		* 取得文件基本名
		* @param [in] Filename		文件名称
		* @return	返回文件基本名
		* - 假如Filename为C:\\hello.txt, 则返回hello
		*/
		static CString GetFileBaseName(CString Filename);

		/**
		* 取得文件所在目录
		* @param [in] Filename		文件名称
		* @return	返回文件所在目录
		* - 假如Filename为C:\\hello\\hello.txt, 则返回C:\\hello
		*/
		static CString GetFileDirectory(CString Filename);

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
		static CString GetDrive(CString Path);

		/**
		* 设置文件扩展名
		* Filename的BaseName不允许为空
		* @param [in] Filename		文件名称
		* @param [in] ExtendName	扩展名称（默认为空）
		* @return	返回设置后的文件名
		*/
		static CString SetFileExtendName(CString Filename , CString ExtendName = _T(""));

		/**
		* 设置文件逻辑名
		* @param [in] Filename		文件名称
		* @param [in] LogicName		逻辑名称
		* @return	返回设置后的文件名
		*/
		static CString SetFileLogicName(CString Filename , CString LogicName);

		/**
		* 设置文件基本名
		* @param [in] Filename		文件名称
		* @param [in] BaseName		文件基本名(不允许为空)
		* @return	返回设置后的文件名
		*/
		static CString SetFileBaseName(CString Filename , CString BaseName);

		/**
		* 设置文件所在目录
		*
		* 函数不会对传入目录的合法性做校验，使用者需要自己确保参数有效
		* @param [in] Filename		文件名称
		* @param [in] Directory		目标目录,
		* @return	返回设置后的文件名
		*/
		static CString SetFileDirectory(CString Filename , CString Directory);

		/**
		* 删除文件或文件夹
		* @param [in] strFile		文件路径
		* @param [in] fFlag			状态值（默认为针窗口）
		* @return	返回是否成功删除文件或文件夹到回收站
		*/
		static bool DeleteDirectory(CString strDir);

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
		static bool GetIconByExtName(CString strExt , HICON& hIcon , int& nIndex , DWORD attrib = FILE_ATTRIBUTE_NORMAL);

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
		static bool CheckFileNameValidate(CString strFile , TCHAR* pChar);

		/**
		*  @brief    获取有效文件名称(不支持的符号转换为全角)
		*
		*  @param    const CString strFile 文件名称
		*  @return   CString 转换后的文件名称
		*/
		static CString GetValidateFileName(const CString strFile);

		/**根据文件句柄取得文件路径 */
		static BOOL GetFilePathFromHandle(HANDLE hFile , LPWSTR lpszPath , UINT cchMax);

		/**
		* 目录拷贝
		* @param [in] srcDir		源目录
		* @param [in] dstDir		目的目录
		* @return	是否拷贝成功
		* - true 拷贝成功， false 拷贝失败
		*/
		static BOOL CopyDirectory(const CString& srcDir , const CString& dstDir);

		/**
		*  @brief    计算相对路径
		*
		*  @param    const CString & strSrcPath
		*  @param    const CString & strRelateDir
		*  @return   CString
		*/
		static CString GetRelatePath(const CString& strSrcPath , const CString& strRelateDir);

		/**
		* 取得临时路径
		* @return	返回临时路径字符串，如（c:\\）
		*/
		static CString GetTempPath(void);

		/**
		*  @brief    打开windows 的浏览对话框让用户选择一个目录;
		*
		*  @param    const CString& strTitle	对话框的标题;
		*  @param    CString& strPathChoosed	用户最终选择的路径;
		*  @param    HWND hwndOwner				对话框的父窗口句柄;
		*  @param    const CString& strDirBegin	对话框打开时定位的初始目录;
		*  @return   用户是否进行了选择;
		*/
		static bool OpenBrowseDirDlg(const CString& strTitle, CString& strPathChoosed, HWND hwndOwner = NULL, const CString& strDirBegin = _T(""));

		/**
		* 对文件进行异或加密
		*
		* @param [in] srcFile		源文件
		* @param [in] dstFile		目标文件
		* @param [in] nHeadLen		文件头大小（特殊方式加密)
		* @param [in] nKey			加密秘钥
		*/
		static bool EncryptFile(const CString& srcFile, const CString& dstFile, int nHeadLen = 128, int nKey = 0xC5236911);

		/**
		* 对文件进行异或解密
		*
		* @param [in] srcFile		源文件
		* @param [in] dstFile		目标文件
		* @param [in] nHeadLen		文件头大小（特殊方式加密)
		* @param [in] nKey			加密秘钥
		*/
		static bool DeEncryptFile(const CString& srcFile, const CString& dstFile, int nHeadLen = 128, int nKey = 0xC5236911);

		/**
		* 判断文件是否加密
		*
		* @param [in] srcFile		源文件
		*/
		static bool IsFileEncrypt(const CString& strFile);

		/**
		* 弹出导入对话框
		*
		* @param [in] szFilters		文件后缀名
		* @param [in] strDefExt		文件类型
		*/
		static StringArray ImportFileWnd(const LPCTSTR& szFilters, const LPCTSTR& strDefExt);

		/**
		* 弹出导出对话框
		*
		* @param [in] strName		文件名
		* @param [in] szFilters		文件后缀名
		* @param [in] strDefExt		文件类型
		*/
		static CString ExportFileWnd(const CString& strName, const CString& szFilters, const LPCTSTR& strDefExt);

		
	};
}