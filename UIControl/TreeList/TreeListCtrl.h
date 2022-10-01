//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           TreeListCtrl.H
//    @author         KG
//    @date           2015/3/10 11:50
//    @brief		  树型列表控件
//*****************************************************

#pragma once

#include <map>
#include <vector>
#include <list>
#include "..\GdiTools.h"
#include "TreeListCfg.h"
#include "Editor\ListEdit.h"
#include "Editor\ListComboBox.h"
#include "Editor\ListAngleEdit.h"
#include "Editor\ListDataTime.h"
#include "Editor\ListTreeComboBox.h"
#include "Editor\AutoSearchCombo.h"
#include "TitleTip.h"
#include "IEventHandle.h"

namespace ControlUI
{
	/**
	*  @class    列表数据源
	*
	*  @brief
	*/
	class CONTROL_UI IListDataSource
	{
		friend class CTreeListCtrl;
	public:
		virtual ~IListDataSource() {}

	public:
		/**
		*  @brief    获取子项数目
		*
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*  @return   size_t 返回子项数目
		*/
		virtual size_t GetCount(CTreeListConfig* pConfig) = 0;

		/**
		*  @brief    获取指定子项数据
		*
		*  @param    size_t nIndex 子项索引
		*  @param    bool bShowItem 设置true则此条数据将显示在控件上（用于过滤不需要显示的中间节点）
		*  @param    HITEMDATA hItemData 列表行数据(只有第一次设置才有用)
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*  @return   IListDataSource*& pListDataSource 当前获取到的数据源
		*/
		virtual IListDataSource* GetItem(size_t nIndex, bool& bShowItem,
			HITEMDATA hItemData, CTreeListConfig* pConfig) = 0;

		/**
		*  @brief    获取单元格数据信息
		*			 只有当editConfig.comboDataFile不为空时，
		*			 才需要填充editConfig.comboList或editConfig.comboDataFile值
		*			 用于提高控件性能。
		*
		*  @param    size_t nCol 单元格所在列
		*  @param    size_t nRow 单元格所在行
		*  @param    const HCOLUMNDATA hColumnData 列头信息
		*  @param    HCELLDATA hCellData 返回单元格数据数
		*  @param    HEDITCONFIG hEditConfig 返回编辑配置数据
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*/
		virtual void GetCellData(size_t nCol, size_t nRow,
			const HCOLUMNDATA hColumnData, HCELLINFO hCellData, HEDITCONFIG hEditConfig, CTreeListConfig* pConfig) = 0;

		/**
		*  @brief    获取单元格文本数据
		*
		*  @param    size_t nCol 单元格所在列
		*  @param    size_t nRow 单元格所在行
		*  @param    const HEDITCONFIG hEditConfig 编辑配置数据
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*  @return   CString 返回单元格文本数据
		*/
		virtual CString GetCellText(size_t nCol, size_t nRow,
			const HEDITCONFIG hEditConfig, CTreeListConfig* pConfig) = 0;

		/**
		*  @brief    设置单元格文本数据
		*
		*  @param    size_t nCol 单元格所在列
		*  @param    size_t nRow 单元格所在行
		*  @param    const HEDITCONFIG hEditConfig 编辑配置数据
		*  @param    const CString& strNewValueg 当前设置的值
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*  @return   bool 返回true则修改成功
		*/
		virtual bool SetCellText(size_t nCol, size_t nRow,
			const HEDITCONFIG hEditConfig, const CString& strNewValue, CTreeListConfig* pConfig) = 0;

	private:
		/**
		*  @brief    获取数据源唯一标识(只能控件调用)
		*
		*  @return   size_t 返回数据源唯一标识
		*/
		virtual size_t GetKey() = 0;
	};

	/**
	*  @class    编辑配置数据
	*
	*  @brief
	*/
	typedef struct tagEDITCONFIG
	{
		friend class CTreeListCtrl;
	public:
		tagEDITCONFIG()
		{
			image = -1;
			dotNum = 2;
			option = 0;
			isShowTips = true;
			isEditMode = false;
			bImageCenter = false;
		}

		void CopyFrom(const tagEDITCONFIG& value)
		{
			// 直接复制表头配置
			bImageCenter = value.bImageCenter;
			image = value.image;
			dotNum = value.dotNum;
			option = value.option;
			field = value.field;
			enumerate = value.enumerate;
			eventHandle = value.eventHandle;
			isShowTips = value.isShowTips;

			// 只有下拉编辑时才需要下拉值，加快性能
			if (isEditMode)
			{
				comboList = value.comboList;
				comboDataFile = value.comboDataFile;
			}
		}

		bool			bImageCenter;	///< 图片居中显示
		CHAR			image;			///< 图片索引
		CHAR			dotNum;			///< 小数点位数
		int				option;			///< 功能选择
		CString			field;			///< 显示字段类型
		CString			enumerate;		///< 对应枚举;
		CStringVector	comboList;		///< 下拉框数据
		CString			comboDataFile;	///< 下拉框数据文件或者树节点XML（优先级高于comboList）
		bool			isEditMode;		///< 是否是下拉数据填充
		bool			isShowTips;		///< 是否显示提示信息
		CString         eventHandle;	///< 消息对应扩展点
		CString			splitChar;		///< 多选框分隔符
	}EDITCONFIG, *HEDITCONFIG;


