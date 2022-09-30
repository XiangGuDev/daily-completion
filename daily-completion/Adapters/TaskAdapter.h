#pragma once
#include "../Base/DCController.h"

class Task;
namespace ControlUI
{
	class CTaskAdapter : public IListDataSource, public DCController
	{
	public:
		/**
		*  @brief    构造函数
		*
		*  @param    pRoot 根节点指针
		*  @param    _bFindMode 是否为查找模式
		*  @return
		*/
		CTaskAdapter(std::shared_ptr<Task> task, bool bRoot = true)
		{
			_bRoot = bRoot;
			_task = task;
		}
	public:

		/**
		*  @brief    获取子项数目
		*
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*  @return   size_t 返回子项数目
		*/
		size_t GetCount(CTreeListConfig* pConfig);

		/**
		*  @brief    唯一标识
		*/
		virtual size_t GetKey();

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
			HITEMDATA hItemData, CTreeListConfig* pConfig);

		/**
		*  @brief    获取单元格数据信息
		*
		*  @param    size_t nCol 单元格所在列
		*  @param    size_t nRow 单元格所在行
		*  @param    const HCOLUMNDATA hColumnData 列头信息
		*  @param    HCELLDATA hCellData 返回单元格数据数
		*  @param    HEDITCONFIG hEditConfig 返回编辑配置数据
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*/
		virtual void GetCellData(size_t nCol, size_t nRow, const HCOLUMNDATA hColumnData,
			HCELLINFO hCellData, HEDITCONFIG hEditConfig, CTreeListConfig* pConfig);

		/**
		*  @brief    获取单元格文本数据
		*
		*  @param    size_t nCol 单元格所在列
		*  @param    size_t nRow 单元格所在行
		*  @param    const HEDITCONFIG hEditConfig 编辑配置数据
		*  @param    CTreeListConfig*& pConfig 树列表控件配置
		*  @return   CString 返回单元格文本数据
		*/
		virtual CString GetCellText(size_t nCol, size_t nRow, const HEDITCONFIG hEditConfig,
			CTreeListConfig* pConfig);

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
		virtual bool SetCellText(size_t nCol, size_t nRow, const HEDITCONFIG hEditConfig,
			const CString& strNewValue, CTreeListConfig* pConfig);
	private:
		bool _bRoot; // 是否是根节点
		std::shared_ptr<Task> _task;
	};
}
