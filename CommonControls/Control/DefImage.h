//*****************************************************
//    
//    @copyright      变电三维设计
//    @version        v1.0
//    @file           DefImage.H
//    @author         KG
//    @date           2018/4/20 13:50
//*****************************************************

#pragma once

/**
*  @class    默认图片资源类
*
*  @brief
*/
class COMMONCONTROLS_API CDefImage
{
public:
	/**
	*  @brief    获取软件目录
	*
	*  @return   HICON
	*/
	static HICON LoadSysIcon();

	/**
	*  @brief    获取Resource\Image目录中图标
	*
	*  @return   HICON 返回图标(自行释放)
	*/
	static HICON LoadIcon(const CString& strFile);

	/**
	*  @brief    获取Resource\Image目录中位图
	*
	*  @return   HBITMAP 返回位图(自行释放)
	*/
	static HBITMAP LoadBitmap(const CString& strFile);
};
