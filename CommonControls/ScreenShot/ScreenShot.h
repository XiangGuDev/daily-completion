//****************************************************************
//
//   @copyright     配网设计组
//   @version       v1.0
//   @file          屏幕截图工具
//   @author        HBQing
//   @date          2018/12/31 14:39
//*****************************************************************/

#pragma once

extern "C" _declspec(dllimport) int StartScreenshot(LPCTSTR szSaveFileName);

class COMMONCONTROLS_API CScreenShot
{
	CScreenShot();

public:
	/**
	*  @brief    截图功能
	*
	*  @param    LPCTSTR szSaveFileName 保持截图文件名称（支持的保存格式:BMP/TIFF/GIF/JPEG/PNG）
	*  @return   bool
	*/
	static bool DoScreenShot(LPCTSTR szSaveFileName);

};