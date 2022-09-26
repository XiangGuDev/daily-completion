/*******************************************************************
* 文件名称：BCGPSplitterWndEx.h
* 作者姓名：刘庆仙
* 文件描述：分割窗口扩展类
* 需求版本：
* 创建时间：2018-5-28
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/
#pragma once
#include <afxext.h>
#include "BCGPSplitterWnd.h"


/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! BCGP分割窗口扩展
		class CONTROLSAPI_EXPORT CBCGPSplitterWndEx : public  CBCGPSplitterWnd
		{
		public:
			controls::CBCGPSplitterWndEx() {}
			virtual ~CBCGPSplitterWndEx() {}

		public:
			/**
			* 拦截窗口消息
			* @param [in] pMsg 消息信息
			* @return
			*/
			virtual BOOL PreTranslateMessage(MSG* pMsg);

			virtual void StartTracking(int ht);

			void StopTracking(BOOL bAccept);
		protected:
			/**
			* 处理控件通知的消息映射
			* @param [in] wParam 指定其余的、消息特定的信息
			* @param [in] lParam 指定其余的、消息特定的信息
			* @param [in] pResult
			* @return
			*/
			virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		protected:
			/**
			* 分割窗口中的属性框的某行属性值发生变化
			* @param [in] wParam	指定其余的、消息特定的信息
			* @param [in] lParam	指定其余的、消息特定的信息
			* @return 返回值就是消息处理结果，它与发送的消息有关
			*/
			afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnGridDBLClick(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnEndLabelEdit(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnBeginLabelEdit(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnGridItemChange(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnGridSelectChange(WPARAM wParam, LPARAM lParam);
			DECLARE_MESSAGE_MAP()
		};
		/** * @} */
	}