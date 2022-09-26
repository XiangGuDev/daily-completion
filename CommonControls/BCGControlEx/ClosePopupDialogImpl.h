//////////////////////////////////////////////////////////////////////
// 文件名称：ClosePopupDialogImpl.h
// 功能描述：弹出框所属父级控件管理
// 创建标识：刘庆仙	2018\09\29
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once


/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! 弹出框所属父级控件管理
		class CONTROLSAPI_EXPORT CClosePopupDialogImpl
		{
		public:
			controls::CClosePopupDialogImpl();
			~CClosePopupDialogImpl();

		public:
			/**
			* 判断是否有子窗口
			* @return 有:返回TRUE
			*/
			BOOL IsHaveChildDlg() const;

			/**
			* 标记子窗口
			* @param [in] bHaveChildDlg 子窗口标记
			*/
			void SetChildDlg(BOOL bHaveChildDlg);

			/**
			* 关闭弹出窗口
			* @param [in] lpszEditValue 控件文本
			* @param [in] bOK			控件文本
			* @param [in] dwUserData	绑定数据
			*/
			virtual void ClosePopupDlg(LPCTSTR lpszEditValue,
				BOOL		bOK,
				DWORD_PTR dwUserData);

			/**
			* 获取父控件文本
			* @return 父控件文本
			*/
			virtual CString GetParentText() { return _T(""); }

			/**
			* 设置父控件文本
			* @param [in] strText 控件文本
			* @return
			*/
			virtual void SetParentText(const CString& strText) { }

		protected:
			BOOL					_bHaveChildDlg;			// 是否有子对话框
		};
		/** * @} */
	}