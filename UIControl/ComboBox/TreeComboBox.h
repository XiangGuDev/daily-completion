//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           TreeComboBox.H
//    @author         KG
//    @date           2015/3/8 11:19
//    @brief          树型组合框控件
//*****************************************************

#pragma once

using namespace CommBase;

namespace ControlUI
{
	// 自定义组合框下拉关闭消息
#define UM_DROP_CLOSE	(WM_USER + 101)

#define UM_CHECK_EDIT (WM_USER + 102)

	class CTreeComboBox;


	/**
	*  @class    组合框下拉树控件
	*
	*  @brief    内部类请不要当控件使用
	*/
	class CONTROL_UI CComboBoxTree : public CTreeCtrl
	{
	public:
		CComboBoxTree();
		virtual ~CComboBoxTree();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief    显示下拉树
		*
		*  @param    CRect rect
		*/
		virtual void Display(CRect rect);

		/**
		*  @brief    消息过滤
		*
		*  @param    MSG * pMsg
		*  @return   BOOL
		*/
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		/**
		*  @brief    初始化
		*
		*  @param    CTComboBox * pCombo
		*/
		void Init(CTreeComboBox* pCombo) { _pCombo = pCombo; };

	public:
		/**
		*  @brief    联动刷新子级勾选状态
		*
		*  @param    HTREEITEM hItem 当前节点
		*/
		void ConsistentChildCheck(HTREEITEM hItem);

		/**
		*  @brief    联动刷新父勾选状态
		*
		*  @param    HTREEITEM hItem 当前节点
		*/
		void ConsistentParentCheck(HTREEITEM hItem);

		/**
		*  关闭下拉控件
		*/
		void CloseTree(int nTag = 0);

	protected:
		afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnDestroy();
		afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam, LPARAM lParam);

	private:
		CTreeComboBox*		_pCombo;		///< 关联的组合框控件
	};

	//// 树型组合框下拉展开消息
#define UM_DROP_DOWN (WM_USER +102 )