	// 列数据列表
	typedef std::vector<HCOLUMNDATA> ColumnDataVector;

	// 列头排序
#define TLC_NO_SORT 0		// 不排序
#define TLC_ASC_SORT 1		// 升序
#define TLC_DSC_SORT -1		// 降序

/**
*  @class    列数据结构
*
*  @brief
*/
	typedef struct CONTROL_UI tagCOLUMNDATA
	{
		friend class CTreeListCtrl;

	public:
		tagCOLUMNDATA::tagCOLUMNDATA()
		{
			width = 20;
			image = -1;
			format = DT_LEFT;
			fixed = false;
			textColor = GetSysColor(COLOR_WINDOWTEXT);
			colRect.SetRectEmpty();
			hParent = NULL;
			sortType = TLC_NO_SORT;
		}

		~tagCOLUMNDATA()
		{
		}

		void SetWidth(SHORT inWidth)
		{
			SHORT nDifWidth = (inWidth - width);

			width = inWidth;

			if (hParent)
				hParent->AdjustParentWidth();

			AdjustChildWidth(nDifWidth);
		}

		SHORT GetWidth()
		{
			return width;
		}

	private:
		void AdjustParentWidth()
		{
			width = 0;

			// 确保父亲列头大小为子列头大小之和
			size_t nChildSize = childColumn.size();
			for (size_t i = 0; i < nChildSize; ++i)
			{
				width += childColumn[i]->GetWidth();
			}

			if (hParent)
			{
				hParent->AdjustParentWidth();
			}
		}

		void AdjustChildWidth(SHORT nDifWidth)
		{
			size_t nChildSize = childColumn.size();
			if (0 == nChildSize)
				return;

			childColumn[nChildSize - 1]->width += nDifWidth;
			childColumn[nChildSize - 1]->AdjustChildWidth(nDifWidth);
		}

		bool IsLeafCol()
		{
			return (0 == childColumn.size());
		}

	public:
		bool fixed; ///< 是否固定宽度
		CHAR image; ///< 图片索引
		USHORT format; ///< 列格式 
		COLORREF textColor; ///< 文字颜色
		CString title; ///< 列标题
		EDITCONFIG editConfig; ///< 编辑控制数据
		CHAR sortType;	// 列头排序类型

	private:
		SHORT width; // 列宽
		CRect colRect; ///< 列所在区域
		HCOLUMNDATA hParent; // 父列头
		ColumnDataVector childColumn; // 子表头数组
	}COLUMNDATA, *HCOLUMNDATA;

	/**
	*  @class    单元格数据结构
	*
	*  @brief
	*/
	typedef struct tagCELLDINFO
	{
		friend class CTreeListCtrl;
	public:
		tagCELLDINFO()
		{
			readonly = false;
			wpHeight = 0;
			backColor = GetSysColor(COLOR_WINDOW);
			textColor = GetSysColor(COLOR_WINDOWTEXT);
		}

		~tagCELLDINFO()
		{
		}

	public:
		int wpHeight;		///< 多行文本显示区域高度	
		bool readonly;		///< 是否只读
		COLORREF backColor; ///< 背景色
		COLORREF textColor; ///< 文本颜色

	}CELLINFO, *HCELLINFO;

	/**
	*  @class    单元格配置数据
	*
	*  @brief
	*/
	typedef struct tagCELLCONFIG
	{
		friend class CTreeListCtrl;
	public:
		tagCELLCONFIG()
		{
		}

	public:
		CString cellText; ///< 单元格文本
		CELLINFO cellInfo; ///< 单元信息
		EDITCONFIG editConfig; ///< 编辑配置

	}CELLCONFIG, *HCELLCONFIG;

	typedef std::vector<CELLCONFIG> CellConfigArray;

	/**
	*  @class    树型列表通知结构
	*
	*  @brief
	*/
	struct CTreeListNotify
	{
		NMHDR hdrNotify;			///< 通知消息结构
		int row;					///< 所在行
		int col;					///< 所在列
		CString strOldText;			///< 原值
		CString *pStrNewText;		///< 新值
		HEDITCONFIG hEditConfig;	///< 单元格信息
		int nClickType;				///< 点击类型 左击 = 0，右击 = 1
	};

