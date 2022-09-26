//////////////////////////////////////////////////////////////////////
// 文件名称：RibbonTransButton.h
// 功能描述：透明度菜单按钮
// 创建标识：刘庆仙 2018/12/10
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once
#include "ControlsExportDef.h"
#include "BCGPRibbonComboBox.h"

/** @addtogroup Controls
* @{
*/
namespace controls {

	//! 透明度菜单按钮
	class CONTROLSAPI_EXPORT CRibbonTransButton : public CBCGPRibbonComboBox
	{
		DECLARE_DYNCREATE(CRibbonTransButton)
	public:
		controls::CRibbonTransButton(UINT nID, const CString& strText, BOOL bHasEditBox = TRUE,
			int nWidth = -1, LPCTSTR lpszLabel = NULL, int nImage = -1,
			BCGP_RIBBON_COMBO_SORT_ORDER sortOrder = BCGP_RIBBON_COMBO_SORT_ORDER_NO_SORT);

		virtual ~CRibbonTransButton();

	public:
		controls::CRibbonTransButton();

	public:
		/**
		* 设置文本信息
		* @param [in] strText 文本信息
		*/
		void SetText(const CString& strText);

		/**
		* 获取文本
		* @param [in] strText 文本信息
		*/
		CString GetText() const;

	protected:
		/**
		* 绘制控件
		* @param [in] pDC 设备上下文
		*/
		virtual void OnDraw(CDC* pDC);

		/**
		* 左键按下
		* @param [in] point 鼠标点击位置
		*/
		virtual void OnLButtonDown(CPoint point);

	protected:
		CString		_strText;		// 文本
	};
	/** * @} */
}