/**
*  @class    树型组合框控件
*
*  @brief
*/
	class CONTROL_UI CTreeComboBox : public CComboBox
	{
		DECLARE_DYNAMIC(CTreeComboBox)

	public:
		CTreeComboBox();
		virtual ~CTreeComboBox();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief    显示下拉树
		*
		*/
		void DisplayTree();

		/**
		*  @brief    获取下拉树控件
		*
		*  @return   CTreeCtrl&
		*/
		CTreeCtrl& GetTreeCtrl();

		/**
		*  @brief    控件是否激活
		*
		*  @return   BOOL
		*/
		BOOL IsControlActive() const;

		/**
		*  @brief    获取下拉树宽度
		*
		*  @param    const int nHeight
		*/
		int GetDroppedWidth();

		/**
		*  @brief    获取下拉树高度
		*
		*  @param    const int nHeight
		*/
		int GetDroppedHeight() const;

		/**
		*  @brief    设置下拉树宽度
		*
		*  @param    const int nHeight
		*			 (为0时自动适应树展开的宽度)
		*/
		void SetDroppedWidth(const int nWidth);

		/**
		*  @brief    设置下拉树高度
		*
		*  @param    const int nHeight
		*/
		void SetDroppedHeight(const int nHeight);

		/**
		*  @brief    是否为选择框模式
		*
		*  @return    bool 返回是否带选择框模式
		*/
		bool IsCheckMode();

		/**
		*  @brief    设置是否为选择框模式
		*
		*  @return    bool 返回是否带选择框模式
		*/
		void SetCheckMode(bool bCheckModel);

		/**
		*  @brief    控件创建之后再加载XML分类数据
		*
		*  @param    const CString& xmlFile 下拉树Xml配置文件(或XML内容字符串)
		*  @param    bool bNoLimitOrSelAll true自动添加不限或全选
		*  @return   bool 成功返回true
		*/
		bool LoadXmlData(const CString& xmlFile, bool bNoLimitOrSelAll = false);

		/**
		*  @brief    根据文件节点加载xml分类数据
		*
		*  @param    CXmlElement * pElm
		*  @param    bool bNoLimitOrSelAll
		*  @return   bool
		*/
		bool LoadXmlData(CXmlElement* pElm, bool bNoLimitOrSelAll = false);

		/**
		*  @brief    加载字符串数组
		*
		*  @param    CStringVector & listData 字符串数组
		*  @param    bool bNoLimitOrSelAll true自动添加不限或全选
		*/
		void LoadStringVector(CStringVector& listData,
			bool bNoLimitOrSelAll = false, const CString strSelAll = L"");

		/**
		*  @brief    保存控件数据(仅当CheckMode为true时支持)
		*
		*  @param    CXmlElement* pElm 保存控件数据根节点
		*/
		void SaveXmlData(CXmlElement* pElm);

		/**
		*  @brief    获取选择字符串
		*
		*/
		CString GetCheckString();

		/**
		*  @brief   设置选择字符串
		*
		*  @param    const CString& strCheck 显示的文本
		*/
		void SetCheckString(const CString& strCheck);

		/**
		*  @brief    设置复选模式分隔符
		*
		*  @param    const CString & ch
		*  @return   void
		*/
		void SetSplitChar(const CString& ch);

		/**
		*  @brief    获得复选模式分隔符
		*
		*  @return   CString
		*/
		CString GetSplitChar() const;

		/**
		*  @brief    获取控件当前选择的附加数据
		*
		*  @return   CString 返回控件选择项的附加数据
		*/
		CString GetCurrentSelData();

		/**
		*  @brief    获取控件当前选择的显示文本
		*
		*  @return
		*/
		CString GetCurrentSelText();

		/**
		*  @brief    设置顶层窗口
		*
		*  @param    CWnd * pOwner
		*/
		void SetParentOwner(CWnd* pOwner);

		/**
		*  @brief    获取父窗口
		*
		*  @return   CWnd*
		*/
		CWnd* GetParentOwner();

		/**
		*  @brief    手动插入节显示节点
		*
		*  @param    const CString & strTest 显示字符
		*  @param    CString strData 关联数据
		*  @param    HTREEITEM hParent 父节点
		*  @param    bool isLeafNode 是否为叶子节点
		*  @return   HTREEITEM 返回新节点
		*/
		HTREEITEM InsertItem(const CString& strTest,
			CString strData, HTREEITEM hParent = TVI_ROOT, bool isLeafNode = false);

		/**
		*  @brief    清除数据
		*
		*/
		virtual void ResetContent();

		/**
		*  函数介绍： 情空节显示节点
		*  输出参数：
		*  返回值：     void
		*/
		virtual void ResetItemContent();

		/**
		*  @brief    复选框控制只能选择一个
		*
		*/
		void SetSingleCheck(bool bCheck) { _bSigleCheck = bCheck; }

		/**
		*  @brief    是否只能选择一个复选框
		*
		*/
		bool IsSingleCheck() { return _bSigleCheck; }

		void SetSelectByText(bool bSelectByText)
		{
			_bSelectByText = bSelectByText;
		}

		void SetSelectData(CString strText, CString StrData)
		{
			_strCurSelText = strText;
			_strCurSelData = StrData;
		}

	public:
		/**
		*  @brief    消息过虑
		*
		*/
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		void ResetItemData();

		static const CString NoData();

		/**
		*  @brief    通过文本获取Data,搜索到第一个匹配的文本值对应的Data即返回
		*
		*  @param    CString & text
		*  @return   CString
		*/
		CString GetDataByText(CString &text);

		/**
		*  @brief    根据Data定位
		*
		*  @param    CString strData
		*  @return
		*/
		void SetCheckSelectByData(CString strData);

		/**
		*  @brief    设置显示文本
		*
		*  @param    CString strText
		*  @return
		*/
		void SetShowText(CString strText);

	protected:
		/**
		*  @brief    获取控件文本数据
		*
		*  @return   CString 返回控件文本
		*/
		CString GetControlText();

		/**
		*  @brief    保存树形节点数据
		*
		*  @param    CTreeCtrl & treeCtrl 树控件
		*  @param    HTREEITEM hParent 父节点
		*  @param    CXmlElement * pElm xml文件节点
		*/
		void SaveTreeCtrl(CTreeCtrl& treeCtrl, HTREEITEM hParent, CXmlElement* pElm);

		/**
		*  @brief    初始化下拉树控件
		*
		*  @param    HTREEITEM hRoot
		*/
		void InitDropTree();

		/**
		*  @brief    收缩所的下拉树节点
		*
		*  @param    HTREEITEM hRoot
		*/
		void CollapseAllTreeItem(HTREEITEM hRoot);

		/**
		*  @brief    通过文本查找对的是下拉项
		*
		*  @param    HTREEITEM hRoot
		*  @param    CString strItem
		*  @param    CString strData
		*  @return   HTREEITEM
		*/
		HTREEITEM FindItemByText(HTREEITEM hRoot, CString strItem, CString strData);

		/**
		*  @brief    获取显示的下拉项
		*
		*  @param    HTREEITEM hRoot
		*  @param    int & nCount
		*/
		void GetShowItem(HTREEITEM hRoot, int& nCount);

		/**
		*  @brief    控件关联处理
		*
		*/
		virtual void PreSubclassWindow();

		/**
		*  @brief    控件关联处理
		*
		*/
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

		CString GetDataFromText(CString curText);

		bool SearchDataByText(CTreeCtrl &treeCtrl, HTREEITEM hParent, CString &curText, CString *curData);

		bool CheckDataExist(CTreeCtrl &treeCtrl, HTREEITEM hParent, CString *curData, bool *isExist, CString strShow);

	protected:
		afx_msg void OnDestroy();
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg LRESULT OnCloseControl(WPARAM wParam, LPARAM lParam);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	protected:
		CWnd* _pOwner;				///< 所属父亲窗口
		int _nDroppedWidth;			///< 下拉框宽度(为0时自动适应树展开的宽度, 默认适应控件宽度(-1))
		int _nDroppedHeight;		///< 下拉框高度
		bool _bInCreate;			///< 是否正在创建
		BOOL _bControlActive;		///< 当前是否激活
		CComboBoxTree _dropTree;	///< 下拉树控件
		bool _bCheckMode;			///< 是否是选择框模式
		bool _bShowComboList;		///< 是否展示了下拉框
		bool _bDelItemData;			///< 是否删除附加数据指针
		bool _bInShowDropDown;		///< 是否正在显不拉项
		bool _bSigleCheck;			///< 复选框控制只能选择一个
		bool _bSelectByText;		///< 是否要根据显示值定位焦点行
		CString _splitCh;			///< 多选下拉选项分割符
		CString _strCurSelText;		///< 当前选择的文本（定位用）
		CString _strCurSelData;		///< 当前选择的文本附加数据（定位用）

	private:
		static const CString					s_strNoData;
	};
}