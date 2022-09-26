//////////////////////////////////////////////////////////////////////
// 文件名称：ToolBarCreator.h
// 功能描述：工具条创建器
// 创建标识：王海民	2018/04/18
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once
#include "ControlsExportDef.h"

class CToolBarCtrl;
class CBCGPToolBar;
#define ID_USER_TOOLBAR_BEGIN                    48000
#define ID_USER_TOOLBAR_END						50000

/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! 工具条创建器
		class CONTROLSAPI_EXPORT CToolBarCreator
		{
			class CData;
		public:
			controls::CToolBarCreator();
			~CToolBarCreator();

			/**
			* 根据配置文件生成ToolBar
			* @param [in] pToolBar	BCG的ToolBar
			* @param [in] xmlFile
			* @return
			*/
			bool CreateToolBar(CBCGPToolBar *pToolBar, LPCTSTR xmlFile);

			/**
			* 获取命令字符串
			* @param [in] id
			* @return
			*/
			LPCTSTR GetCommandString(int id);

			/**
			* 获取命令字符串
			* @param [in] id
			* @return
			*/
			LPCTSTR GetEnableString(int id);

		private:
			CData						*_pData;
		};
		/** * @} */
	}