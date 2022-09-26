/*---------------------------------------------------------------------
*文件名称：IMouseResponseJudgment.h
*功能描述：鼠标响应判断接口
*创建标识：陶子龙2021.10.11.
*
*修改标识：
*修改描述：
----------------------------------------------------------------------*/
#pragma once

class IMouseResponseJudgment
{
public:
	/*
	* 函数介绍：鼠标按下响应判断，是否支持拖拽
	* 输入参数：ITimeLineCtrlDataSource *timeLineCtrlDataSource 鼠标点击行的数据源
	*			int nCol 鼠标点击的列
	* 输出参数：void
	* 返回值  ：bool 是否支持拖拽
	*/
	virtual bool BeginDrag(ITimeLineCtrlDataSource *timeLineCtrlDataSource, int nCol) = 0;

	/*
	* 函数介绍：鼠标弹起响应判断，是否结束拖拽
	* 输入参数：ITimeLineCtrlDataSource *timeLineCtrlDataSource 鼠标点击行的数据源
	*			int nCol 鼠标移动到的列
	* 输出参数：void
	* 返回值  ：void
	*/
	virtual void EndDrag(ITimeLineCtrlDataSource *timeLineCtrlDataSource, int nCol) = 0;
};