	/**
	*  @class    排序通知事件
	*
	*  @brief
	*/
	struct CColumnSortNotify
	{
		NMHDR hdrNotify;			///< 通知消息结构
		HCOLUMNDATA hColData;		///< 列数据
		size_t nColIndex;			///< 列头索引
		CHAR sortType;				// 列头排序类型
	};

	class IListDataSource;

	/**
	*  @class    列表行数据
	*
	*  @brief
	*/
	typedef struct tagITEMDATA
	{
		friend class CTreeListCtrl;
	public:
		tagITEMDATA()
		{
			expand = true;

			seleced = false;
			hasChild = false;
			lastChild = false;
			treeLevel = 0;
			hParentData = NULL;
			rowRect.SetRectEmpty();
			pDataSource = NULL;
		}

		~tagITEMDATA()
		{
			DelDataSource();
		}

		/**
		*  @brief    清除数据源指针
		*
		*/
		void DelDataSource()
		{
			if (NULL != pDataSource)
			{
				delete pDataSource;
				pDataSource = NULL;
			}
		}

		/**
		*  @brief    判断当前节点是否收缩状态
		*
		*  @return   bool 返回收缩状态
		*/
		bool IsCollapse()
		{
			if (!expand)
				return true;

			if (NULL == hParentData)
				return !expand;

			return hParentData->IsCollapse();
		}

		/**
		*  @brief    判断当前节点是指定层级是否为最后节点
		*
		*  @return   bool 返回是否为最后节点
		*/
		bool IsLevelLastChild(int level)
		{
			if (level <= 0)
				return lastChild;

			if (NULL == hParentData)
				return false;

			return hParentData->IsLevelLastChild(--level);
		}

		/**
		*  @brief    返回数据源
		*
		*  @return   IListDataSource*	数据源指针
		*/
		IListDataSource* GetDataSource()
		{
			return pDataSource;
		}

		/**
		*  @brief    判定是否还存在子节点
		*
		*  @return   bool	返回是否存在子节点
		*/
		bool HasChildNode()
		{
			return hasChild;
		}


	public:
		bool expand; ///< 是否展开

	private:
		UCHAR treeLevel; ///< 节点所在树层次(第一层0,第二层1)
		bool hasChild; ///< 是否有子节点
		bool lastChild; ///< 当前节点中是否为最后一个子节点
		bool seleced; ///< 是否选中
		CRect rowRect; ///< 行区域
		CRect expandRect;	///< 展开、收缩区域;	 	
		HITEMDATA hParentData; ///< 父节点
		IListDataSource* pDataSource; ///< 此行对应的数据源
	}ITEMDATA, *HITEMDATA;

	// 定义数据源标识与行数据对应关系
	typedef std::map<size_t, HITEMDATA> MapDataSource;
	typedef MapDataSource::iterator MapDataSourceIter;

	typedef std::list<std::pair<size_t, HITEMDATA>> ListDataSource;
	typedef ListDataSource::iterator ListDataSourceIter;

	typedef std::vector<IListDataSource*>	DataSourceArray;
	typedef DataSourceArray::iterator DataSourceArrayIter;

	/**
	*  @class    树型列表控件类
	*
	*  @brief
	*/
	class CONTROL_UI CTreeListCtrl : public CWnd
	{
	public:
		CTreeListCtrl();
		virtual ~CTreeListCtrl();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief   更新控件显示
		*
		*  @param   bool bDataChange 当为true是则更新行数据，否则直刷新界面
		*  @remark	当前添操作列或者数据项时
		*			调用此方法用于更新界面显示与滚动条
		*/
		void UpdateList(bool bDataChange = false, bool bCancelSel = false);

		/**
		*  @brief    创建控件
		*
		*  @param    const RECT & rect 窗口区域
		*  @param    CWnd * pParentWnd 父窗口
		*  @param    UINT nID 控件ID
		*  @param    DWORD dwStyle 样式
		*  @return   bool 成功返回TRUE
		*/
		bool Create(const RECT& rect, CWnd* pParentWnd, UINT nID = 0xfff0,
			DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE);

		/**
		*  @brief    设置数据源
		*
		*  @param    IListDataSource * pDataSource 数据源指针
		*  @param    bool bDelete 是否删除数据源指针
		*/
		void SetDataSource(IListDataSource* pDataSource, bool bDelete);

		/**
		*  @brief    加载列表配置文件
		*
		*  @param    const CStirng & strCfg 配置文件路径
		*  @param    const CString & strCfgName 配置名(一个配置文件存在多个配置,为空则读第一个)
		*  @return   bool 成功返回true
		*/
		bool LoadConfig(const CString& strCfg, const CString& strCfgName = L"");

		/**
		*  @brief    设置控件图标
		*
		*  @param    CImageList* pImageList 控件图标集
		*  @param    bool bAutoDel 是否自动释放图像列表
		*/
		void SetImageList(CImageList* pImageList, bool bAutoDel = true);

