/*-----------------------------------------------------
* 文件名称：  BCGPPropEx.H
*  功能描述： 扩展属性
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
	class COMMONCONTROLS_API CBCGPPropEx : public CBCGPProp
	{
		friend class CBCGPPropListPro;
	public:
		CBCGPPropEx(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);

		CBCGPPropEx(const CString& strName, const _variant_t& varValue,
			LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
			LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL,
			LPCTSTR lpszValidChars = NULL);

		CBCGPPropEx(const CString& strName, UINT nID, const _variant_t& varValue,
			LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
			LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL,
			LPCTSTR lpszValidChars = NULL);

		virtual ~CBCGPPropEx();

		/**
		*  函数介绍： 绘制属性名
		*
		*  输入参数： CDC* pDC
		*  输入参数： CRect rect
		*/
		virtual void OnDrawName(CDC* pDC, CRect rect);

		/**
		*  函数介绍： 绘制属性值
		*
		*  输入参数： CDC* pDC
		*  输入参数： CRect rect
		*/
		virtual void OnDrawValue(CDC* pDC, CRect rect);

		/**
		*  函数介绍： 创建编辑框
		*
		*  输入参数： CRect rectEdit
		*  输入参数： BOOL& bDefaultFormat
		*/
		virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);

		/**
		*  函数介绍： 设置属性项配置
		*
		*  输入参数： const PROPERTYITEM &ite
		*/
		void SetPropertyCfg(const PROPERTYITEM &item);

		/**
		*  函数介绍： 完成编辑
		*
		*  返回值： BOOL
		*/
		virtual BOOL OnEndEdit();

		/**
		*  函数介绍： 设置数据源
		*
		*  输入参数： IPropertyObjectData *pDataSrc
		*/
		void SetItemData(IPropertyObjectData *pDataSrc);

		/**
		*  函数介绍： 点击属性值
		*
		*  输入参数： UINT uiMsg
		*  输入参数： CPoint point
		*/
		virtual BOOL OnClickValue(UINT uiMsg, CPoint point);

		// 判断当前模型是否被删除
		bool IsDelete();

	protected:

		/**
		*  函数介绍： 绘制超链接
		*
		*  输入参数： CDC* pDC
		*  输入参数： CRect rect
		*  输入参数： const CString &strText
		*/
		void DrawHyperLink(CDC* pDC, CRect rect, const CString &strText);

		/**
		*  函数介绍： 将其他数据类型转换成字符串
		*
		*  返回值：   CString
		*/
		virtual CString FormatProperty();

	protected:
		BOOL						_bHighlighted;	// 高亮
		PROPERTYITEM				_prop;			// 属性项参数
		CFont						_nameFont;		// 属性名字体
		CFont						_valFont;		// 属性值字体
		CFont						_underline;		// 超链接字体
		IPropertyObjectData			*_pDataSrc;		// 数据源指针
		bool								_bDelete = false;
	};
}



