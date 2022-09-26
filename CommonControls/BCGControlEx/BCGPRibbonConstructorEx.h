/*******************************************************************
* 文件名称：BCGPRibbonConstructorEx.h
* 作者姓名：刘庆仙
* 文件描述：创建自定义控件
* 需求版本：
* 创建时间：2018-4-16
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/
#pragma once

#include "BCGPRibbonConstructor.h"
#include "BCGPRibbonMainPanelEx.h"

class CBCGPRibbonBar;
class CBCGPRibbonStatusBar;

class CBCGPRibbonConstructorEx : public CBCGPRibbonConstructor
{
public:
	CBCGPRibbonConstructorEx(const CBCGPRibbonInfo& info);

	virtual ~CBCGPRibbonConstructorEx() {};

public:
	/**
	* 创建控件
	* @param [in] info 控件信息
	* @return 返回控件实体
	*/
	virtual CBCGPBaseRibbonElement* CreateElement(const CBCGPRibbonInfo::XElement& info) const;

	/**
	* 创建主菜单
	* @param [in] info 控件信息
	* @return
	*/
	virtual void ConstructCategoryMain(CBCGPRibbonBar& bar, const CBCGPRibbonInfo::XCategoryMain& info) const;

	/**
	* 创建菜单按钮
	* @param [in] bar 
	* @return
	*/
	virtual CBCGPRibbonMainButton* CreateMainButton(CBCGPRibbonBar& bar) const;

	/**
	* 创建菜单按钮
	* @param [in] info bar
	* @param [in] info 控件信息
	* @return
	*/
	virtual CBCGPRibbonMainPanelEx* CreateCategoryMainEx(CBCGPRibbonBar& bar, const CBCGPRibbonInfo::XCategoryMain& info) const;

	/**
	* 判断是否是自定义控件
	* @param [in] strID			 控件ID名
	* @param [in] strEleType	 控件类别
	* @return 是自定义控件返回TRUE,否则返回FALSE
	*/
	BOOL IsCustomElement(const CString& strID, 
						 CString& strEleType) const;

};

