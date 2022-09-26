#pragma once
#include "ComboAnyOne.h"
#include <vector>

// 下拉框列表
class COMMONCONTROLS_API CCheckBoxList : public CTreeCtrl, public IPopupCtrl
{
public:
	CCheckBoxList();
	~CCheckBoxList();

	// 添加项，返回索引
	int AddItem(LPCTSTR szItem, DWORD_PTR extraData);

	// 返回项信息
	HTREEITEM GetItemAt(int nIndex);

	// 删除项信息
	void DeleteItem(int nIndex);

	// 删除所有项
	void DeleteAllItem();

	// 设置选中状态
	BOOL SetCheck(HTREEITEM hItem, BOOL fCheck = TRUE);

protected:

	// 查找项索引
	int GetIndex(HTREEITEM hItem);

	// 发送通知消息
	void SendNotify(UINT code, HTREEITEM item);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMDBClickTree(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	// 返回窗口
	virtual CWnd* GetWnd(CWnd *pOwner);

	// 返回需要在Combo上显示的内容
	virtual CString GetCtrlTextForShow();

	// 通知控件显示
	virtual void OnPreShowCtrl(LPCTSTR curComboTxt);

	// 返回控件不显示滚动条的尺寸
	virtual CSize GetBestSize();

protected:
	std::vector<HTREEITEM> m_arrTreeItems;	// 所有的子项
	std::vector<HTREEITEM> m_arrSelItems;	// 所有的选中子项
};

class COMMONCONTROLS_API CComboCheckBoxs : public CComboAnyOne
{
public:
	CComboCheckBoxs();
	~CComboCheckBoxs();

	// 添加项，返回索引
	int AddItem(LPCTSTR szItem, DWORD_PTR extraData = NULL);

	// 清空所有项
	void DeleteAllItem();

	// 设置某一项是否选中
	void SetItemSelectStatus(int nIndex, BOOL bSelected);

	// 返回指定项选中状态
	BOOL GetItemStatus(int nIndex);

	// 返回所有索引
	void GetSelectionIndexs(std::vector<int> &indexBuffer);

	// 返回所有选中数据
	void GetSelectionExtraDatas(std::vector<DWORD_PTR> &indexBuffer);

	void GetSeletionItemTexts(std::vector<CString> &selItemTexts);

	void SetSelectionItemText(CString strSelItemTexts);
};

