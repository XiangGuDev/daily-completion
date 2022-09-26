/*-----------------------------------------------------
* 文件名称：  PropertyBandCtrl.H
*  功能描述： 属性面板
* 创建标识：  张志勇  2020/7/6 8:59
*
* 修改标识：
* 修改描述：
*
* 修改标识：
* 修改描述：
-----------------------------------------------------*/

#pragma once

namespace PropEx
{
	struct tagPropertyStyle;
	typedef tagPropertyStyle* HPROPERTYSTYLE;

	struct tagPropertyItem;
	typedef tagPropertyItem* HPROPERTYITEM;

	class IPropertyObjectData;
	class CBCGPPropListPro;
	class CBCGPPropEx;

	class COMMONCONTROLS_API CPropertyBandCtrl : public CBCGPWnd
	{
		typedef std::map<IPropertyObjectData*, CBCGPProp*> ItemPropMap;

		DECLARE_DYNAMIC(CPropertyBandCtrl)

		DECLARE_SINGLE_CLASS(CPropertyBandCtrl);
	public:
		CPropertyBandCtrl();
		virtual ~CPropertyBandCtrl();

		/**
		*  函数介绍： 创建属性面板控件，创建完成之后调用SetPropertyData传入数据指针，最后调用UpdatePropertyPanel显示属性面板
		*
		*  输入参数： CWnd* pParentWnd  父窗口指针
		*  输入参数： UINT nID		  ID
		*  输入参数： const CRect &rt	  窗口大小
		*  返回值：   BOOL			  创建成功返回TRUE，反之
		*/
		BOOL CreatePropertyCtrl(CWnd* pParentWnd, UINT nID, const CRect &rt);

		/**
		*  函数介绍： 设置属性面板数据（只有设置正确的数据指针之后调用UpdatePropertyPanel才能正确显示）
		*
		*  输入参数： const std::vector<IPropertyObjectData*> &datas 数据指针在属性面板销毁时释放，不需要手动释放
		*/
		void SetPropertyData(const std::vector<IPropertyObjectData*> &datas);

		/**
		*  函数介绍： 更新面板显示
		*
		*  输入参数： bool bUpdate 若为true则重新绘制
		*/
		void UpdatePropertyPanel(bool bUpdate);

		/**
		*  函数介绍： 设置属性面板风格，对属性面板风格有特殊要求可对风格进行设置，不设置则使用默认的风格样式
		*
		*  输入参数： HPROPERTYSTYLE pStyle 由外部传入的指针不用自行释放，窗口销毁时会自行释放
		*/
		void SetPropertyStyle(HPROPERTYSTYLE pStyle);

		/**
		*  函数介绍： 获取属性面板风格
		*
		*  返回值： HPROPERTYSTYLE pStyle
		*/
		HPROPERTYSTYLE GetPropertyStyle();

	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		virtual void PostNcDestroy();

		/**
		*  函数介绍： 释放内存
		*/
		void Release();

		/**
		*  函数介绍： 获取或创建子项
		*
		*  输入参数： IPropertyObjectData *pItem
		*  输入参数： CBCGPProp *pParentGroup
		*  输入参数： PROPERTYITEM &item
		*  返回值： CBCGPProp*
		*/
		CBCGPProp* GetOrCreateBCGPProp(IPropertyObjectData *pItem, CBCGPProp *pParentGroup, PROPERTYITEM &item);

		/**
		*  函数介绍： 收集数据
		*
		*  输入参数： IPropertyObjectData *pItem
		*  输入参数： CBCGPProp *pParentGroup
		*/
		void Collect(IPropertyObjectData *pItem, CBCGPProp *pParentGroup);

		/**
		*  函数介绍： 最小子节点
		*
		*  输入参数： IPropertyObjectData *pItem
		*  输入参数： const PROPERTYITEM &item
		*/
		bool IsLeaf(IPropertyObjectData *pItem, const PROPERTYITEM &item);

		/**
		*  函数介绍： 将外部获取的参数设置到CBCGPProp中
		*
		*  输入参数： CBCGPProp *pPropItem
		*  输入参数： const PROPERTYITEM &item
		*/
		void SetPropItem(CBCGPProp *pPropItem, const PROPERTYITEM &item);

		/**
		*  函数介绍： 创建最小子项
		*
		*  输入参数： IPropertyObjectData *pItem
		*  输入参数： PROPERTYITEM &item
		*/
		CBCGPProp *CreateLeafProp(IPropertyObjectData *pItem, PROPERTYITEM &item);

		/**
		*  函数介绍： 属性面板风格
		*/
		void EnableStyle();

	protected:
		int									_nID;			// ID
		CBCGPPropListPro					*_propList;		// 属性面板控件
		HPROPERTYSTYLE						_pStyle;		// 风格
		ItemPropMap							_itemMap;		// Item指针对应的属性项
		std::vector<IPropertyObjectData*>	_datas;			// 数据组
	};

}
