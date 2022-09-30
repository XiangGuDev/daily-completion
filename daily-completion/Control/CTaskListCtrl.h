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
	afx_msg void OnDestroy();
};

