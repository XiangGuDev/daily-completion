//////////////////////////////////////////////////////////////////////
// 文件名称：BCGPDialogEx.h
// 功能描述：对话框扩展类
// 创建标识：刘庆仙	2018\08\22
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once
#include "BCGPDialog.h"

/** @addtogroup Controls
* @{
*/
	namespace controls {
		HMODULE GetSelfModuleHandle();

		class CChangeModule
		{
		public:
			CChangeModule();
			~CChangeModule();
		private:
			HINSTANCE _hInstance;
		};

		//! BCGP对话框扩展
		class CBCGPDialogEx : public CBCGPDialog
		{
			DECLARE_DYNCREATE(CBCGPDialogEx)
		public:
			controls::CBCGPDialogEx();

			controls::CBCGPDialogEx(UINT nIDTemplate, CWnd *pParent = NULL);

			controls::CBCGPDialogEx(LPCTSTR lpszTemplateName, CWnd *pParentWnd = NULL);

		public:
			BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd);
		};
		/** * @} */
	}