#pragma once
/*******************************************************************
* 文件名称：Progress.h
* 作者姓名：杨佳东
* 文件描述：进度对话框
* 需求版本：
* 创建时间：2018-9-18
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/

#include "ProductSDK.h"
#include <spring\commkit\TimeTrigger.h>
#include "ControlsExportDef.h"
#include <Toolkit\include\IProgressEvent.h>


class ITaskThread;

/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! 进度对话框
		class CONTROLSAPI_EXPORT CProgress : public CDialog, public toolkit::deskface::IProgressEvent, public spring::commkit::ITimeListenner
		{
		public:
			/**
			* 构造函数
			* @param [in]		thread		任务执行线程
			* @param [in]		pParent		父级窗口
			*/
			controls::CProgress(toolkit::deskface::ITaskThread* thread, CString strTaskName, CWnd* pParent = NULL);

			/**
			* 析构函数
			*/
			virtual ~CProgress();

		protected:
			void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
			BOOL OnInitDialog();

			virtual BOOL PreTranslateMessage(MSG* pMsg);

		public:
			/**
			* 时间循环
			*/
			void OnTime();

			/**
			* 进度开始
			* @param [in]		strTask			任务名称
			* @param [in]		iStepMax		进度最大值
			*/
			void Begin(LPCTSTR strTask, int iStepMax);

			/**
			* 进度结束
			*/
			void End();

			/**
			* 进度增加（默认一个单位值1）
			* @param [in]		strMsg= NULL	任务名称
			*/
			void Step(LPCTSTR strMsg = NULL);

			/**
			* 进度增加iPos个值
			* @param [in]		iPos			进度值
			* @param [in]		strMsg = NULL	任务名称
			*/
			void StepAt(int iPos, LPCTSTR strMsg /*= NULL*/);

			/**
			* 设置一步走动的距离
			* @param [in] nSize
			*/
			void SetStepSize(int iSize);

			/**
			* 关闭进度条控件
			*/
			void Close();

			/**
			* 显示对话框
			* @param [in] thread 进度值
			* @param [in] strTaskName 任务名称
			*/
			static void ShowDalog(toolkit::deskface::ITaskThread* thread, CString strTaskName, CWnd* parent = nullptr);

		protected:
			CProgressCtrl						_ProgressBar;		// 进度条控件
			CStatic								_TaskMsgCtrl;		// 任务名称显示标签
			spring::commkit::CTimeTrigger		_Timer;				// 时间循环控制器
			toolkit::deskface::ITaskThread*		_TaskThread;		// 事务执行线程
			int									_iStep;				// 一步长度
			CString								_strTaskName;		// 任务名称
			CString								_strLastMsg;		// 上一条消息
		};
		/** * @} */
	}