		/**
		*  @brief    设置事件通知父窗口
		*
		*  @param    CWnd * pParent	父窗口句柄
		*/
		void SetNofityWnd(HWND hNotifyWnd);

		/**
		*  @brief    设置是否支持多选;
		*
		*  @param    bool bSingleSelect 是否支持多选, true:为单选；false:为多选
		*/
		void SetSingleSelect(bool bSingleSelect = true);

		/**
		*  @brief    设置焦点行;
		*
		*  @param    size_t nSelectIndex 默认选中行（数据索引）
		*/
		void SetFocusItem(size_t nSelectIndex = 0);

		/**
		*  @brief    设置焦点行
		*
		*  @param    size_t nKey 待设置焦点行的数据源
		*/
		void SetFocusItemByKey(size_t nKey);

		/**
		*  @brief    设置焦点行
		*
		*  @param    size_t nKey 待设置焦点行的数据源
		*/
		void SetFocusItemByDataSource(IListDataSource* pDataSource);

		/**
		*  @brief    设计焦点行
		*
		*  @param    const CString & strSel	行数据
		*  @return   bool	找到数据并设置返回true
		*/
		bool SetFocusItemByCString(const CString& strSel);

		/**
		*  @brief    获取选中的单个数据源对象
		*
		*  @return   IListDataSource* 选中的数据源对象，未选中则返回NULL
		*/
		IListDataSource* GetSelectedItem();

		/**
		*  @brief    获取选择的多个数据源节点;
		*
		*  @param    DataSourceArray & datasourceArray 多个数据源节点;
		*/
		void GetSelectedItems(DataSourceArray &datasourceArray);

		/**
		*  @brief    获取当前控件所有数据源
		*
		*  @param    DataSourceArray & datasourceArray 数据源节点;
		*/
		void GetDataSourceArray(DataSourceArray &datasourceArray);

		/**
		*  @brief    获取选中的行的数据索引值(基于0)
		*
		*  @return   int 没有选中返回-1
		*/
		int GetSelectedIndex();

		/**
		*  @brief    获取所有行数
		*
		*  @return   返回行数
		*/
		size_t GetTotalRowCount();

		/**
		*  @brief    清除控件数据
		*
		*/
		void DeleteAllItem();

		/**
		*  @brief    是否显示网格
		*
		*  @param    bool bShow true为显示false为影藏
		*/
		void SetHasGrid(bool bShow = true);

		/**
		*  @brief    获取当前控件高度
		*
		*  @return   int	返回高度值
		*/
		int GetCtrlHeight();

		/**
		*  @brief    设置列头高度
		*
		*  @param   int	nHeight 列头高度值
		*/
		void SetHeadItemHeight(int nHeight);

		/**
		*  @brief    选定树控件第一个叶子节点
		*
		*  @param    CTreeListCtrl & 引用的树控件
		*  @param    bool 聚焦第一个子节点后是否显示回顶端
		*  @return   void
		*/
		void SelectFirstLeafNode(bool bFocusOnTop = false);

		/**
		*  @brief    确保指定行索引可见（滚动条，不包括收缩状态）
		*
		*  @param	 int nDataRowIndex 数据行索引
		*/
		void MakeRowVisible(int nDataRowIndex);

		/**
		*  @brief    设置列头文字是否自动换行（表头与单元格）
		*
		*  @param	 bool bWorldWarp 是否换行
		*/
		void SetColumnWordWarp(bool bWorldWarp);

		/**
		*  @brief    设置行文字是否自动换行
		*
		*  @param	 bool bWorldWarp 是否换行
		*/
		void SetRowWordWarp(bool bWorldWarp);

		/**
		*  @brief    设置是否去掉空格
		*
		*  @param    bool bTrim
		*  @return   void
		*/
		void SetTrim(bool bTrim);

		/**
		*  @brief    设置是否检查选择项是否变化
		*
		*  @param    bool bCheck 检测选择项是否变化
		*  @return   void
		*/
		void SetSelectChangeCheck(bool bCheck);

		/**
		*  @brief    设置树型收缩展开图标样式
		*
		*  @return   void
		*/
		void SetCollapseStyle(bool bStyle = false);

	public:
		/**
		*  @brief    显示列头
		*
		*  @param    bool bShow true为显示false为隐藏
		*/
		void ShowHeader(bool bShow = true);

		/**
		*  @brief    是否显示树结构线
		*
		*  @param    bool bShow true为显示false为隐藏
		*/
		void SetHasLine(bool hasLine = true);

		/**
		*  @brief    设置哪列显示为树;
		*
		*  @param    int nColumnIndex 显示为树的列(第一层表头索引)
		*  @param    HCOLUMNDATA hParentCol 父表头
		*/
		void SetTreeColumn(int nColumnIndex = 0);

