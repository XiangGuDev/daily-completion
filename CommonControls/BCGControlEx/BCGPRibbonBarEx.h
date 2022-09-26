/*******************************************************************
* 文件名称：BCGPRibbonBarEx.h
* 作者姓名：刘庆仙
* 文件描述：RibbonBar扩展类
* 需求版本：
* 创建时间：2018-5-31
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/
#pragma once

#include "BCGPRibbonBar.h"

class CBCGPBaseRibbonElement;
class CBCGPRibbonMainPanelEx;
/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! BCGP功能条控件扩展
		class CONTROLSAPI_EXPORT CBCGPRibbonBarEx : public CBCGPRibbonBar
		{	
		public:
			controls::CBCGPRibbonBarEx();
			~CBCGPRibbonBarEx();

		public:
			/**
			* 显示自定义区入口菜单
			* @param [in] pWnd  窗口对象
			* @param [in] x		菜单显示位置的x坐标
			* @param [in] y		菜单显示位置的y坐标
			* @param [in] pHit  Ribbon控件对象
			* @return
			*/
			virtual BOOL OnShowRibbonContextMenu(CWnd *pWnd, int x, int y,
				CBCGPBaseRibbonElement *pHit);

			CBCGPRibbonMainPanelEx* AddMainCategoryEx(
				LPCTSTR		lpszName,
				UINT		uiSmallImagesResID,
				UINT		uiLargeImagesResID,
				CSize		sizeSmallImage = CSize(16, 16),
				CSize		sizeLargeImage = CSize(32, 32),
				CRuntimeClass*	pRTI = NULL);

			void SetmAutoDestroy(BOOL bAutoDestory);

			BOOL GetBackstageMode()
			{
				return m_bBackstageMode;
			}

			BOOL GetPrintPreviewMode()
			{
				return m_bPrintPreviewMode;
			}

			BOOL OnBeforeShowBackstageViewEx()
			{
				return OnBeforeShowBackstageView();
			}

			BOOL OnBeforeShowMainPanelEx()
			{
				return OnBeforeShowMainPanel();
			}

			virtual void RecalcLayout();
		};
		/** * @} */
	}