/*-----------------------------------------------------
* 文件名称：  IProperty.H
*  功能描述： 属性面板数据指针
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
	namespace Property {

		// 属性类别
		enum COMMONCONTROLS_API EPropertyCategory
		{
			eGroup,		// 组
			eSimple,	// 单一项，叶子
			eDefalut	// 默认定义方式，有子节点的为组
		};

		// 属性值控件类型
		enum COMMONCONTROLS_API EPropertyItemType
		{
			eNULL,			// 组节点没有属性值不需要控件
			eEdit,			// 普通编辑框
			eDigitEdit,		// 数字编辑框
			eScaleEdit,		// 比例编辑框
			eAngleEdit,		// 角度编辑框
			eComboBox,		// 下拉列表
			eHyperlink,		// 超链接
			eButton,		// 按钮
			eCheckBox		// 复选框
		};
	}
	// 属性面板风格
	typedef struct COMMONCONTROLS_API tagPropertyStyle
	{
	public:
		tagPropertyStyle() :bEnableToolbar(FALSE), bEnableSearch(FALSE), bEnableHeader(FALSE),
			bEnableDesc(FALSE), bEnableModify(FALSE), bEnableMenu(FALSE) {}

		BOOL	bEnableToolbar;		// 显示按类别排序或按字母排序按钮

		BOOL	bEnableSearch;		// 显示搜索框
		CString	strSearchPrompt;	// 搜索框提示文字

		BOOL	bEnableHeader;		// 显示列头，默认不显示
		CString	strLeftName;		// 左侧属性列头名称
		CString strRightName;		// 右侧属性列头名称

		BOOL	bEnableDesc;		// 面板最下方显示属性名描述
		BOOL    bEnableModify;		// 属性值发生修改加粗显示，默认不开启
		BOOL	bEnableMenu;		// 使用右键菜单，默认不开启
	}PROPERTYSTYLE, *HPROPERTYSTYLE;

	typedef struct COMMONCONTROLS_API tagDigitEdit
	{
	public:
		tagDigitEdit() :nDotNum(8), nDigitNum(8), nFlag(eValidEdit | eVeDecimal | eVeNegative | eVeDotNum),
			dMaxVal(DBL_MAX), dMinVal(DBL_MIN) {}

		void Init()
		{
			nDotNum = 8;
			nDigitNum = 8;
			nFlag = eValidEdit | eVeDecimal | eVeNegative | eVeDotNum;
			dMaxVal = DBL_MAX;
			dMinVal = DBL_MIN;
		}

		int					nDotNum;		// 编辑框限制小数位数
		int					nFlag;			// 编辑框标记,EValidEditControlFlag
		int					nDigitNum;		// 最大位数
		double				dMaxVal;		// 最大值
		double				dMinVal;		// 最小值
	}DIGITEDIT, *HDIGITEDIT;

	typedef struct COMMONCONTROLS_API tagPropertyItem
	{
	public:
		tagPropertyItem() :bShow(true), bExpand(TRUE), bReadOnly(FALSE), type(Property::eEdit), cate(Property::eDefalut), bNameFont(FALSE), bValueFont(FALSE),
			valueClr(-1), nameClr(-1), nSel(0), strTrue(L"True"), strFalse(L"False"), nNameAlign(DT_LEFT), nValueAlign(DT_LEFT), bCheckState(TRUE) {}

		void CopyFrom()
		{
			bShow = true;
			bExpand = TRUE;
			bReadOnly = FALSE;
			type = Property::eEdit;
			cate = Property::eDefalut;
			valueClr = -1;
			nameClr = -1;
			bNameFont = FALSE;
			bValueFont = FALSE;
			nSel = 0;
			strTrue = L"True";
			strFalse = L"False";
			digit.Init();
			nNameAlign = DT_LEFT;
			nValueAlign = DT_LEFT;
			bCheckState = TRUE;
		}

		bool						bShow;			// 显示(true)/隐藏(false)
		BOOL						bExpand;		// 展开(TRUE)/收缩(FALSE)
		BOOL						bReadOnly;		// 只读(TRUE)
		BOOL						bNameFont;		// 使用自定义属性名字体，字体一定要正确否则容易出现乱码
		BOOL						bValueFont;		// 使用自定义属性值字体
		BOOL						bCheckState;	// checkbox状态，TRUE为√
		LOGFONT						nameFont;		// 属性名字体，bNameFont设置为TRUE，字体由属性面板释放，不必在外部操作
		LOGFONT						valueFont;		// 属性值字体，bValueFont设置为TRUE
		int							nSel;			// ComboBox默认选择项
		int							nNameAlign;		// 属性名对齐方式三种：DT_LEFT、DT_CENTER、DT_RIGHT
		int							nValueAlign;	// 属性值对齐方式
		DIGITEDIT					digit;			// 数字编辑框配置，包含：小数位数，最大值，最小值等
		COLORREF					nameClr;		// 属性名颜色
		COLORREF					valueClr;		// 属性值颜色	
		Property::EPropertyItemType	type;			// 属性值列控件类型
		Property::EPropertyCategory	cate;			// 项类别
		CString						strItemName;	// 属性名称，组节点只有name，没有value
		CString						strTrue;		// 只用于属性值为bool类型时，当为true时对应显示的字符串，默认为"True"
		CString						strFalse;		// 只用于属性值为bool类型时，当为false时对应显示的字符串，默认为"False"
		std::vector<CAny>			options;		// combox集合
	}PROPERTYITEM, *HPROPERTYITEM;


	class COMMONCONTROLS_API IPropertyObjectData
	{
	public:
		virtual ~IPropertyObjectData() {}

		/**
		*  函数介绍： 获取数据源子节点数量
		*
		*  返回值：   size_t
		*/
		virtual size_t GetPropertyCount() = 0;

		/**
		*  函数介绍： 获取对应位置子节点
		*
		*  输入参数： size_t nIndex  子节点索引
		*  返回值：  IPropertyData*  返回子节点指针
		*/
		virtual IPropertyObjectData* GetPropertyItem(size_t nIndex) = 0;

		/**
		*  函数介绍： 获取子节点配置信息，通过HPROPERTYITEM指针可设置属性项各个属性，如：是否显示、是否可读、名称等
		*
		*  输入参数： HPROPERTYITEM pItem
		*/
		virtual void GetItemConfig(HPROPERTYITEM pItem) = 0;

		/**
		*  函数介绍： 获取属性值
		*
		*  返回值：   CString
		*/
		virtual CAny GetItemValue() = 0;

		/**
		*  函数介绍： 给属性项设置新值
		*
		*  输入参数： const CAny &newVal
		*  输入参数： const CAny &oldVal
		*/
		virtual void SetItemValue(const CAny &newVal, const CAny &oldVal) = 0;

		/**
		*  函数介绍： 超链接点击事件
		*
		*  输入参数： const PROPERTYITEM & item
		*/
		virtual void HyperlinkPressDown(const PROPERTYITEM &item) {};

		/**
		*  函数介绍： 按钮点击事件
		*
		*  输入参数： const PROPERTYITEM & item
		*/
		virtual void ButtonPressDown(const PROPERTYITEM &item) {};

		/**
		*  函数介绍： 点击右侧Item项
		*
		*  输入参数： const PROPERTYITEM & item
		*/
		virtual void ClickRValItem(const PROPERTYITEM &item) {}
	};

	class COMMONCONTROLS_API IPropertyObject
	{
	public:
		virtual ~IPropertyObject() {}

		/**
		*  函数介绍： 获取数据节点指针
		*
		*  返回值： IPropertyObjectData*
		*/
		virtual std::vector<IPropertyObjectData*> GetPropertyDataList() = 0;

		/**
		*  函数介绍： 获取工程属性数据节点指针
		*
		*  返回值： std::vector<IPropertyObjectData*>
		*/
		virtual std::vector<IPropertyObjectData*> GetProjectPropertyDataList() = 0;
	};

}

