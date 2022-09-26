#pragma once

#include "EditBase.h"
#include "EditContainer.h"

namespace ControlUI
{

	typedef std::vector<std::pair<CString, CString>> DropListSet;

	/**
	*  @class    CAutoSearchCombo
	*
	*  @brief    列表普通组合框控件
	*/
	class CAutoSearchCombo : public CComboBox, public IEditBase
	{
		DECLARE_DYNAMIC(CAutoSearchCombo)

	public:
		CAutoSearchCombo();

		virtual ~CAutoSearchCombo();

	public:
		/**
		*  @brief    创建控件;
		*
		*  @param    CWnd * pParent 父窗口句柄;
		*  @param    CRect & rect 绘制区域;
		*  @param    bool bDropList 是否不可编辑;
		*  @param    LPCTSTR sInitText 默认显示文本;
		*  @return   bool 创建成功返回true，否则返回false;
		*/
		bool Create(CWnd* pParent, CRect& rect, int option, LPCTSTR sInitText);

		bool LoadXmlData(const CString& strXmlFile);

		void LoadStringVector(CStringVector& listData);

		void ProcessKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

		void SetSelAndFocus();

		void DoKillFocus(CWnd* pWnd);

		void CancelEdit();

		WNDPROC			_pEditProc;  ///< 编辑框消息

	protected:
		void DrawItem(HDC hDC, const CRect& rcItem, int iItem, int iState);

		int ResetDropContent();

		virtual void MeasureItem(LPMEASUREITEMSTRUCT pMItem);

		virtual void DrawItem(LPDRAWITEMSTRUCT pDIStruct);

		virtual BOOL PreTranslateMessage(MSG* pMsg);

		DECLARE_MESSAGE_MAP()

		afx_msg void OnKillFocus(CWnd* pNewWnd);

		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

		afx_msg void OnEditChange();

		afx_msg void OnDropDown();

		CString			_strInitText;		///< 初始显示值
		CString			_strData;			///< 组合框的扩展数据
		bool			_bDropList;			///< 无输入框模式
		bool			_bCandel;			///< 是否取消
		bool			_autoComplete;		///< 自动完成匹配
		COLORREF		_crNormal;			///< 正常颜色
		COLORREF		_crSelected;		///< 选择颜色
		COLORREF		_crText;			///< 文本颜色
		CEditContainer	_container;			///< 边框容器
		DropListSet		_loadStringList;	///< 加载所有的下拉列表
	};
}