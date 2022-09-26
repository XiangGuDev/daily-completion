#pragma once

//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2019 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPGanttControl.h: interface for the CBCGPGanttControl class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPWnd.h"
#include "BCGPGanttGrid.h"

#include <BCGCBPro.h>
#include "BCGPGanttChart.h"
#include "BCGPGanttGridEx.h"
#include "BCGPGanttChartEx.h"
#include "BCGPSplitterWnd.h"
#include <BCGPGanttControl.h>

static UINT BCGM_GRID_ITEM_CHANDE = ::RegisterWindowMessage(_T("BCGM_GRID_ITEM_CHANDE"));
class CONTROLSAPI_EXPORT CBCGPGanttControlEx : public CBCGPGanttControl
{
	friend class CBCGPGanttGridEx;
	friend class CBCGPGanttChartEx;
	friend class CBCGPGanttViewEx;
	friend class CBCGPGanttGridRowEx;

	DECLARE_DYNCREATE(CBCGPGanttControlEx)

public:
	CBCGPGanttControlEx();
	virtual ~CBCGPGanttControlEx();

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual int  CreateControls();
	virtual CBCGPGanttGrid* CreateGrid();
	virtual CBCGPGanttChart* CreateChart();
	virtual void OnGridItemChanged(CBCGPGridItem* pGridItem, int nRow, int nColumn);
	virtual void UpdateGridEx();

public:

protected:
	//virtual void DoItemsChange(CWnd* pSender, const BCGP_GANTT_STORAGE_UPDATE_INFO& updateInfo);
	DECLARE_MESSAGE_MAP()
};