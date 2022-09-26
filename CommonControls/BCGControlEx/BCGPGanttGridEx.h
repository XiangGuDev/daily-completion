#pragma once

////////////////////////////////////////////////////////////////////////////\
// CBCGPGridDateTimeItem object

class CONTROLSAPI_EXPORT CBCGPGanttFirstItem : public CBCGPGridItem
{
	//DECLARE_DYNAMIC(CBCGPGridItemEx)
	DECLARE_SERIAL(CBCGPGanttFirstItem)
// Construction
protected:
	CBCGPGanttFirstItem();

public:
	CBCGPGanttFirstItem(const _variant_t& varValue, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL,
		LPCTSTR lpszValidChars = NULL);

	virtual ~CBCGPGanttFirstItem();

// Overrides
public:
	virtual void OnDrawValue(CDC* pDC, CRect rect);

// 扩展保护属性
	void SetRect(CRect rect);
	
	// 左侧空白缩进
	void SetLeftRect(CRect rect);
	CRect GetLeftRect();
	
	// 绘制下拉框
	void DrawExpandBox(CDC* pDC);

	// 调整编辑矩形
	virtual void AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin);
// Attributes
public:

protected:
	CRect _leftRect;
};

//////////////////////////////////////////////////////////////////////////////////////
class CONTROLSAPI_EXPORT CBCGPGanttGridRowEx : public CBCGPGridRow
{
	friend class CBCGPGanttControlEx;
	friend class CBCGPGridCtrlEx;
	DECLARE_DYNAMIC(CBCGPGanttGridRowEx)

public:
	// Simple item
	CBCGPGanttGridRowEx(int nColumns = 0)
		: CBCGPGridRow(nColumns)
	{
	}

	// Group constructor
	CBCGPGanttGridRowEx(const CString& strGroupName)
		: CBCGPGridRow(strGroupName)
	{
	}

	virtual ~CBCGPGanttGridRowEx()
	{
	}

public:
	// 绘制首个Item
	virtual void DrawFirstItemValue(CDC* pDC, CRect rect, BOOL bGroupUnderline);
	virtual void OnExpand(BOOL bExpand);

	void SwapSubItems(CBCGPGridRow* pRow1, CBCGPGridRow* pRow2);
	bool HasChild();
};

////////////////////////////////////////////////////////////////////////////////////////\

static UINT BCGM_GRID_ITEM_DRAG = ::RegisterWindowMessage(_T("BCGM_GRID_ITEM_DRAG"));
class CONTROLSAPI_EXPORT CBCGPGanttGridEx : public CBCGPGanttGrid
{
	friend class CBCGPGanttControlEx;
	friend class CBCGPGanttGridRowEx;
	//friend class CBCGPGridRow;
	DECLARE_DYNCREATE(CBCGPGanttGridEx)

public:
	CBCGPGanttGridEx();
	virtual ~CBCGPGanttGridEx();

	// Overrides
public:
	// 创建扩展的CBCGPGridRowEx
	virtual CBCGPGridRow* CreateRow();
 	virtual CBCGPGridRow* CreateRow(CString strName);
	virtual CBCGPGridRow* CreateRow(int nColumns);
	virtual void InsertRowAfter(CBCGPGridRow* pParentRow, int nSubPos, CBCGPGridRow* pSubItem);
	virtual BOOL OnDrawItem(CDC* pDC, CBCGPGridRow* pItem);
	// 设置ExpandBox 绘制位置
	virtual int GetExtraHierarchyOffset() const;
	virtual int GetHierarchyLevelOffset() const;

protected:
	virtual BOOL OnTrackHeader()
	{
		return FALSE; // do not resize columns inside client area by mouse, only at header
	}

public:
	// 设置树控件的首行 - 暂未实现
	int GetTreeColumn();
	void SetTreeColumn(int nColumn);
	
	CBCGPGridRow* GetLastSelRow();
	// 设置字体大小
	void SetFontSize(int nSize);
	void SetFontSizeScale(float fScale);

	// 设置有子项的行字体加粗
	void SetBoldFontByHasChild(bool bBold = true);
	bool GetBoldFontByHasChild();

	// 设置拖拽
	void SetDragDropRows();

	// 扩展	CBCGPGridCtrl 中的保护方法或属性
	DWORD GetMarkedIndexes() { return m_nMarkedIndexes;}
	int GetcxCurrText() { return m_cxCurrText; }

	virtual void OnRowHeaderDblClick(CBCGPGridRow* pRow, CPoint point, CRect rect);
	virtual CRect OnGetSelectionRect();
	virtual void OnDrawSelectionBorder(CDC* pDC);

	virtual BOOL DragItems(CPoint point);
	virtual BOOL DoDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL DoDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	/**
	* 结束编辑
	* @param [in] pItem
	*/
	virtual void OnEndInplaceEdit(CBCGPGridItem* pItem);

	virtual BOOL EditItem(CBCGPGridRow* pItem, LPPOINT lptClick = NULL);

	/**
	* 设置可以退出编辑状态的响应字符
	* @param [in] nChar 指定响应字符
	* @param [in] nRepCnt 
	* @param [in] nFlags  
	*/
	virtual BOOL CanEndInplaceEditOnChar(UINT nChar, UINT nRepCnt, UINT nFlags) const;

	/**
	* 窗口的消息
	* @param [in] message 指定消息类型。
	* @param [in] wParam  指定其余的、消息特定的信息。该参数的内容与message参数值有关。
	* @param [in] IParam  指定其余的、消息特定的信息。该参数的内容与message参数值有关。
	* 返回值：返回值就是消息处理结果，它与发送的消息有关。
	*/
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	/**
	*  在:PreTranslateMessage中调用，当资源item处于编辑状态时点击下拉框，将资源item中的值设置为原内容
	*/
	void UpdateInplaceWnd();

protected:
	int _bBoldFontByHasChild;

protected:
	//{{AFX_MSG(CBCGPGanttGrid)
	//}}AFX_MSG
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnEndLabelEdit(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private:
	int m_nIMECharSize;
	int m_nIMECharLeft;
};

