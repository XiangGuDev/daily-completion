/*---------------------------------------------------------------------
*文件名称：ITimeLineCtrlDataSource.h
*功能描述：时间轴控件数据源
*创建标识：陶子龙2021.9.23.
*
*修改标识：
*修改描述：
----------------------------------------------------------------------*/
#pragma once

//时间帧状态
struct  TimeFrameStatus
{
	TimeFrameStatus(void)
	{
		bKeyFrame = false;
		bTweenAnimation = false;
	}

	bool bKeyFrame;						// 是否为关键帧
	bool bTweenAnimation;				// 是否为补间动画
};
// 时间帧信息列表(key 值为列位置索引，value 值为时间帧状态)
typedef std::map<int, std::shared_ptr<TimeFrameStatus>> TimeFrameRowInfoMap;

typedef std::shared_ptr<TimeFrameRowInfoMap> TimeFrameRowInfoPtr;
typedef std::vector<TimeFrameRowInfoPtr> TimeFrameRowInfoPtrVec;
typedef std::map<int, TimeFrameRowInfoPtr> TimeFrameRowInfoPtrMap;	// key 为当前图层区位置索引

class ITimeLineCtrlDataSource :public IListDataSource
{
	friend class CTimeLineCtrl;
public:
	virtual ~ITimeLineCtrlDataSource(void) {}

public:
	/*
	* 函数介绍：获取时间帧行信息列表
	* 输入参数：size_t nRow 单元格所在行
	* 输出参数：void
	* 返回值  ：TimeFrameRowInfoMap 时间帧行信息列表
	*/
	virtual TimeFrameRowInfoMap& GetTimeFrameRowInfoMap() = 0;

	/*
	* 函数介绍：设置时间帧信息列表
	* 输入参数：size_t nRow 单元格所在行
	*			const TimeFrameRowInfoMap &timeFrameInfoMap 新的时间帧行信息列表
	* 输出参数：void
	* 返回值  ：bool 返回true则修改成功
	*/
	virtual bool SetTimeFrameRowInfo(size_t nRow, const TimeFrameRowInfoMap &timeFrameRowInfoMap) = 0;
};