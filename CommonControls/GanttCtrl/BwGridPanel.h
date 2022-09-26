//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           BwGridPanel.h
//    @author         yf
//    @data           2021/01/20  
//    @brief          Grid对话框
//
//*****************************************************

#pragma once

#include "BwGanttGrid.h"
#include <vector>

COMMONCONTROLS_API extern UINT BW_GRIDDLG_EXPORT;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_IMPORT;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_BEFOREDELETE;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_DELETE;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_BIND;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_NEW;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_MOVE;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_SELCHANGE;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_ENDEDIT;
COMMONCONTROLS_API extern UINT BW_GRIDDLG_ENDEDITDONE;

class CBwGanttControl;
class CBwGanttChart;

class CBwGridPanel : public CWnd
{
	DECLARE_DYNCREATE(CBwGridPanel)
public:
	// 任务节点信息
	struct stTaskInfo
	{
		CString _strName;
		COleDateTime _planStartTime;
		COleDateTime _planEndTime;
		COleDateTime _realStartTime;
		COleDateTime _realEndTime;
	};

	CBwGridPanel();           // 动态创建所使用的受保护的构造函数
	virtual ~CBwGridPanel();
public:
	// 创建窗口
	void Create(CWnd *pParent, LPRECT rt, UINT uId = 0);

	// 设置Gantt控制器
	void SetControl(CBwGanttControl *pControl);

	// 获取Gantt控制器
	CBwGanttControl *GetControl();
	
	// 获取grid控件
	CBwGanttGrid *GetGrid();

	// 设置KNode根节点
	void SetRoot(KNode *pRoot);

	// 获取KNode根节点
	KNode * GetRoot();

	// 名称是否存在
	bool IsNameExist(const CString &strCurName, KNode *pNode);

	// 从业务对象加载数据到表格
	void LoadFromNode(bool bRedraw = true);

	// 加载业务对象并查找节点下标
	int LoadAndFindIndex(KNode *pFind, bool bRedraw = true);

	// 获取按钮区域矩形高度
	int GetBtnAreaHeight();

	// 启用按钮
	void EnableButtons(bool bEnable = true);

	// 获取日期范围
	void GetDateRange(COleDateTime &startTime, COleDateTime &endTime);

	// 获取选中下标
	int GetSelIndex();

	// 获取选中的多个下标
	std::set<int> GetSelIndexes();

	// 设置图标列表
	void SetImageList(CImageList *pImageList);

	// 显示工具条
	void ShowToolbar(bool bShow);

	// 获取生长方式字符串
	CString GetGrowthPatternString(int nGrowthPattern);

	// 选中行
	void SelectRow(int nIndex);
	void SelectRow();

	// 结束编辑
	bool OnEndEditGrid(CBCGPGridItem * pGridItem, KNode *pNode, const CBwGanttGrid::ColumnFormat &colFormat);
	void OnEndEditGridDone(CBCGPGridItem * pGridItem, KNode *pNode, const CBwGanttGrid::ColumnFormat &colFormat);

	// CBwGridPanel 诊断

#ifdef _DEBUG
	void AssertValid() const;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const;
#endif
#endif //_DEBUG
protected:
	DECLARE_MESSAGE_MAP()

	// 继承于基类的方法
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	LRESULT OnClickGrid(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// 创建新的任务节点
	KNode *CreateNewTaskNode(const stTaskInfo & taskInfo);

	void OnButtonClicked(UINT uId);

	// 创建新任务
	KNode * CreateNewTask();

	// 创建新的子任务
	KNode *CreateNewChildTask();

	// 绑定
	void Bind();

	// 删除当前任务
	void DeleteCurTask();

	// 上移
	void MoveUp();

	// 下移
	void MoveDown();

	// 移动当前任务
	void MoveCurTask(bool bUp = true);

	// 设置当前日期
	void SetCurTime(const COleDateTime &curTime);

	// 导出
	void Export();

	// 导入
	void Import();

	// 加载 
	void Load();

	// 创建Grid控件
	void CreateGrid();

	// 创建工具栏
	void CreateToolBar();

	// 获取Chart控件
	CBwGanttChart *GetChart();

	// 创建任务名称
	CString CreateTaskName();

	// 获取生长方式类型标签
	int GetGrowthPatternTag(const CString &strGrowthPattern);

	// 更新进度持续时间
	void UpdateDuringTime();

	// CString转日期
	COleDateTime CString2Datetime(CString strDate);

	// 日期转CString
	CString Datetime2CString(COleDateTime dateTime);
	// 获取默认Task数据
	stTaskInfo GetDefaultTaskInfo();

	// 从业务对象加载数据到表格(返回值为加载的节点个数)
	int LoadFromNode(KNode *pRoot, KNode *pFind = NULL, int *pFindIndex = NULL, int nStartRow = 0, int nLevel = 0);

	// 获取子层级的数量
	int GetAllSubItemCount(CBCGPGridRow *pRow);

	// 获取日期范围
	void GetDateRange(COleDateTime &startTime, COleDateTime &endTime, KNode *pParent);

	// 调整滚动条
	void AjustVScroll();
	
	// 是否可以创建新的子级任务
	bool CanCreateNewChildTask();

protected:
	KNode *_pRoot; // 进度根节点
	CBwGanttGrid *_pGrid; // grid控件对象指针
	CBwGanttChart *_pChart; // 甘特图Chart对象指针
	CBwGanttControl * _pControl; // 甘特图控制器对象指针
	CFont _font; // 字体对象指针（需要持久存储，不然没有效果）

	CButton _btnNew; // 新建按钮
	CButton _btnNewChild; // 新建子集
	CButton _btnDelete; // 删除按钮
	CButton _btnBind; // 绑定按钮
	CButton _btnImport; // 导入按钮
	CButton _btnExport; // 导出按钮
	CButton _btnUp; // 上移按钮
	CButton _btnDown; // 下移按钮

	std::vector<shared_ptr<TBBUTTON>> _arrBtns;

	static CString s_ClassName;	// 窗口类名
	HWND m_NotifyWnd; // 通知窗口
	CToolTipCtrl m_TipCtrl; // 提示信息
	COLORREF m_crTabContainerColor; // Tab区域填充颜色

	CToolBarCtrl _toolBar;

	CImageList *_pImageList; // 图标列表
	bool _bShowToolBar; // 是否显示工具条

	int _nSelIndex; // 当前选择行

	int _oldSel; // 上一次选择行
	int _oldPos; // 上一次垂直滚动条位置
};