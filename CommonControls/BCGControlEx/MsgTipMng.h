//////////////////////////////////////////////////////////////////////
// 文件名称：MsgTipMng.h
// 功能描述：提示窗口管理器
// 创建标识：徐立 2019/10/19
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once
#include "ControlsExportDef.h"

//  #define SHOW_TIP_MSG(hWnd, lpText, lpCaption, uType) \
//  	::MessageBox(hWnd, lpText, lpCaption, uType);

#define SHOW_TIP_MSG(hWnd, lpText, lpCaption, uType) \
	CMsgTipMng::GetInst()->AddTipWindow(hWnd, lpText);

#define SHOW_TIP_MSG_TIME(hWnd, lpText, lpCaption, uType, nTime) \
	CMsgTipMng::GetInst()->AddTipWindow(hWnd, lpText, nTime);

class CMsgTipDlg;
class CONTROLSAPI_EXPORT CMsgTipMng
{
public:
	CMsgTipMng();
	~CMsgTipMng();

	static CMsgTipMng* GetInst();

	/**
	* 释放事务对象内存
	*/
	static void FreeInst();

	/**
	* 添加提示信息
	* @param [in] strTipInfo 提示信息
	* @param [in] nTime 显示时间
	* @return
	*/
	void AddTipWindow(HWND hWnd, const CString& strTipInfo, int nTime = 0);

	/**
	* 销毁当前提示窗口
	* @return
	*/
	void DestroyTipDialog();

private:
	static CMsgTipMng		*_pInst;
	CMsgTipDlg* _TipDialog;
};

