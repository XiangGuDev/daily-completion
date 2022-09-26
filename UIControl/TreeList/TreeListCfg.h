//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           TreeListCfg.H
//    @author         KG
//    @date           2015/3/23 14:23
//    @brief          树型列表配置加载器
//*****************************************************

#pragma once

using namespace CommBase;

#include <vector>

namespace ControlUI
{
	// 单元格数据
	struct tagCELLDINFO;
	typedef tagCELLDINFO* HCELLINFO;

	// 行数据
	struct tagITEMDATA;
	typedef tagITEMDATA* HITEMDATA;

	// 列头数据
	struct tagCOLUMNDATA;
	typedef tagCOLUMNDATA* HCOLUMNDATA;

	// 编辑配置数据
	struct tagEDITCONFIG;
	typedef tagEDITCONFIG* HEDITCONFIG;

	// 单元格配置数据
	struct tagCELLCONFIG;
	typedef tagCELLCONFIG* HCELLCONFIG;

	class CTreeListCtrl;

	// 单元格配置二维数组
	typedef std::vector<HCELLCONFIG> CellConfigVector;
	typedef CellConfigVector::iterator CellConfigVectorIter;

	typedef std::vector<CellConfigVector> CellConfigVectors;
	typedef CellConfigVectors::iterator	CellConfigVectorsIter;
	typedef CellConfigVectors::const_iterator CellConfigVectorsConstIter;

	enum EFormatType
	{
		FORMAT_LEFT = 0,		/**< 左对齐 */
		FORMAT_MIDDLE,			/**< 居中对齐 */
		FORMAT_RIGHT,			/**< 右对齐 */
	};

	DECL_DYN_ENUM(EFormatType);
	DECL_DYN_ENUM(ETreeListEditFlag);
	DECL_DYN_ENUM(EValidEditControlFlag);
	DECL_DYN_ENUM(ETreeComboEditControlFlag);
	DECL_DYN_ENUM(EProgressControlFlag);
	DECL_DYN_ENUM(EDateTimeEditFlag);

	/**
	*  @class    配置加载器
	*
	*  @brief
	*/
	class CONTROL_UI CTreeListConfig
	{
	public:
		CTreeListConfig(CTreeListCtrl* pTreeListCtrl);
		virtual ~CTreeListConfig();

	public:
		/**
		*  @brief    获取树型列表控件
		*
		*  @return   CTreeListCtrl* 树型列表控件
		*/
		CTreeListCtrl* GetTreeListCtrl();

		/**
		*  @brief    加载树型列表配置文件
		*
		*  @param    const CString & strCfgFile 配置文件
		*  @param    const CString & strCfgName 配置名
		*  @return   bool 成功返回true
		*/
		bool LoadConfig(const CString& strCfgFile, const CString& strCfgName);

		/**
		*  @brief    测试是否在节点过滤数组中
		*
		*  @param    int nType 数据源节点类型
		*  @return   bool 查找到返回true
		*/
		bool IsTypeInFilter(int nType);

		/**
		*  @brief    获取单元格配置数据
		*
		*  @return   const CellConfigVectors& 返回配置数据
		*/
		const CellConfigVectors& GetCellConfigVectors();

		/**
		*  @brief    获取对应行列配置数据
		*
		*  @param    size_t nRow 表格行索引
		*  @param    size_t nCol 表格列索引
		*  @param	 HCELLCONFIG& pCellCfg  配置数据
		*  @return   成功返回true
		*/
		bool GetCellConfigByRowCol(size_t nRow, size_t nCol, HCELLCONFIG& pCellCfg);

		/**
		*  @brief    设置临时附加数据（用于控件编辑临时使用）
		*
		*  @param    const CVariantData& xData 临时数据
		*/
		void SetTempData(const CString& strTmpData);

		/**
		*  @brief    设置临时附加数据（用于控件编辑临时使用）
		*
		*  @return    CVariantData 返回临时数据
		*/
		const CString& GetTempData();

	private:
		/**
		*  @brief    加载树型列表表头
		*
		*  @param    CXmlElement * pColumnCfg 列表头XML节点
		*  @param    HCOLUMNDATA hPrarentColumn 父表头
		*  @return   bool 成功返回true
		*/
		bool LoadColumn(CXmlElement* pColumnCfg, HCOLUMNDATA hPrarentColumn);

		/**
		*  @brief    加载列表自由行数据
		*
		*  @param    CXmlElement * pListData 行XML节点
		*/
		void LoadListData(CXmlElement* pListData);

		/**
		*  @brief    加载节点类型过滤器
		*
		*  @param    CXmlElement * pTypeFilter 类型过滤器XML节点
		*/
		void LoadTypeFilter(CXmlElement* pTypeFilter);

		/**
		*  @brief    加载单元格编辑配置
		*
		*  @param    CXmlElement* pCellCfg 单元配置
		*  @param    HEDITCONFIG hEditConfig 单元格编辑数据
		*  @param    HEDITCONFIG hColEditConfig 列所在编辑数据
		*/
		void LoadEditConfig(CXmlElement* pCellCfg, HEDITCONFIG hEditConfig,
			HEDITCONFIG hColEditConfig = NULL);

		/**
		*  @brief    获取字符串颜色
		*
		*  @param    const CString & strColor 字符串颜色
		*  @return   COLORREF 返回字符串颜色
		*/
		COLORREF GetStringColor(const CString& strColor);

		/**
		*  @brief    清除单元格配置二维数组
		*
		*/
		void ClearCellConfigVectors();

		/**
		*  @brief    根据Format关键字的字符串获取枚举值
		*
		*  @param    const CString & 输入的字符串
		*  @return   int 输出的枚举值
		*/
		int GetFormatEnum(const CString& str);

		/**
		*  @brief    根据Option关键字的字符串获取枚举值
		*
		*  @param    const CString & 输入的字符串
		*  @return   int 输出的枚举值
		*/
		int GetOptionEnum(const CString& str);

	private:
		CString _strTmpData;	// 用于传递树控件编辑时附加数据
		std::vector<int> _typeFilter;	///< 数据源节点类型过滤数组
		CTreeListCtrl* _pTreeListCtrl;	///< 所在树控件
		CellConfigVectors _cellCfgVectors; ///< 单元格配置二维数组
		double _colWdithScale; ///< 列头字体缩放比例(125%)
	};
}