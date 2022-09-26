//********************************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           CCustomComboBox.H
//    @author         wuxuelong
//    @date           2016/4/29 10:01
//    @brief          自定义Combo（可设置父窗口）
//*********************************************************************

#pragma once

namespace ControlUI
{
	class CONTROL_UI CCustomComboBox : public CAutoWidthComboBox
	{
		DECLARE_DYNAMIC(CCustomComboBox)

	public:
		CCustomComboBox();
		~CCustomComboBox();

		DECLARE_MESSAGE_MAP()

	public:

		/**
		*  @brief    设置信息父窗口
		*
		*  @param    CWnd * pParent
		*/
		void SetMsgParent(CWnd* pParent);

		virtual int SetItemData(int nIndex, const TCHAR* strData);

		virtual void MoveWindow(int x, int y, int nWidth, int nHeight,
			BOOL bRepaint = TRUE);
		virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

	protected:
		afx_msg void OnComboSelected();

	private:
		std::vector<TCHAR*>	_arrStrData;
		CWnd*	_pParent;		///< 父窗口
	};
}