		/**
		*  @brief    启用禁用列表控件列头排序功能;
		*
		*  @param    bool bEnable 是否启用排序功能
		*/
		void EnableColumnSort(bool bEnable);

		/**
		*  @brief    获取列数目
		*
		*  @param    HCOLUMNDATA hParentCol 父表头
		*  @return   size_t 返回列数目
		*/
		size_t GetColumnCount(HCOLUMNDATA hColumnData = NULL);

		/**
		*  @brief    获取指定列数据
		*
		*  @param    size_t nCol 指定列
		*  @param    HCOLUMNDATA hParentCol 父表头
		*  @return   HCOLUMNDATA 返回列数据
		*/
		HCOLUMNDATA GetColumn(size_t nCol, HCOLUMNDATA hColumnData = NULL);

		/**
		*  @brief    添加列
		*
		*  @param    size_t nCol 插入列的位置
		*  @param    HCOLUMNDATA hParentCol 父表头
		*  @return   HCOLUMNDATA 返回新插入列
		*/
		HCOLUMNDATA InsertColumn(size_t nCol, HCOLUMNDATA hParentCol = NULL);

		/**
		*  @brief    删除列
		*
		*  @param    size_t nCol 删除列的位置
		*  @param    HCOLUMNDATA hParentCol 父表头
		*
		*/
		void DeleteColumn(size_t nCol, HCOLUMNDATA hParentCol = NULL);

		/**
		*  @brief    删除所有列头
		*
		*/
		void DeleteAllColumn();

		/**
		*  @brief    获取列头字体缩放比例
		*
		*  @return   double 比例值
		*/
		double GetFontScale();

		/**
		*  @brief    取消当前编辑
		*
		*/
		void CancelEdit();

		/**
		*  @brief    编辑焦点行指定列
		*
		*  @param    int nCol 编辑的列
		*/
		void EditFocusItem(int nCol);

	public:
		/**
		*  @brief    设置列宽根据控件宽度变化而自动调整
		*
		*  @param    bool bIsAutoColumnWidth	自动标识
		*/
		void SetAutoColumnWidth(bool bIsAutoColumnWidth = true);

		/**
		*  @brief    设置选中行展开状态
		*
		*  @param    bool bExpand	展开状态设置
		*  @return   bool	true则设置成功，否则失败
		*/
		bool SetExpandBySelectItem(bool bExpand = true);

		/**
		*  @brief    根据数据源找到行并设置展开状态
		*
		*  @param    size_t nKey	数据源独有KEY
		*  @param    bool bExpand	展开状态
		*  @param    bool bParent	递归设置父节点展开状态
		*  @return   bool	true则设置成功，否则失败
		*/
		bool SetExpandItemByKey(size_t nKey, bool bExpand = true, bool bParent = false);

	protected:
		/**
		*  @brief    编辑前逻辑判断
		*
		*  @param    int nCol 编辑列
		*  @param    int nRow 编辑行
		*  @param    HEDITCONFIG hEditCfg 单元格信息
		*  @param    const CString& strOldCellText 单元格旧文本
		*  @return   bool 可以编辑返回true，否则返回false
		*/
		virtual bool OnBeginEdit(int nCol, int nRow,
			HEDITCONFIG hEditCfg, const CString& strOldCellText);

		/**
		*  @brief    编辑后处理逻辑
		*
		*  @param    int nCol 编辑列
		*  @param    int nRow 编辑行
		*  @param    HEDITCONFIG hEditCfg 单元格信息
		*  @param    const CString& strOldCellText 单元格旧文本
		*  @param    CString& strNewCellText 单元格新文本(可修改)
		*  @return   bool 正常编辑返回true，否则返回false
		*/
		virtual bool OnEndEdit(int nCol, int nRow,
			HEDITCONFIG hEditCfg, const CString& strOldCellText, CString& strNewCellText);

		/**
		*  @brief    显示并执行右键菜单命令
		*
		*  @return   bool 执行成功返回true，否则返回false
		*/
		virtual bool ShowRightMenu();

		/**
		*  @brief    重载消息处理函数
		*
		*  @param    MSG * pMsg	消息传递
		*  @return   BOOL	返回消息
		*/
		BOOL PreTranslateMessage(MSG* pMsg);

		/**
		*  @brief    通鼠标点编辑单元格
		*
		*  @param    CPoint ptMs 鼠标客户区域
		*  @param    bool bIsDbClk 是否为单击
		*/
		void EditItemByPoint(CPoint ptMs, bool bIsDbClk = false);

		/**
		*  @brief   编辑指定参数的列
		*
		*  @param   IListDataSource* pDataSource 当前编辑的数据源
		*  @param   int nRow 编辑行
		*  @param   int nCol 编辑列
		*  @param   CRect rect 列区域
		*  @param   HCOLUMNDATA hColumnData 列头信息
		*  @param   bool bIsDbClk 是否为单击
		*/
		void EditItemByParam(IListDataSource* pDataSource,
			int nRow, int nCol, CRect rect, HCOLUMNDATA hColumnData, bool bIsDbClk);

