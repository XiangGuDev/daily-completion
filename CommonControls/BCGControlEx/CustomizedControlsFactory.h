/*******************************************************************
* 文件名称：CustomizedControlsFactory.h
* 作者姓名：刘庆仙
* 文件描述：自定义控件工厂
* 需求版本：
* 创建时间：2018-4-17
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/

#pragma once

#include <map>
#include "ControlsExportDef.h"


/** @addtogroup Controls
* @{
*/
	namespace controls {
		class ICustomizedControlCreator;

		typedef std::map<CString, controls::ICustomizedControlCreator*> MapControlCreator;

		//! 自定义控件工厂
		class CONTROLSAPI_EXPORT CCustomizedControlFactory
		{
		public:
			controls::CCustomizedControlFactory();
			virtual ~CCustomizedControlFactory();

			/**
			* 命令管理器实例
			*/
			static controls::CCustomizedControlFactory* GetInstance();

			/**
			* 清除数据
			* @return
			*/
			static void ClearData();
		public:

			/**
			* 获取自定义控件管理器
			* @param [in] strElemName 自定义控件名
			* @return 返回自定义控件管理器
			*/
			controls::ICustomizedControlCreator* GetCustomizedControlCreator(const CString& cstrElemName);

			/**
			* 注册自定义控件管理器
			* @param [in] strElemName 自定义控件名
			* @param [in] pControlCreator 自定义控件管理器
			*/
			void RegCustomizedControlCreator(const CString& cstrElemName, controls::ICustomizedControlCreator* pControlCreator);

		private:
			static controls::CCustomizedControlFactory* _pCustomizedControlFactory;	// 命令函数集合单例
			MapControlCreator m_mapControlCreator;							// 控件管理器Map
		};
		/** * @} */
	}