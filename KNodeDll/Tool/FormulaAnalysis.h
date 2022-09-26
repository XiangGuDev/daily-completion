/*//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v4.0
//    @file           FormulaAnalysis.H
//    @author         mahaidong
//    @date           2019/2/22 10:37
//    @brief          平台代码，逻辑表达式解析类
//******************************************************/

#pragma once

namespace CommBase
{
	class COMMBASE_API CAnalysisFormula
	{
	public:
		/**
		* 规则库表达式分析构造函数
		* @param [in] formula		分析的表达式
		*/
		CAnalysisFormula(LPCTSTR formula);

		/**
		* & and | 比较
		* @return 返回比较是否成功
		* - true 成功，false 失败
		*/
		bool A_AndOr(void);

	private:
		/**
		* 操作符 比较
		* @return 返回比较是否成功
		* - true 成功，false 失败
		*/
		bool B_Operator(void);

		/**
		* 分析加减算术表达式
		* @return 返回加减值
		*/
		double C_AddSub(void);

		/**
		* 分析加乘除算术表达式
		* @return 返回乘除值
		*/
		double D_MulDiv(void);

		/**
		* 分析数值
		* @return 返回数值
		*/
		double E_Number(void);

	protected:
		CString		_Formula;
		CString		_SrcValue;
		CString		_DesValue;
		double		_MulDivReslut;
		double		_AddSubReslut;
		int			_pos;
		bool		_bOp1;
		bool		_bResult;
	};
}