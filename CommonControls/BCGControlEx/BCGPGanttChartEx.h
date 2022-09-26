#pragma once
#include <BCGPGanttRenderer.h>
#include <BCGControlBar\BCGPGanttChart.h>

class CBCGPGanttRendererEx : public CBCGPGanttRenderer
{
	DECLARE_DYNAMIC(CBCGPGanttRendererEx)

public:
	CBCGPGanttRendererEx();
	virtual ~CBCGPGanttRendererEx();

	virtual void DrawBar(CBCGPGanttDrawContext& ctxDraw);
	virtual void DrawSelection(CBCGPGanttDrawContext& ctxDraw);

protected:
	void DrawItemName(CBCGPGanttDrawContext& ctxDraw);
};

static UINT BCGM_GANTT_CHART_ITEM_RESIZED = ::RegisterWindowMessage(_T("BCGM_GANTT_CHART_ITEM_RESIZED"));

class CONTROLSAPI_EXPORT CBCGPGanttChartEx : public CBCGPGanttChart
{
	friend class CBCGPGanttItemStorageBase;
	friend class CBCGPGanttControlEx;
	struct UPDATE_INFO;

protected:
	virtual CWnd* GetNotificationWindow() const;
	// Returns an item renderer. Called if m_pRenderer is NULL.
	virtual CBCGPGanttRenderer* QueryRenderer();
	virtual BOOL QueryToolTipText(const CBCGPGanttItem* pItem, CString& strText, CString& strDescription) const;
	virtual void MoveSelectedItems(COleDateTimeSpan dtOffset);
	virtual void ResizeSelectedItems(COleDateTimeSpan dtOffset);
	virtual BOOL DoChartClick(UINT nFlags, CPoint point);

	DECLARE_DYNCREATE(CBCGPGanttChartEx)

public:
	CBCGPGanttChartEx();
	~CBCGPGanttChartEx();
	void CustomTooltip(bool bCustom);
	void SetToolTipTitle(const CBCGPGanttItem* pItem, CString strTitle);
	void SetToolTipContex(const CBCGPGanttItem* pItem, CString strContext);
	void SetChartColor(COLORREF cf);
	//BCGP_GANTT_CHART_COLORS     m_UserColors;

private:
	bool _bShowCustomTooltip;
	CString _strTitle;
	CString _strContext;
protected:
	afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMH, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
