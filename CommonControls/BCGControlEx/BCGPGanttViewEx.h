// ReSharper disable CommentTypo
#pragma once


////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttView view

#include <BCGPGanttControl.h>
#include "BCGPGanttControlEx.h"
class CONTROLSAPI_EXPORT CBCGPGanttViewEx : public CView
{
	DECLARE_DYNCREATE(CBCGPGanttViewEx)

protected:
	CBCGPGanttViewEx();           // protected constructor used by dynamic creation

// Attributes
public:
	CBCGPGanttControlEx& GetControl()
	{
		return m_wndGanttControl;
	}
	const CBCGPGanttControlEx& GetControl() const
	{
		return m_wndGanttControl;
	}

	CBCGPGanttGrid* GetGrid()
	{
		return GetControl().GetGrid();
	}
	const CBCGPGanttGrid* GetGrid() const
	{
		return GetControl().GetGrid();
	}

	CBCGPGanttChart* GetChart()
	{
		return GetControl().GetChart();
	}
	const CBCGPGanttChart* GetChart() const
	{
		return GetControl().GetChart();
	}

	CBCGPSplitterWnd* GetSplitter()
	{
		return GetControl().GetSplitter();
	}
	const CBCGPSplitterWnd* GetSplitter() const
	{
		return GetControl().GetSplitter();
	}

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPGanttView)
protected:
	void OnDraw(CDC* pDC) override;      // overridden to draw this view
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// User-defined grid data 
	virtual ~CBCGPGanttViewEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual CBCGPGanttChartEx* OnCreateChart();
	virtual CBCGPGanttGridEx* OnCreateGrid();

protected:
	virtual BOOL OnChartClick(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnChartClickItem(UINT /*nFlags*/, CBCGPGanttItem* /*pItem*/) { return FALSE; }
	virtual BOOL OnChartClickHeader(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnChartDoubleClick(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnChartDoubleClickItem(UINT /*nFlags*/, CBCGPGanttItem* /*pItem*/) { return FALSE; }
	virtual BOOL OnChartDoubleClickHeader(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnChartItemMoving(BCGP_GANTT_ITEM_DRAGDROP* /*pDragDrop*/) { return TRUE; }
	virtual void OnChartItemChanged(CBCGPGanttItem* /*pItem*/, DWORD /*dwAction*/) { }
	virtual BOOL OnChartScaleChanging(BOOL /*bIncrease*/) { return FALSE; }
	virtual void OnChartScaleChanged() {}

	virtual void OnStorageChanged(const BCGP_GANTT_STORAGE_UPDATE_INFO& /*updateInfo*/) { }
	virtual void OnStorageConnectionAdded(const CBCGPGanttConnection& /*connection*/) { }
	virtual void OnStorageConnectionRemoved(const CBCGPGanttConnection& /*connection*/) { }

	virtual BOOL OnGridRowWriteData(const BCGP_GANTT_CONTROL_ROW& /*rowInfo*/) { return FALSE; }
	virtual BOOL OnGridRowReadData(const BCGP_GANTT_CONTROL_ROW& /*rowInfo*/) { return FALSE; }

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPGanttView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnNotifyCreateChart(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyCreateGrid(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartClick(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartClickItem(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartClickHeader(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartDblClick(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartDblClickItem(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartDblClickHeader(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartItemMoving(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartItemsChanged(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartScaleChanging(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyChartScaleChanged(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyStorageChanged(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyStorageConnectionAdded(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyStorageConnectionRemoved(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyReadDataFromGrid(WPARAM, LPARAM);
	afx_msg LRESULT OnNotifyWriteDataToGrid(WPARAM, LPARAM);
	afx_msg LRESULT OnChangeVisualManager(WPARAM wp, LPARAM lp);
	
	DECLARE_MESSAGE_MAP()

protected:
	CBCGPGanttControlEx       m_wndGanttControl;
};