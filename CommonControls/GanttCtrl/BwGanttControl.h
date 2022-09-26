//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           BwGanttControl.h
//    @author         yf
//    @data           2020/12/16  
//    @brief          甘特图控件
//
//*****************************************************

#pragma once

#include "BCGPGanttControl.h"
#include "BwSplitCtrl.h"
#include <map>
#include <set>
#include "BwGridPanel.h"
#include "BwGanttChart.h"

//------------------------------------------------------

class COMMONCONTROLS_API CBwGanttControl : public CBCGPGanttControl
{
	friend class CBwGanttGrid;
	friend class CBwGanttChart;
public:
	CBwGanttControl();
	virtual ~CBwGanttControl();

	// 插入结果信息
	struct BW_GANTT_INSERT_RESULT
	{
		BW_GANTT_INSERT_RESULT()
			:_nRow(-1)
			,_bRes(false)
		{

		}

		int _nRow;
		bool _bRes;
	};

	// 删除结果信息
	struct BW_GANTT_DELETE_RESULT
	{
		BW_GANTT_DELETE_RESULT()
			:_nRow(-1)
			, _bRes(false)
		{

		}

		int _nRow;
		bool _bRes;
	};

public:
	// 创建
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 创建控件
	virtual int  CreateControls();

	// 获取分隔条
	CBwSplitCtrl*       GetSplitter(); 

	// 更新grid控件
	virtual void UpdateGridEx(bool bRedraw = true);

	// 设置当前日期垂直线颜色
	void SetCurTimeLineColor(COLORREF clr);

	// 设置当前日期
	void SetCurTime(const COleDateTime &curTime);

	// 设置根节点
	void SetRoot(KNode *pRoot);

	// 从业务对象加载数据到表格
	void LoadFromNode();

	// 启用按钮
	void EnableButtons(bool bEnable = true);

	// 获取日期范围
	void GetDateRange(COleDateTime &startTime, COleDateTime &endTime);

	// 设置工具栏图标列表
	void SetToolBarImageList(CImageList *pImageList);

	// 显示工具条
	void ShowToolbar(bool bShow = true);

	// 设置滑动条宽度
	void SetSplitterWidth(int nWidth);

	// 设置滑动条位置
	void SetSplitterPosPct(int nPos);

	// 设置单行进度条(bSingle为true表示单行，为false表示双行）
	void SetSingleProgressBar(bool bSingle = true);
	
	// 设置时间度量类型
	void SetTimeDimension(CBwGanttChart::ETimeDimension td);

	// 获取时间度量类型
	CBwGanttChart::ETimeDimension GetTimeDimension();

	// 更新滚动条位置
	void UpdateScrollPos();

	// 插入KNode节点(bAddChild表示是否插入子级)
	BW_GANTT_INSERT_RESULT InsertNode(KNode *pNode, bool bAddChild = false);

	// 删除KNode节点
	BW_GANTT_DELETE_RESULT DeleteNode(KNode *pNode);

	// 更新Grid数据
	void UpdateGridItem(KNode *pNode);

	// 通过KNode获取GanttItem
	CBwGanttChart::CBwGanttItemInfo GetGanttItemByKNode(KNode *pNode);

	// 通过KNode创建GanttItem
	CBwGanttItem *CreateGanttItemByKNode(KNode *pParent, KNode *pNode);
	
	// 改变所有节点的编辑状态
	void ModifyAllRowEditState();

	// 使组节点行可编辑
	void ModifyRowEditState(CBCGPGridRow* pRow);

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	// 消息函数
	afx_msg LRESULT OnNotifyStorageChanged(WPARAM, LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void DoItemsChange     (CWnd* pSender, const BCGP_GANTT_STORAGE_UPDATE_INFO& updateInfo);
	virtual CBwGanttGrid* CreateGanttGrid();
	virtual CBwGanttChart* CreateGanttChart();

	// 更新Grid数据
	void UpdateGridItem(CBCGPGanttItem* pItem, CBCGPGridRow* pRow);

	// 响应Grid数据更改
	virtual void OnGridItemChanged(CBCGPGridItem* pGridItem, int nRow, int nColumn);
	
	// 响应垂直修改尺寸
	virtual void DoVerticalResize(CWnd* pSender);

	// 调整Chart列头
	void AdjustChartHeadHeight();
protected:
	CBwSplitCtrl m_wndSplitter; // 分隔条控件
	std::set<int *> _arrCols; // 列下标map
	CBwGridPanel _gridPanel; // Grid控件对话框

	CBwGanttChart::ETimeDimension _eTimeDimension; // 时间度量类型
};