	private:
		/**
		*  @brief    初始化控件数据
		*
		*/
		void InitControlData();

		/**
		*  @brief    注册控件类
		*
		*  @return   bool 成功返回TRUE
		*/
		bool RegisterTreeList();

		/**
		*  @brief    删除所有列头
		*
		*  @param    HCOLUMNDATA hParentCol 父表头
		*/
		void DeleteAllColumn(HCOLUMNDATA hParentCol);

		/**
		*  @brief    执行拖动列
		*
		*/
		void DoDragColumn();

		/**
		*  @brief    更新鼠标测试
		*
		*  @param   bool bUpdateSel 是否更新收缩状态
		*  @return   bool 返回true则需要刷新界面
		*/
		bool UpdateHitTest(bool bUpdateSel);

		/**
		*  @brief    更新滚动条
		*
		*  @param    int scrollMode 需要更新的滚动条
		*/
		void UpdateScrollBar(int scrollMode = SB_BOTH);

		/**
		*  @brief    收集数据源数据
		*
		*/
		void CollectItemListData();

		/**
		*  @brief    递归收集数据源数据
		*
		*  @param    IListDataSource * pDataSource 数据源指针
		*  @param    ListDataSource & dataSouceList 数据源列表
		*  @param    DataSourceMap & dataSourceMap 数据源与行对应关系
		*  @param    HITEMDATA hParentData 父亲节点指针
		*  @param    int nTreeLevel 数据源所在树节点层次
		*  @param    HITEMDATA &hLastChildItem 最后一个子节点
		*  @return   bool 是否有子节点
		*/
		bool CollectItemListData(IListDataSource* pDataSource,
			ListDataSource& dataSouceList, MapDataSource& dataSourceMap,
			HITEMDATA hParentData, int nTreeLevel, HITEMDATA& hLastChildItem);

		/**
		*  @brief    更新列头排序状态
		*/
		void UpdateColumnSortState();

	private:
		/**
		*  @brief    计算表头区域
		*
		*  @return   返回表头区域
		*/
		CRect CalcHeaderRect();

		/**
		*  @brief    绘制表头
		*
		*  @param    CMemoryDC & dcMemory 内存DC
		*  @param    const CRect& rcHeader 绘制区域
		*/
		void DrawHeader(CMemoryDC& dcMemory, const CRect& rcHeader);

		/**
		*  @brief    绘制表头列
		*
		*  @param    CMemoryDC & dcMemory 内存DC
		*  @param    const CRect& rcHeader 绘制区域
		*  @param    HCOLUMNDATA hColumnData 列数据
		*/
		void DrawColumn(CMemoryDC& dcMemory, const CRect& rcColumn, HCOLUMNDATA hColumnData);

	private:
		/**
		*  @brief    计算列表区域
		*
		*  @return   返回列表区域
		*/
		CRect CalcListRect();

		/**
		*  @brief    绘制行
		*
		*  @param    CMemoryDC & dcMemory 内存DC
		*  @param    const CRect& rcHeader 表头绘制区域
		*  @param    const CRect& rcList 除去表头的绘制区域
		*/
		void DrawRow(CMemoryDC& dcMemory, const CRect& rcHeader,
			const CRect& rcList);

		/**
		*  @brief    绘制行背景
		*
		*  @param    CMemoryDC & dcMemory 内存DC
		*  @param    HITEMDATA hItemData 列表行数据
		*  @param    const CRect & rcRow  行绘制区域
		*  @param    ColumnDataVector& leafColumnVector 列头叶子节点
		*  @param    bool bFirstRow  是否为第一行
		*/
		void DrawRowBackground(CMemoryDC& dcMemory, HITEMDATA hItemData,
			const CRect& rcRow, ColumnDataVector& leafColumnVector, bool bFirstRow = false);

		/**
		*  @brief    绘制单元格
		*
		*  @param    CMemoryDC & dcMemory 内存DC
		*  @param    const CRect& rcCell 绘制区域
		*  @param    bool bTreeNode 是否是示为树节点
		*  @param    bool bFirstCell 是否是第一列
		*  @param    HITEMDATA hItemData 列表行数据
		*  @param    HCELLDATA hCellData 单元数据
		*  @param    HEDITCONFIG hEditConfig 编辑配置
		*  @param    HCOLUMNDATA hColumnData 列头数据
		*  @param    const CString& strCellText 单元格文本
		*/
		void DrawCell(CMemoryDC& dcMemory, const CRect& rcCell, bool bTreeNode, bool bFirstCell,
			HITEMDATA hItemData, HCELLINFO hCellData, HEDITCONFIG hEditConfig, HCOLUMNDATA hColumnData,
			const CString& strCellText);

