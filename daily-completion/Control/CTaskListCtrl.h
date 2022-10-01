#pragma once
#include "../Base/DCController.h"

class UpdateGridEvent;
class CTaskListCtrl : public ControlUI::CTreeListCtrl, public DCController
{
public:
	CTaskListCtrl();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnUpdateGrid(std::shared_ptr<UpdateGridEvent> e);
	void OnSearchKeyChanged(const CString &strOld, const CString &strNew);
	afx_msg void OnDestroy();
private:
	std::function<void(const CString &, const CString &)> _OnSearchKeyChanged;
	std::function<void(std::shared_ptr<UpdateGridEvent> e)> _OnUpdateGrid;
};

