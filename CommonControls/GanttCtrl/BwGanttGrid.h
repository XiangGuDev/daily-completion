//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           BwGanttGrid.h
//    @author         yf
//    @data           2020/12/16  
//    @brief          甘特图Grid控件
//
//*****************************************************

#pragma once

#include "BCGPGanttGrid.h"
#include <vector>
#define  WM_USER_SORTCOLUM    WM_USER + 901

class COMMONCONTROLS_API CBwGrowthPatternItem : public CBCGPGridItem
{
	DECLARE_DYNCREATE(CBwGrowthPatternItem)
public:
	CBwGrowthPatternItem();
public:
	virtual void OnDrawValue(CDC* pDC, CRect rect);

	// 冻结该项(禁用）
	void FreezeItem(bool bFreeze = true);
private:
	COLORREF _clrTextOld;
};

class COMMONCONTROLS_API CNoEditItem : public CBCGPGridItem
{
	DECLARE_DYNCREATE(CNoEditItem)
public:
	CNoEditItem();
};

class COMMONCONTROLS_API CBwDateTimeItem : public CBCGPGridDateTimeItem
{
	DECLARE_DYNCREATE(CBwDateTimeItem)
public:
	CBwDateTimeItem();
	virtual CString FormatItem();
	virtual BOOL OnUpdateValue();
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
};

class CBwGanttControl;
class CBwGridPanel;

class COMMONCONTROLS_API CBwGanttGrid : public CBCGPGanttGridEx
{
public:
	friend class CBwGridPanel;
public:
	enum EDataType
	{
		eString,
		eDate,
		eBool,
		eInt,
		eDouble,
	};

	///列定义
	struct ColumnFormat
	{
	public:
		ColumnFormat(const CString& strName, const int width,
			const EDataType editInPlaceFlags = eString,
			CRuntimeClass *runtime = nullptr,
			const CString& dataFieldName = L"",
			const _variant_t& default = _variant_t(),
			const std::vector<CString>& optionValues = {})
			: _strName(strName)
			, _width(width)
			, _editInPlaceFlags(editInPlaceFlags)
			, _runtime(runtime)
			, _dataFieldName(dataFieldName)
			, _default(default)
			, _OptionValues(optionValues)
		{
			if (_dataFieldName.IsEmpty())
			{
				// 设置数据域属性名
				_dataFieldName = strName;
			}
		}
		CString			_strName;			//名称
		int				_width;				//宽度
		EDataType				_editInPlaceFlags;	//编辑标记
		CRuntimeClass	*_runtime;			//单元格扩展运行时类
		CString			_dataFieldName;		//数据域名
		_variant_t		_default;			//默认值
		std::vector<CString> _OptionValues;  //可选择值集合，指定的列是下拉框(或其他需要选择的控件)的时候将会使用
	};
	//列合并定义
	struct ColumnMergeFormat
	{
	public:
		ColumnMergeFormat(const CString& strName, const int nLeft, const int nRight, const int nTop, const int nBottom)
			: _strName(strName)
			, _nLeft(nLeft)
			, _nRight(nRight)
			, _nTop(nTop)
			, _nBottom(nBottom)
		{}
		CString			_strName;			//合并后名称
		int				_nLeft;				//列头左位置
		int				_nRight;			//列头右位置
		int				_nTop;				//列头上位置
		int				_nBottom;			//列头下位置
	};

	// 列编辑定义
	struct stBwGridEditInfo
	{
		stBwGridEditInfo(CBCGPGridItem * pItem, KNode *pNode, ColumnFormat colFormat)
			: _pItem(pItem)
			, _pNode(pNode)
			, _colFormat(colFormat)
			, _result(true)
		{}
		CBCGPGridItem * _pItem;
		KNode *_pNode;
		ColumnFormat _colFormat;
		bool _result; // 为false时编辑失败，不将值赋值到KNode
	};

	//单元格定义
	struct Cell
	{
	public:
		Cell(const _variant_t& value, const DWORD_PTR data = 0)
			: _Value(value)
			, _Data(data)
		{}
		_variant_t		_Value;//值
		DWORD_PTR		_Data; //关联的数据指针
	};
	//一行数据
	typedef std::vector<Cell>  RowData;
public:
	CBwGanttGrid();
	virtual ~CBwGanttGrid();
public:
	const std::vector<ColumnFormat> &GetColumnFormats();

	/**
	* 单元格格式处理回调
	* @param [in]  nRow				所在行
	* @param [in]  nCol				所在列
	* @param [in] pItem				单元格对象
	* @return
	*/
	typedef void(WINAPI *RowCellFormatCall)(int nRow, int nCol,
		CBCGPGridItem* pItem, VOID* pUserData);
	
