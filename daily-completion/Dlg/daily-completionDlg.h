﻿
// daily-completionDlg.h: 头文件
//

#pragma once
#include "YFramework/BindableProperty/BindableProperty.h"
#include "YFramework/Singleton/BaseSingleton.h"
#include "../Base/DCController.h"
#include "../Base/BaseTaskDlg.h"

namespace ControlUI
{
	class CListCtrlDataSource;
}
class CTaskListCtrl;
class CSettingsDlg;
class CTimeSettingDlg;
class CdailycompletionDlg : public CBaseTaskDlg, public DCController, public BaseSingleton<CdailycompletionDlg>
{
	friend class BaseSingleton<CdailycompletionDlg>;
private:
	CdailycompletionDlg(CWnd* pParent = nullptr);	// 标准构造函数
public:
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAILYCOMPLETION_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickMenu();
	afx_msg void OnClickTimeSetting();
	afx_msg void OnDestroy();
	afx_msg void OnSearchKeyChanged();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTaskListEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnMouseButtonDown(WPARAM wParam, LPARAM lParam);
private:
	CBCGPButton _btnMenu; // 菜单
	CBCGPButton _btnTimeSetting; // 菜单
	CBCGPEdit _editSearch; // 搜索
	std::shared_ptr<CTaskListCtrl> _taskList; // 任务列表
	std::shared_ptr< CSettingsDlg> _settingsDlg; // 设置窗口
	std::shared_ptr< CTimeSettingDlg> _timeDlg; // 时间设置窗口
	bool _bDontHide;
};


