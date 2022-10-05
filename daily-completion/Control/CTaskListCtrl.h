#pragma once
#include "../Base/DCController.h"

class UpdateGridEvent;
class CTaskListCtrl : public ControlUI::CTreeListCtrl, public DCController
{
public:
	CTaskListCtrl();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnUpdateGrid(std::shared_ptr<UpdateGridEvent> e);
	void OnSearchKeyChanged(const CString &strOld, const CString &strNew);
	afx_msg void OnDestroy();
	void LoadIcon(const std::vector<CString> &pathList);
private:
	std::function<void(const CString &, const CString &)> _OnSearchKeyChanged;
	std::function<void(std::shared_ptr<UpdateGridEvent> e)> _OnUpdateGrid;
private:
	CImageList _imageList; // Õº±Í¡–±Ì
};

