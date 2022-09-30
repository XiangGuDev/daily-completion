
// daily-completionDlg.h: 头文件
//

#pragma once
#include "YFramework/BindableProperty/BindableProperty.h"
#include "YFramework/Singleton/BaseSingleton.h"
#include "../Base/DCController.h"

namespace ControlUI
{
	class CListCtrlDataSource;
}
class CTaskListCtrl;
class CdailycompletionDlg : public CDialogEx, public DCController
{
public:
	CdailycompletionDlg(CWnd* pParent = nullptr);	// 标准构造函数
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
	afx_msg void OnDestroy();
private:
	CBCGPButton _btnMenu; // 菜单
	CBCGPEdit _editSearch; // 搜索
	std::shared_ptr<CTaskListCtrl> _taskList; // 任务列表
};


