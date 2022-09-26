//////////////////////////////////////////////////////////////////////
// 文件名称：BCGPDialogTEx.h
// 功能描述：对话框二次扩展类
// 创建标识：刘庆仙	2018\08\22
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once
#include "BCGPDialog.h"

#include "ClosePopupDialogImpl.h"

/** @addtogroup Controls
* @{
*/
	namespace controls {

		//! BCGP对话框二次扩展
		class CONTROLSAPI_EXPORT CBCGPDialogTEx : public CBCGPDialog
		{
			DECLARE_DYNCREATE(CBCGPDialogTEx)

		public:
			controls::CBCGPDialogTEx();

			controls::CBCGPDialogTEx(UINT nIDTemplate, CWnd *pParent = NULL);

			controls::CBCGPDialogTEx(LPCTSTR lpszTemplateName, CWnd *pParentWnd = NULL);

		public:
			controls::CClosePopupDialogImpl* _pParentCtl;		//关联控件信息

		protected:
			afx_msg void OnDestroy();
			DECLARE_MESSAGE_MAP()
		};
		/** * @} */
	}