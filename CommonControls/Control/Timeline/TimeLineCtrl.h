/*---------------------------------------------------------------------
*文件名称：TimeLineCtrl.h
*功能描述：时间轴控件
*创建标识：陶子龙2021.9.23.
*
*修改标识：
*修改描述：
----------------------------------------------------------------------*/
#pragma once
#include "ITimeLineCtrlDataSource.h"

class COMMONCONTROLS_API CTimeLineCtrl :public CWnd
{
public:
	CTimeLineCtrl(void);
	virtual ~CTimeLineCtrl(void);

protected:
	DECLARE_DYNAMIC(CTimeLineCtrl)
	DECLARE_MESSAGE_MAP()

public:
	/*
	* 函数介绍：创建时间轴控件
	* 输入参数：const RECT& rect 控件区域
	*			CWnd* pParentWnd 父窗口
	*			UINT nID(0xfff0) 控件ID
	*			DWORD dwStyle(WS_BORDER | WS_CHILD | WS_VISIBLE) 控件风格
	* 输出参数：void
	* 返回值  ：bool 是否创建成功
	*/
	bool Create(const CRect& rect, CWnd* pParentWnd, UINT nID = NULL,
		DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE);

	/*
	* 函数介绍：加载列表配置文件
	* 输入参数：const CStirng & strCfg 配置文件路径
	*			const CString&  strCfgName 图层配置名(一个配置文件存在多个配置,为空则读第一个)
	* 输出参数：void
	* 返回值  ：bool 成功返回true
	*/
	bool LoadConfig(const CString& strCfg, const CString& strLayerCfgName = L"");

	/*
	* 函数介绍：设置数据源
	* 输入参数：ITimeLineCtrlDataSource *pDataSource 数据源指针
	*			bool bDelDataSource 是否删除数据源指针
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetDataSource(ITimeLineCtrlDataSource *pDataSource, bool bDelDataSource);

	/*
	* 函数介绍：更新控件
	* 输入参数：bool bDataChange 当为true是则更新行数据，否则直刷新界面
	* 输出参数：void
	* 返回值  ：void
	*/
	void UpdateCtrl(bool bDataChange = false);

	/*
	* 函数介绍：设置事件通知父窗口
	* 输入参数：HWND hNotifyWnd 通知窗口句柄
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetNofityWnd(HWND hNotifyWnd);

	/*
	* 函数介绍：设置左侧图层区哪列为树
	* 输入参数：int nColumnIndex(0) 左侧图层区树的列
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetTreeColumn(int nColumnIndex = 0);

	/*
	* 函数介绍：设置鼠标响应判断接口
	* 输入参数：IMouseResponseJudgment *pMouseResponseJudgement 鼠标响应判断接口
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetMouseResponseJudgment(IMouseResponseJudgment *pMouseResponseJudgement);

	/*
	* 函数介绍：设置时间帧刻度的长度
	* 输入参数：int nTimeFrameLength 时间帧刻度的长度
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetTimeFrameLength(int nTimeFrameLength);

	/*
	* 函数介绍：获取时间帧刻度的长度
	* 输入参数：void 
	* 输出参数：void
	* 返回值  ：int 时间帧刻度的长度
	*/
	int GetTimeFrameLength(void);

	/*
	* 函数介绍：获取数据源列表
	* 输入参数：void
	* 输出参数：DataSourceArray &datasourceArray 数据源列表
	* 返回值  ：void
	*/
	void GetDataSourceArray(DataSourceArray &datasourceArray);

	/*
	* 函数介绍：获取数据源标识与控件数据(记录状态数据)
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：ListDataSource 数据源标识与控件数据(记录状态数据)
	*/
	ListDataSource GetListDataSource(void);

	/**
	*  @brief    获取选中的单个数据源对象
	*
	*  @return   IListDataSource* 选中的数据源对象，未选中则返回NULL
	*/
	IListDataSource* GetSelectItem();

	//设置左侧树焦点行
	void SetLayerAreaFocusItem(int nIndex);

	//获取左侧树焦点行索引
	int GetLayerAreaSelectedIndex();
private:
	/*
	* 函数介绍：注册控件类
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：bool 是否成功注册
	*/
	virtual bool RegisterTimeLineCtrl(void);

protected:
	//窗口尺寸大小变化响应事件
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	CTimeLineSplitCtrl					_verSplit;					// 分隔条控件
	CTimeLineTreeListCtrl				_treeLayerAreaCtrl;			// 左侧图层区控件
	CTimeFrameListCtrl					_timeFrameListCtrl;			// 右侧时间帧列表控件

protected:
	ListDataSource						_listDataSource;			// 数据源标识与控件数据(记录状态数据)
	bool								_bDelDataSource;			// 是否删除数据源指针
};