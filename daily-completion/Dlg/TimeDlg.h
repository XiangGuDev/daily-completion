
// daily-completionDlg.h: 头文件
//

#pragma once
#include "YFramework/BindableProperty/BindableProperty.h"
#include "YFramework/Singleton/BaseSingleton.h"
#include "../Base/DCController.h"

class CTaskListCtrl;
class CTimeDlg : public CDialogEx, public DCController, public BaseSingleton<CTimeDlg>
{
	friend class BaseSingleton<CTimeDlg>;
	CTimeDlg(CWnd* pParent = nullptr);	// 标准构造函数
public:
	// 对话框数据
	enum {
		IDD = IDD_TIME_DLG
	};

	void Reset();
	void Start();
	void Pause();
	void SetTip(const CString &tip);
protected:
	void UpdateTime();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickIcon();
private:
	CBCGPStatic _content;
	CBCGPButton _btnIcon;
	bool _bStart;
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

