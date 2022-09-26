/*******************************************************************
* 文件名称：ControlsCreator.h
* 作者姓名：刘庆仙
* 文件描述：加载自定义控件
* 需求版本：
* 创建时间：2018-4-16
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/

#pragma once
#include <vector>
#include "ControlsExportDef.h"
#include "BCGPRibbonInfo.h"

class CBCGPRibbonBar;

/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! 加载自定义控件
		class CONTROLSAPI_EXPORT CRibbonBarCreator
		{
		public:
			/**
			* 加载控件信息
			* @param [in] lpszFileName	资源路径
			* @param [in] pBar			RibbonBar
			* @return 加载成功返回TRUE
			*/
			static  BOOL LoadFromResPath(LPCTSTR lpszFileName, CBCGPRibbonBar *pBar);
		};
		/** * @} */
	}