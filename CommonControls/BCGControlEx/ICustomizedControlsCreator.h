/*******************************************************************
* 文件名称：ICustomizedControlsCreator.h
* 作者姓名：刘庆仙
* 文件描述：自定义控件创建器接口
* 需求版本：
* 创建时间：2018-4-16
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/

#pragma once
#include <atlstr.h>
#include "ControlsExportDef.h"
#include "BCGPRibbonInfo.h"
class CBCGPBaseRibbonElement;

// 自定控件创建器基类
class CBCGPRibbonConstructorEx;

/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! 自定控件创建器基类
		class CONTROLSAPI_EXPORT ICustomizedControlCreator
		{
		public:
			virtual ~ICustomizedControlCreator() {};

		public:
			/**
			* 创建控件
			* @param [in] strCustomizedDefine	控件信息
			* @return CBCGPBaseRibbonElement* Ribbon控件
			*/
			virtual CBCGPBaseRibbonElement* CreateControl(const CBCGPRibbonInfo::XElement& info, CBCGPRibbonConstructorEx *pConstructor) = 0;
		};
		/** * @} */
	}