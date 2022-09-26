//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           BwGanttControl.h
//    @author         yf
//    @data           2020/12/16  
//    @brief          甘特图Chart控件
//
//*****************************************************

#pragma once

#include "BCGPGanttChart.h"
#include "BwGanttRenderer.h"

struct stUpdateInfo
{
	int                 nHScrollDelta;   // Differs from 0 if horizontal scrolling occurred
	int                 nVScrollDelta;   // Differs from 0 if horizontal scrolling occurred

	BOOL                bItemsLayoutChanged; // If TRUE, scroll deltas are ignored.

	BOOL                bLayoutChanged; // Occurs when control has been resized, header positions changed etc.

	BOOL                bAppearanceChanged; // Fonts, Colors, Grid lines, Visual style etc.

	BOOL                bConnectionsChanged;

	stUpdateInfo()
		: nHScrollDelta(0)
		, nVScrollDelta(0)
		, bItemsLayoutChanged(TRUE)
		, bLayoutChanged(TRUE)
		, bAppearanceChanged(TRUE)
		, bConnectionsChanged(TRUE)
	{
	}

	bool NeedsUpdate() const
	{
		return  nHScrollDelta != 0 ||
			nVScrollDelta != 0 ||
			bItemsLayoutChanged ||
			bLayoutChanged ||
			bAppearanceChanged ||
			bConnectionsChanged;
	}

	void Overwrite(const stUpdateInfo& info)
	{
		nHScrollDelta += info.nHScrollDelta;
		nVScrollDelta += info.nVScrollDelta;
		bItemsLayoutChanged = bItemsLayoutChanged || info.bItemsLayoutChanged;
		bLayoutChanged = bLayoutChanged || info.bLayoutChanged;
		bAppearanceChanged = bAppearanceChanged || info.bAppearanceChanged;
		bConnectionsChanged = bConnectionsChanged || info.bConnectionsChanged;
	}

	void Reset()
	{
		nHScrollDelta = 0;
		nVScrollDelta = 0;
		bItemsLayoutChanged = FALSE;
		bLayoutChanged = FALSE;
		bAppearanceChanged = FALSE;
		bConnectionsChanged = FALSE;
	}
};

struct BW_GANTT_CHART_HEADER_CELL_INFO
{
	CRect			  rectCell;			    // cell area (in header)
	CRect			  rectColumn;		    // column area. Defines visible chart area for this column.
	CRect			  rectClip;			    // clipping rectangle
	COleDateTime	  dtCellLeftMostTime;	// starting time for cell
	COleDateTimeSpan  dtCellTimeDelta;
	const BCGP_GANTT_CHART_HEADER*	  pHeaderInfo;

	BW_GANTT_CHART_HEADER_CELL_INFO();

	bool operator == (const BW_GANTT_CHART_HEADER_CELL_INFO& rhs) const;
	bool operator != (const BW_GANTT_CHART_HEADER_CELL_INFO& rhs) const;

	bool Exists() const;
	void Reset();
};

class CBwGanttControl;

class COMMONCONTROLS_API CBwGanttChart : public CBCGPGanttChart
{
public:
	// 刻度类型
	enum ETimeDimension
	{
		eDate, // 日期刻度
		eValue, // 值刻度
	};

	// 甘特图项信息结构体
	struct CBwGanttItemInfo
	{
		CBwGanttItemInfo()
			: _pGanttItem(NULL)
			, _nIndex(-1)
		{

		}

		bool IsExist()
		{
			return _pGanttItem && _nIndex > -1;
		}

		CBwGanttItem *_pGanttItem;
		int _nIndex;
	};
public:
	CBwGanttChart();
	virtual ~CBwGanttChart();
	CBwGanttRenderer* GetRenderer();
	virtual void PerformAdjustLayout();

	// 设置当前日期
	void SetCurTime(const COleDateTime &curTime);
	// 设置当前日期垂直线颜色
	void SetCurTimeLineColor(COLORREF clr);
	// 设置Gantt控制器
	void SetControl(CBwGanttControl *pControl);
	// 设置单行进度条(bSingle为true表示单行，为false表示双行）
	void SetSingleProgressBar(bool bSingle = true);
	// 设置水平滚动条位置
	void SetHSrollPos(const COleDateTime& time);
	// 获取选中项
	CBwGanttItem *GetSelItem();
	// 获取选中下标
	int GetSelIndex();

public:
	// 继承基类方法
	afx_msg void OnPaint();
	BOOL HeaderCellFromPoint(CPoint pt, BW_GANTT_CHART_HEADER_CELL_INFO& colInfo) const;
public:
	DECLARE_MESSAGE_MAP()

	struct stRowLayout
	{
		CBCGPGanttItem* pItem;
		int             nRowTopY;
		int             nRowBottomY;
		CRect			rectBar;
		CRect			rectRealBar;

		stRowLayout() : pItem(NULL)
			, nRowTopY(0)
			, nRowBottomY(0)
			, rectBar(0, 0, 0, 0)
		{
		}
	};
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnStorageChanged(WPARAM, LPARAM);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMH, LRESULT* pResult);
protected:
	CBwGanttControl* GetGanttControl();
	virtual void DoPaint(CDC& dc);
	virtual void SetupItemDrawContext(CBwGanttDrawContext& ctx) const;
	void UpdateItemsLayout();
	void RecalculateItemsTimeRange();
	virtual void UpdateHeaders();
	virtual void DrawItemBackground(CDC& dc, const CRect& rectItem, const CBCGPGanttItem* pItem) const;
	virtual void DrawHeader(CDC& dc, const BCGP_GANTT_CHART_HEADER& header, const CRect& rcPaint) const;
	virtual void DrawHeaderCell(CDC& dc, const BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL bHilite) const;
	virtual void DrawHeaderText(CDC& dc, const BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo, const CString& sCellText, BOOL bHilite) const;
protected:
	CBwGanttRenderer*         m_pRenderer; // 渲染器
	CArray<stRowLayout>	m_arrItemRects; // 行矩形
	COleDateTime _curTime; // 当前日期
	COLORREF _clrCurTime; // 当前日期垂直线颜色
	std::vector<CString> _arrWeekDays;
	CBwGanttControl * _pControl; // 甘特图控制器对象指针
	bool _bSingleProgressBar; // 是否为单行进度
};

