/*******************************************************************
* 文件名称：BCGPComboBoxEx.h
* 作者姓名：刘庆仙
* 文件描述：BCGPComboBox的扩展类
* 需求版本：
* 创建时间：2018-6-8
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/
#pragma once

#include "BCGPComboBox.h"


/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! BCGP下拉框扩展
		class CONTROLSAPI_EXPORT CBCGPComboBoxEx : public CBCGPComboBox
		{
		public:
			controls::CBCGPComboBoxEx();
			virtual ~CBCGPComboBoxEx();

		public:
			/**
			* 绘制下拉项
			* @param [in] lpDIS	自绘控件信息
			* @return
			*/
			virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

			/**
			* 窗口的消息
			* @param [in] message 指定消息类型。
			* @param [in] wParam  指定其余的、消息特定的信息。该参数的内容与message参数值有关。
			* @param [in] IParam  指定其余的、消息特定的信息。该参数的内容与message参数值有关。
			* 返回值：返回值就是消息处理结果，它与发送的消息有关。
			*/
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

			/**
			* 设置某行不可点击
			* @param [in] nRowIndex	行索引
			* @return
			*/
			void SetEnable(int nRowIndex);

			/**
			* 区分Combox使用的地方(目前写死：1为功能区使用的Combox,2为快速访问栏使用的Combox)
			* @param [in] nType
			* @return
			*/
			void SetEnableType(int nType);
		private:
			int _nIndex;			// 当前选中的行
			int _nEnableIndex;		// 不可点击的行索引
			int _nType;				// 分类(目前写死：1为功能区使用的Combox,2为快速访问栏使用的Combox)
		};
		/** * @} */
	}