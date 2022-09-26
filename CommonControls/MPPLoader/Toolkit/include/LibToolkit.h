
#pragma once

//=========================================================
// 应用程序辅助工具臬
//=========================================================
// begin namespace
namespace BwProj{ namespace toolkit{

/** @addtogroup CadreLibToolkit
* @{
*/


#ifdef _DEBUG
#define TIME_TEST_INIT			 \
	DWORD dwStart = 0; \
	DWORD dwEnd = 0;

#define TIME_TEST_START			 \
	dwStart = ::GetTickCount ();

#define TIME_TEST_END			 \
	dwEnd = ::GetTickCount ();

#define TIME_TEST_PRINT(msg)	 \
	ATLTRACE ("%s: %i ms.\n", msg, dwEnd - dwStart);
#else
#define TIME_TEST_INIT
#define TIME_TEST_START
#define TIME_TEST_END
#define TIME_TEST_PRINT(msg)
#endif

/** 系统图标索引枚举类型 */
typedef enum
{
	SYSTEM_ICON_DOCUMENT = 1,
	SYSTEM_ICON_NETWORK_AREA = 17,
	SYSTEM_ICON_FOLDER_CLOSE = 3,
	SYSTEM_ICON_FOLDER_OPNE = 4,
	SYSTEM_ICON_FLOPPY = 6,
	SYSTEM_ICON_HARDWARE_DISK = 7,
	SYSTEM_ICON_NETLINK_DISK = 9,
	SYSTEM_ICON_NETLINK_DISK_DISCONNECT = 10,
	SYSTEM_ICON_CDROOM = 11,
	SYSTEM_ICON_INTERNET = 13,
	SYSTEM_ICON_MYCOMPUTER = 15,
	SYSTEM_ICON_PRINTER = 16,
	SYSTEM_ICON_FINDER = 22,
	SYSTEM_ICON_HELPER = 23,
	SYSTEM_ICON_RUN = 24,
	SYSTEM_ICON_SHUTDOWN = 27,
	SYSTEM_ICON_RECYCLE_EMPTY = 31,
	SYSTEM_ICON_RECYCLE_FULL = 32,
	SYSTEM_ICON_DESKTOP = 34,
	SYSTEM_ICON_FONT_FOLDER = 38,
	SYSTEM_ICON_MUSIC = 40,
	SYSTEM_ICON_IEFAV_FOLDER = 43,
	SYSTEM_ICON_LOCK = 44,
}SYSTEM_ICON_INDEX;

//! 应用程序辅助工具类
class COMMONTOOLS_API CLibToolkit
{
public:
	/** 
	* 生成 GUID
	* @return	返回生成的GUID字符串
	*/
	static CString CreateGuid (void);

	/** 
	* 取得模块路径
	* @param [in] hInst		模块的句柄
	* @return	返回模块路径，如（c:\\a.exe）
	*/
	static CString GetAppModuleFilename (HINSTANCE hInst);

	/** 
	* 取得模块路径
	* @param [in] hInst		模块的句柄
	* @return	返回模块路径，如（c:\\）
	*/
	static CString GetAppModulePath (HINSTANCE hInst);

	/** 
	* 取得临时路径
	* @return	返回临时路径字符串，如（c:\\）
	*/
	static CString GetTempPath (void);

	/** 
	* 取得API出错信息
	* @return	返回API出错信息
	*/
	static CString GetWin32LastError (void);

	/** 
	* 取得系统图标 
	* @param [in] index		系统图标索引
	* @return	系统图标对象
	*/
	static HICON GetSystemIcon(SYSTEM_ICON_INDEX index);

	// 获取进程路径列表
	static int LoadProcessList(CAtlArray<CString>& list);
};

/** * @} */
}}	// end namespace
/** @} */