		/**
		*  @brief    设置单选项
		*
		*  @param    HITEMDATA hItemSelect 选中的节点
		*  @return   bool 设置成功返回true，否则返回false
		*/
		bool SetItemSelectFlag(HITEMDATA hItemSelect);

		/**
		*  @brief    项父级发送Notify通知消息
		*
		*  @param    UINT nMessage 消息ID
		*  @param    int nClickType 左击 = 0，右击 = 1
		*  @param    int nCol 编辑列
		*  @param    int nRow 编辑行
		*  @param    HEDITCONFIG hEditCfg 单元格信息
		*  @param    const CString & strOldText 修改前单元格文本
		*  @param    CString* pStrNewText 修改后单元格文本指针
		*  @return   LRESULT 事件通知结果
		*/
		LRESULT NotifyParent(UINT nMessage, int nCol = -1, int nRow = -1, HEDITCONFIG hEditCfg = NULL,
			const CString& strCellText = L"", CString* pStrNewText = NULL, int nClickType = 0);

		/**
		*  @brief    获取当前鼠标指向位置的单元格区域
		*
		*  @param    int nCol 单元格所在列
		*  @param    int nRow 单元格所在行
		*  @param    HCOLUMNDATA& hColData 当前选择列头数据
		*  @param    CRect & rect 当前鼠标指向位置的单元格区域
		*  @param    IListDataSource* &pDataSource 返回的数据源节点
		*  @param    CPoint* pPtMs 当前鼠标客户坐标
		*  @return   bool 焦点在控件范围内则返回true，否则返回false
		*/
		bool GetFocusRect(int &nCol, int &nRow, HCOLUMNDATA& hColData,
			CRect &rect, IListDataSource* &pDataSource, CPoint* ptMs = NULL);

		/**
		*  @brief    释放自定义控件;
		*
		*/
		void DeleteControl();

		/**
		*  @brief    创建编辑控件
		*
		*  @param    int nOption 编辑选项
		*  @param    EDITCONFIG editConfig 编辑配置
		*  @param    CRect rect 绘制区域
		*  @param    const CString& strDefaultValue 控件展现的文本
		*  @param    const CString& strDefaultData 控件展现的罗展
		*/
		void CreateEditorControl(int nOption,
			EDITCONFIG editConfig,
			CRect rect,
			const CString& strDefaultValue,
			const CString& strDefaultData);

		/**
		*  @brief    根据控件宽度自动调整列宽
		*/
		void AutoColumnWidth();

		/**
		*  @brief    获取单元格文本显示区域
		*
		*  @param    CDC* pDcTest 测试DC
		*  @param    const CString & strText 单元格文本
		*  @param    CRect & rcCell 单元格区域
		*  @param    bool bTreeNode 是否为树节点
		*  @param    HITEMDATA hItemData 行数据
		*  @param    HEDITCONFIG hEditConfig 编辑配置
		*  @param    HCOLUMNDATA hColumnData 列数据
		*  @param    CRect & rcTextOut 实际显示需要的大小
		*/
		void CalcCellTextOutRect(CDC* pDcTest, const CString& strText,
			CRect& rcCell, bool bTreeNode, HITEMDATA hItemData,
			HEDITCONFIG hEditConfig, HCOLUMNDATA hColumnData, CRect& rcTextOut);

		/**
		*  @brief    处理单元格提示信息
		*
		*/
		void ProcessCellTooltip();

		/**
		*  @brief    键盘按键设置选中行
		*
		*  @param    bool bNextRow	true选取下一行，否则选取上一行
		*  @return   void
		*/
		void SetSelRowByKey(bool bNextRow);

		/**
		*  @brief    获取列头最大层级
		*
		*  @return   HCOLUMNDATA hColumnData 列头
		*/
		void GetMaxColumnLevel(int& nMaxLevel,
			HCOLUMNDATA hColumnData = NULL, int nCurLevel = 0);

		/**
		*  @brief    获取列头线性结构
		*
		*  @param   ColumnDataVector& outColumnVector 列头线性结构
		*  @param   HCOLUMNDATA hColumnData 列头(递归使用)
		*/
		void GetColumnLinearityStruct(ColumnDataVector& outColumnVector,
			HCOLUMNDATA hColumnData = NULL);

		/**
		*  @brief    获取列头叶子结构
		*
		*  @param   ColumnDataVector& leafColumnVector 列头线性结构
		*  @param   HCOLUMNDATA hColumnData 列头(递归使用)
		*/
		void GetColumnAllLeafStruct(ColumnDataVector& leafColumnVector,
			HCOLUMNDATA hColumnData = NULL);

