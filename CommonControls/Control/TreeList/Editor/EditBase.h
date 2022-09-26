//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           IEditBase.H
//    @author         jzl
//    @data           2015/7/7 17:08
//    @brief          编辑控件基类
//*****************************************************

#pragma once

/**
*  @class    IEditBase
*
*  @brief    编辑控件基类
*/
class IEditBase
{
public:
	virtual ~IEditBase() {}

	/**
	*  @brief    取消编辑
	*
	*/
	virtual void CancelEdit() = 0;
};