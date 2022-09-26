#pragma once

enum DialogSetting_PosType { byNameFrame = 1, centerWnd };
DECLARE_ENUM_CONVERSION(DialogSetting_PosType)

// 对话框设置
struct DialogSetting
{
	CString strCaptionName;		// 标题栏
	DialogSetting_PosType posType;
};

class CTabDialog : public CDialog
{
	enum {IDD = IDD_TAB_DIALOG};
public:
	CTabDialog(TabPageContextArray& arrSubPage, DialogSetting dlgSetting);
	~CTabDialog();

protected:
	DECLARE_MESSAGE_MAP()

	// 初始化对话框时，创建页面
	afx_msg BOOL OnInitDialog();

	// 调整窗口大小时，调整tab控件大小
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 响应tab页切换事件，通知子页面离开和进入
	afx_msg void OnTabSelChange();

	// 重载确定，避免按回车键就结束了
	virtual void OnOK();

	virtual void OnCancel();

protected:
	CEditableTabCtrl _tabCtrl;				// tab控件
	TabPageContextArray &_arrSubPages;		// 所有页面对象
	ITabPageContext *_pLastSelPage;			// 上一次选中的页面
	DialogSetting	_dlgSetting;			// 界面设置
};