	/**
	* 合并单元格处理回调函数函数定义
	* @param [in]  nCol           所在列
	* @param [in] pPriRowItem	  前一行单元格
	* @param [in] pAfterRowItem   当前行单元格
	* @param [in] pUserData			SetRowCellCallBack输入的pUserData
	* @return	  bool   如果当前单元格需要和前一行单元格合并，返回TRUE，否则返回FALSE
	*/
	typedef BOOL(WINAPI *RowCellMergeCall)(int nCol, CBCGPGridItem* pPriRowItem,
		CBCGPGridItem* pcurRowItem, VOID* pUserData);

	// 创建对象
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	/**
	* 设置默认风格
	*/
	virtual void SetDefaultStyle();

	/**
	* 设置列头信息
	* @param [in]   columnFormats		列定义
	* @param [in]   nHeaderLineCount	列头行数
	* @param [in]   columnFormats		列头合并设置
	* @return
	*/
	virtual void SetColumnFormats(const std::vector<ColumnFormat>& columnFormats, int nHeaderLineCount = 1, const std::vector<ColumnMergeFormat>& ColumnMergeFormat = {});
	
	/**
	* 设置单元格格式处理回调
	* @param [in]   callfun		回调函数指针
	* @param [in]   pUserData	用户数据
	* @note 需要在更新数据之前调用SetRowCellFormatCallBack,指定的回调函数将会在数据发生添加和更新后触发（SetRowDatas\AddRowData\InsertRowData）
	* @return
	*/
	virtual void SetRowCellFormatCallBack(RowCellFormatCall callfun, VOID* pUserData = NULL);
	
	/**
	* 设置单元格合并处理回调
	* @param [in]   callfun		回调函数指针
	* @param [in]   pUserData	用户数据
	* @note 需要在更新数据之前调用SetRowCellMergeCallBack,指定的回调函数将会在数据发生添加和更新后触发（SetRowDatas\AddRowData\InsertRowData）
	* @return
	*/
	virtual void SetRowCellMergeCallBack(RowCellMergeCall callfun, VOID* pUserData = NULL);

	virtual CBCGPGridColumnsInfo& GetColumnsInfo()  override
	{
		return m_ColumnsEx;
	}

	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const  override
	{
		return m_ColumnsEx;
	}

	// 获取Grid项下标
	int GetGridItemIndex(CBCGPGridRow *pItem);
	int GetGridItemIndex(CBCGPGridItem *pItem);

	// 设置Grid值
	void SetGridData(int nRow, int nCol, const _variant_t& varValue);

	// 获取Grid值
	_variant_t GetGridData(int nRow, int nCol);

	// 根据名称获取列索引
	int GetColIndexByName(const CString &strName);

	// 获取选中行的下标数组
	void GetSelectedIndexes(std::set<int> &arrSelIndexes);

	// 获取选中行的下标
	int GetSelIndex();

	// 设置控制器
	void SetControl(CBwGanttControl *pControl);

	// 设置列头对齐方式
	void SetHeaderAlign(int nAlign);

	// 获取行
	CBCGPGridRow* GetRow(int nPos);

	// 重新绘制，重写该方法用来阻止sort方法的重绘
	virtual void DoRebuildTerminalItems();

	// 点击列头
	virtual void OnHeaderColumnClick(int nColumn);
	
	// 清除排序列
	void ClearSortColumn();
protected:
	virtual BOOL OnTrackHeader() override
	{
		return FALSE; // do not resize columns inside client area by mouse, only at header
	}

	/*
	* 创建列
	*/
	virtual void CreateColumns();
	virtual CBCGPGanttControl* GetGanttControlNotify() const;
	virtual void  OnItemChanged(CBCGPGridItem* pItem, int nRow, int nColumn);

	virtual CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	virtual void OnDrawHeader(CDC* pDC) override;
	virtual void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) override;
	virtual void OnPosSizeChanged() override;
protected:
	virtual void  OnUpdateVScrollPos(int nVOffset, int nPrevVOffset);
protected:
	BOOL _bCreated;										//是否已创建成功
	std::vector<ColumnMergeFormat> _ColumnMergeFormat;	// 列合并定义集合
	std::vector<ColumnFormat> _ColumnFormats;			//列定义集合
	CBCGPGridColumnsInfoEx	m_ColumnsEx;				// other header implementation

	RowCellFormatCall _pRowCellFormatCall;				//单元格格式处理回调
	RowCellMergeCall _pRowCellMergeCall;				//行数据合并处理回调
	VOID* _pFormatUserData;									////单元格格式处理回调用户数据指针
	VOID* _pMergeUserData;									////行数据合并处理回调用户数据指针

	CBwGanttControl *_pControl;							// 甘特图控制器指针
	std::map<int, int>					_mapColClick;			// 列头点击次数
	int											_nCurSelColIndex = -1;	// 当前进行排序的列头的下标
};

