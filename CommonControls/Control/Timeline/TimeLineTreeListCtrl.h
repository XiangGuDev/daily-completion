/*---------------------------------------------------------------------
*文件名称：TimeLineTreeListCtrl.h
*功能描述：时间轴控件需要的派生类树列表控件
*创建标识：陶子龙2021.9.28.
*
*修改标识：
*修改描述：
----------------------------------------------------------------------*/
#pragma once

class CTimeLineTreeListCtrl :public CTreeListCtrl
{
public:
	CTimeLineTreeListCtrl(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	/*
	* 函数介绍：获取数据源标识与控件数据(记录状态数据)
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：ListDataSource 数据源标识与控件数据(记录状态数据)
	*/
	ListDataSource GetListDataSource(void);

	/*
	* 函数介绍：更新滚动条
	* 输入参数：int scrollMode 需要更新的滚动条
	* 输出参数：void
	* 返回值  ：void
	*/
	virtual void UpdateScrollBar(int scrollMode = SB_HORZ) override;

	/*
	* 函数介绍：绘制行
	* 输入参数：CMemoryDC & dcMemory 内存DC
	*			const CRect& rcHeader 表头绘制区域
	*			const CRect& rcList 除去表头的绘制区域
	* 输出参数：void
	* 返回值  ：void
	*/
	virtual void DrawRow(CMemoryDC& dcMemory, const CRect& rcHeader,
		const CRect& rcList) override;

public:
	/*
	* 函数介绍：设置对应的时间轴控件中的右侧时间帧控件
	* 输入参数：CTimeFrameListCtrl *pTimeFrameCtrl 对应的时间轴控件中的右侧时间帧控件
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetCorrespondingTimeFrameCtrl(CTimeFrameListCtrl *pTimeFrameCtrl);

protected:
	//鼠标左键按下响应事件
	void OnLButtonDown(UINT nFlags, CPoint point);
		
	/**
	*  @brief    项父级发送Notify通知消息
	*
	*  @param    UINT nMessage 消息ID
	*  @param    int nClickType 左击 = 0，右击 = 1
	*  @param    int nCol 编辑列
	*  @param    int nRow 编辑行
	*  @param    HEDITCONFIG hEditCfg 单元格信息
	*  @param    const CString&  strOldText 修改前单元格文本
	*  @param    CString* pStrNewText 修改后单元格文本指针
	*  @return   LRESULT 事件通知结果
	*/
	LRESULT NotifyParent(UINT nMessage, int nCol = -1, int nRow = -1, HEDITCONFIG hEditCfg = NULL,
		const CString& strCellText = L"", CString* pStrNewText = NULL, int nClickType = 0);


protected:
	CTimeFrameListCtrl*				_pTimeLineCtrl;					// 对应的时间轴控件中的右侧时间帧控件
};