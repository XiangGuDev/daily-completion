//////////////////////////////////////////////////////////////////////
// 文件名称：ProgressEvent.h
// 功能描述：进度条事件
// 创建标识：曾江佑	2018.09.17
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ProductSDK.h"

namespace toolkit {
	namespace deskface {

		/** @addtogroup Toolkit
		* @{
		*/
		//! 进度条事件
		class IProgressEvent
		{
		public:
			~IProgressEvent() {}

			/**
			* 进度开始
			* @param [in] task	任务名称
			* @param [in] StepMax	进度最大值
			*/
			virtual void Begin(LPCTSTR strTask, int nStepMax) {}

			/**
			* 进度结束
			*/
			virtual void End(void) {}

			/**
			* 进度增加（默认一个单位值1）
			* @param [in] strMsg = NULL
			*/
			virtual void Step(LPCTSTR strMsg = NULL) {}

			/**
			* 进度增加nPos个值
			* @param [in] nPos	进度值
			* @param [in] strMsg = NULL 任务名称
			*/
			virtual void StepAt(int nPos, LPCTSTR strMsg = NULL) {}

			/**
			* 设置一步走动的距离
			* @param [in] nSize
			*/
			virtual void SetStepSize(int nSize) = 0;
		};/** * @} */

		/** @addtogroup Toolkit
		* @{
		*/
		//! 任务线程
		class ITaskThread : public spring::commkit::CThreadImpl<ITaskThread>
		{
		public:
			virtual ~ITaskThread() {}

			/**
			* 执行
			*/
			virtual DWORD Run(void) = 0;

			/**
			* 获取错误信息
			*/
			virtual LPCTSTR GetLastError(void) = 0;

			/**
			* 错误列表
			* @param [in out] 错误列表
			*/
			virtual void GetErrorList(std::vector<LPCTSTR> &errList) {}

			/**
			* 时间跳动(用于实现自动走进度条，如果要实现实际进度不实现此方法即可）
			*/
			virtual void TimeTick(void) {};

			/**
			* 获取进度事件
			* return 进度事件对象
			*/
			virtual IProgressEvent *GetProgressEvent() = 0;

			/**
			* 设置进度事件
			* @param [in] p 进度事件对象
			*/
			virtual void SetProgressEvent(IProgressEvent *p) = 0;
		};/** * @} */




	}
}
