//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           TitleTip.H
//    @author         jzl
//    @data           2015/7/15 19:25
//    @brief          标题提示控件
//*****************************************************

#pragma once

namespace ControlUI
{
	/**
	*  @class    标题提示控件类
	*
	*  @brief
	*/
	class CONTROL_UI CTitleTip : public CStatic
	{
	public:
		CTitleTip();
		~CTitleTip();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief    创建控件
		*
		*  @return   bool 成功返回TRUE
		*/
		bool Create(CWnd* pParentWnd, CFont* pFont);

		/**
		*  @brief    设置颜色
		*
		*/
		void SetColor(COLORREF clrBack, COLORREF clrBorder);

		/**
		*  @brief    设置文本绘制模式
		*
		*/
		void SetTextMode(int nTextMode);

		/**
		*  @brief    设置提示文本
		*
		*/
		void SetTipText(const CString& strTipText);

		/**
		*  @brief    设置监控区域
		*
		*/
		void SetRect(CRect rcMonitor, CRect rcWnd);

		/*@brief    设置是否颜色文本
		*
		*/
		void SetClrText(bool bClrText);

		/**
		*  @brief    处理提示显示
		*
		*/
		void RelayEvent(PMSG pMsg);

		/**
		*  @brief    重置提示信息
		*
		*/
		void Reset();

	public:
		void OnPaint();
		BOOL OnEraseBkgnd(CDC* pDC);

	private:
		bool _bShow; ///< 是否显示
		CFont* _pFont; ///< 字体
		CRect _rcMonitor; ///< 监测的区域
		CRect _rcWnd;	///< 控件区域
		int _nTextMode; ///< 文字格式
		bool _bClrText;	///< 是否带颜色文本
		CWnd* _pParent; ///< 父亲窗口
		COLORREF _clrBack;	///< 背景色
		COLORREF _clrBorder; ///< 边框颜色
		CString _strTipText;	///< 提示文本
	};
}