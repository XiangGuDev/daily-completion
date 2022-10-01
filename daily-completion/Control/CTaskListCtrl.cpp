#include "pch.h"
#include "../Resource.h"
#include "CTaskListCtrl.h"
#include "../System/TaskSystem.h"
#include "../Model/Task.h"
#include "../Adapters/TaskAdapter.h"
#include "../Model/GlobalModel.h"
#include "../Model/UpdateGridEvent.h"

using namespace ControlUI;

BEGIN_MESSAGE_MAP(CTaskListCtrl, CTreeListCtrl)
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CTaskListCtrl::CTaskListCtrl()
{
	_bkColor = RGB(240, 240, 240);
	_OnSearchKeyChanged = std::bind(&CTaskListCtrl::OnSearchKeyChanged, this, std::placeholders::_1, std::placeholders::_2);
	_OnUpdateGrid = std::bind(&CTaskListCtrl::OnUpdateGrid, this, std::placeholders::_1);

}

int CTaskListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	_imageList.Create(16, 16, ILC_COLOR32, 0, 4);

	RegisterEvent<UpdateGridEvent>(_OnUpdateGrid);
	GetModel<CGlobalModel>()->SearchKey.RegisterChangedEvent(_OnSearchKeyChanged);

	return 0;
}


void CTaskListCtrl::OnDestroy()
{
	__super::OnDestroy();
	_imageList.DeleteImageList();

	UnRegisterEvent<UpdateGridEvent>(_OnUpdateGrid);
	GetModel<CGlobalModel>()->SearchKey.UnRegisterChangedEvent(_OnSearchKeyChanged);
}

void CTaskListCtrl::LoadIcon(const std::vector<CString>& pathList)
{
	HICON icon = nullptr;
	for (auto &iconPath : pathList)
	{
		icon = (HICON)::LoadImage(AfxGetResourceHandle(), iconPath, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (!icon) continue;
		_imageList.Add(icon);
		DestroyIcon(icon);
	}
	SetImageList(&_imageList, false);
}

void CTaskListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// 处理点击空白处的右键菜单弹出
	auto curAdapt = static_cast<CTaskAdapter *>(GetSelectedItem());
	bool bComplete = false;
	bool bFixed = false;
	std::shared_ptr<Task> task;
	if (curAdapt)
	{
		task = curAdapt->GetTask();
		bFixed = task->bFixed;
		bComplete = task->bComplete;
	}

	CPoint curPt;
	GetCursorPos(&curPt);
	CMenu rightMenu;
	if (!rightMenu.LoadMenu(IDR_MENU))
		return;

	CMenu *pSubMenu = rightMenu.GetSubMenu(1);
	if (pSubMenu == NULL)
		return;
	if (_hHoverRow == NULL || task == NULL)
	{
		pSubMenu->RemoveMenu(IDC_DEL, MF_BYCOMMAND);
		pSubMenu->RemoveMenu(IDC_COMPLETE, MF_BYCOMMAND);
		pSubMenu->RemoveMenu(IDC_REDO, MF_BYCOMMAND);
		pSubMenu->RemoveMenu(IDC_FIXED, MF_BYCOMMAND);
		pSubMenu->RemoveMenu(IDC_UNFIXED, MF_BYCOMMAND);
	}
	else
	{
		if (bComplete)
		{
			pSubMenu->RemoveMenu(IDC_COMPLETE, MF_BYCOMMAND);
		}
		else
		{
			pSubMenu->RemoveMenu(IDC_REDO, MF_BYCOMMAND);
		}
		if (bFixed)
		{
			pSubMenu->RemoveMenu(IDC_FIXED, MF_BYCOMMAND);
		}
		else
		{
			pSubMenu->RemoveMenu(IDC_UNFIXED, MF_BYCOMMAND);
		}
	}

	UINT nCmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
		curPt.x, curPt.y, this);
	if (nCmd == IDC_NEW)
	{
		GetSystem<CTaskSystem>()->AddTask();
		OnUpdateGrid(std::make_shared<UpdateGridEvent>());
	}
	else if (nCmd == IDC_DEL)
	{
		GetSystem<CTaskSystem>()->RemoveTask(task);
	}
	else if (nCmd == IDC_REDO)
	{
		task->bComplete = false;
		OnUpdateGrid(std::make_shared<UpdateGridEvent>());
		GetSystem<CTaskSystem>()->Save();
	}
	else if (nCmd == IDC_COMPLETE)
	{
		task->bComplete = true;
		OnUpdateGrid(std::make_shared<UpdateGridEvent>());
		GetSystem<CTaskSystem>()->Save();
	}
	else if (nCmd == IDC_FIXED)
	{
		task->bFixed = true;
		OnUpdateGrid(std::make_shared<UpdateGridEvent>());
		GetSystem<CTaskSystem>()->Save();
	}
	else if (nCmd == IDC_UNFIXED)
	{
		task->bFixed = false;
		OnUpdateGrid(std::make_shared<UpdateGridEvent>());
		GetSystem<CTaskSystem>()->Save();
	}
	CTreeListCtrl::OnRButtonUp(nFlags, point);
}

void CTaskListCtrl::OnUpdateGrid(std::shared_ptr<UpdateGridEvent> e)
{
	SetDataSource(new CTaskAdapter(NULL, true), TRUE);
	UpdateList(TRUE);
}

void CTaskListCtrl::OnSearchKeyChanged(const CString & strOld, const CString & strNew)
{
	SetDataSource(new CTaskAdapter(NULL, true), TRUE);
	UpdateList(TRUE);
}

