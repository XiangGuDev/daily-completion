#include "pch.h"
#include "../Resource.h"
#include "CTaskListCtrl.h"
#include "../System/TaskSystem.h"
#include "../Model/Task.h"
#include "../Adapters/TaskAdapter.h"

using namespace ControlUI;

BEGIN_MESSAGE_MAP(CTaskListCtrl, CTreeListCtrl)
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CTaskListCtrl::CTaskListCtrl()
{
	RegisterEvent<UpdateGridEvent>(std::bind(&CTaskListCtrl::OnUpdateGrid, this, std::placeholders::_1));
}

void CTaskListCtrl::OnDestroy()
{
	__super::OnDestroy();
	UnRegisterEvent<UpdateGridEvent>(std::bind(&CTaskListCtrl::OnUpdateGrid, this, std::placeholders::_1));
}

void CTaskListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint curPt;
	GetCursorPos(&curPt);
	CMenu rightMenu;
	if (!rightMenu.LoadMenu(IDR_MENU))
		return;

	CMenu *pSubMenu = rightMenu.GetSubMenu(1);
	if (pSubMenu == NULL)
		return;

	UINT nCmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
		curPt.x, curPt.y, this);
	if (nCmd == IDC_NEW)
	{
		auto task = std::make_shared<Task>();
		task->bComplete = false;
		task->strName = L"新名字";
		GetSystem<CTaskSystem>()->AddTask(task);
		OnUpdateGrid(std::make_shared<UpdateGridEvent>());
	}
	else if (nCmd == IDC_DEL)
	{
	}
	CTreeListCtrl::OnRButtonUp(nFlags, point);
}

void CTaskListCtrl::OnUpdateGrid(std::shared_ptr<UpdateGridEvent> e)
{
	SetDataSource(new CTaskAdapter(NULL, true), TRUE);
	UpdateList(TRUE);
}



