// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

// TODO:  在此处引用程序需要的其他头文件

#pragma pack (push, 8)
#pragma warning(disable: 4786 4996 4819)

#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//- Exclude rarely-used stuff from Windows headers
#endif

#ifndef WINVER					//- Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501			//- Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT			//- Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		//- Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS			//- Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0510	//- Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE				//- Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0600		//- Change this to the appropriate value to target IE 5.0 or later.
#endif
#include <afx.h>

//#include <vld.h>

//-----------------------------------------------------------------------------
#include <afxwin.h>				//- MFC core and standard components
#include <afxext.h>				//- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//- MFC OLE classes
#include <afxodlgs.h>			//- MFC OLE dialog classes
#include <afxdisp.h>			//- MFC Automation classes
#endif

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//- MFC ODBC database classes
#endif

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//- MFC DAO database classes
#endif

#include <afxdtctl.h>			//- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//- MFC support for Windows Common Controls
#endif

#include "resource.h"
//1、系统依赖项
//std
#include <cmath>
#include <cfloat>
#include <vector>
#include <map>
#include <string>
#include <queue>
//windows
#include <Windows.h>
#include <Shlwapi.h>

//2、第三方组件依赖
#include <BCGCBProInc.h>
#include <osglib.h>
#include <VMP\VMProtectSDK.h>
#include <fbxsdk.h>
//d3station
#include "d3mlib.h"
#include <Toolkit/include.h>
using namespace toolkit;
#include <Platform/include.h>
#include <Geometry/include.h>
#include <DesignFile/include.h>
#include <Controls/include.h>
#include <Views/include.h>
#include <Algorithm/include.h>
//3、本解决方案其他项目依赖
#include "../CommonTools/CommonToolsDef.h"
#include "DataModel/DataModelDef.h" 

//自身依赖
#include "CommonControlsDef.h" 