	public:
		void OnPaint();
		BOOL OnEraseBkgnd(CDC* pDC);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnLButtonDblClk(UINT nFlags, CPoint point);
		void OnRButtonDown(UINT nFlags, CPoint point);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		void OnSize(UINT nType, int cx, int cy);

		LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
		LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
		LRESULT OnEndEdit(WPARAM wParam, LPARAM lParam);
		LRESULT OnHyperLink(WPARAM wParam, LPARAM lParam);
		void OnKillFocus(CWnd* pNewWnd);
		LRESULT OnThemeChanged();

	protected:
		CListEdit				_listEdit;				///< 带有校验功能的文本编辑框
		CListComboBox			_listCombo;				///< 组合框编辑
		CListDataTime			_listDataTime;			///< 时间编辑
		CListAngleEdit			_listAngleEdit;			///< 角度编辑
		CListTreeComboBox		_listTreeCombo;			///< 树组合框编辑
		CAutoSearchCombo		_autoCombo;				///< 自动搜索组合框
		CWnd					*_pControl;				///< 自定义控件句柄

	protected:
		bool					_bCheckSelChange;		///< 是否检查选择过滤
		bool					_bLinkOn;				///< 当前有超链接触发
		bool					_lockTipUpdate;			///< 锁住提示更新
		bool					_showHeader;			///< 是否显示表头
		bool					_hasGrid;				///< 是否有网格
		bool					_hasLine;				///< 是否显示树结构线
		bool					_bMouseOver;			///< 鼠标是否悬停
		bool					_bBtnDown;				///< 鼠标左键是否按下
		bool					_bSingleSelect;			///< 是否支持多选
		bool					_bDelDataSource;		///< 是否删除数据源指针
		int						_nHeaderHeight;			///< 表头高度
		int						_nRowHeight;			///< 行默认高度
		int						_treeColumn;			///< 哪列显示为树
		int						_nEditRowIndex;			///< 当前编辑的行索引
		int						_nEditColIndex;			///< 当前编辑的列索引(基于列头叶子节点)
		HWND					_hNofigyWnd;			///< 事件通知窗口
		CSize					_iconSize;				///< 控件图标大小
		CFont					_treeListFont;			///< 控件字体
		CFont					_underlineFont;			///< 下划线字体
		CPoint					_dragStartPt;			///< 列拖到起始位置
		COLORREF				_bkColor;				///< 控件背景颜色
		COLORREF				_selTopColor;			///< 行选中时渐变颜色
		COLORREF				_selBottomColor;		///< 行选中时渐变颜色
		COLORREF				_hotTopColor;			///< 行悬浮时渐变颜色
		COLORREF				_hotBottomColor;		///< 行悬浮时渐变颜色
		COLORREF				_selBorderColor;		///< 行选中背景边框
		COLORREF				_hotBorderColor;		///< 行悬浮时背景边框
		COLORREF				_gridColor;				///< 网格颜色
		CTheme					_headerTheme;			///< 表头皮肤
		CTheme					_progressTheme;			///< 进度条皮肤
		HICON					_hIconCollapse;			///< 收缩图标
		HICON					_hIconExpand;			///< 展开图标
		CImageList*				_pImageList;			///< 图标列表
		bool					_bDelImageList;			///< 是否释放图像列表
		HCOLUMNDATA				_hHoverCol;				///< 鼠标当前悬停的列
		HCOLUMNDATA				_hPressCol;				///< 鼠标当前按下的列
		HCOLUMNDATA				_hDragingCol;			///< 鼠标当前托动的列
		HITEMDATA				_hHoverRow;				///< 鼠标当前悬停的行
		int						_nHoverRowIndex;		///< 鼠标当前悬停的行
		ColumnDataVector		_columnVector;			///< 表头数据
		ListDataSource			_listDataSource;		///< 数据源标识与控件数据(记录状态数据)
		IListDataSource			*_pListDataSource;		///< 控件数据源指针
		CTreeListConfig			*_pConfig;				///< 树型列表配置
		CRect					_rcLastRect;			///< 记录控件上一次大小
		bool					_bAutoColumnWidth;		///< 自动调整列宽标识
		int						_nLastSelectItem;		///< 上一次选中行
		CTitleTip				_cellToolTip;			///< 单元格提示信息
		bool					_bUpDownFlag;			///< 键盘上下按键标志
		bool					_bNoChild;				///< 当前数据是否为一维数据(为了弥补_treeColumn默认值不为-1造成对齐问题bw-134327)
		bool					_bColumnWordWarp;		///< 列头是否文字换行显示
		bool					_bRowWordWarp;			///< 行是否文字换行显示
		bool					_bColumnSort;			///< 是否开启列头排序功能
		bool					_bIsTrim;				///< 是否去掉空格
		bool					_bCollapseStyle;		///< 树控件折叠展开样式
		float					_fHightLightFactor;     ///< 高亮色比重
	};
}