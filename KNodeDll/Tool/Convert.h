//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           DynEnum.H
//    @author         fanHong
//    @date           2017/11/3 15:32
//    @brief          动态枚举
//
//*****************************************************

#pragma once

namespace CommBase
{
	//! 数据转换类
	class COMMBASE_API CConvert
	{
	public:
		/**
		* 比较两个浮点数是否相等
		* @param [in] var1	浮点数1
		* @param [in] var2	浮点数2
		* @param [in] dot	小数位数，默认为-1，表示不四舍五入（差不大于0.000001即认为相等）
		* @return	返回两个浮点数是否相等
		* - true 相等，false 不相等
		*/
		static bool FloatEqual(double var1 , double var2 , int dot = -1);

		/**
		* 浮点数四舍五入
		* @param [in] num		要格式化的数值
		* @param [in] dot		小数位数，为 -1 表示是不四舍五入
		* @return	返回格式化后的数值
		*/
		static double FloatRound(double num , int dot);

		/**
		* 浮点数四舍五入成字符串
		* @param [in] num		要格式化的数值
		* @param [in] dot		小数位数，为 -1 表示是不四舍五入
		* @return	返回浮点数格式化后的字符串
		*/
		static CString Float2Text(double num , int dot = -1);

		/**
		* 浮点数四舍五入成字符串
		* @param [in] num		要格式化的数值
		* @param [in] dot		小数位数，为 -1 表示是不四舍五入（实际由于C++库的原因只输出6位小数）
		* @param [in] buffer	输出缓存
		* @param [in] FullZero	如果小数位不够，是否用 0 填充
		* @return	返回浮点数格式化后的字符串
		*/
		static LPCTSTR Float2Text(double num , TCHAR* buffer , int dot = -1 , bool FullZero = false);

		/**
		* 浮点数四舍五入成字符串
		* @param [in] num		要格式化的数值
		* @param [in] dot		小数位数，为 -1 表示是不四舍五入
		* @param [in] FullZero	如果小数位不够，是否用 0 填充
		* @return	返回浮点数格式化后的字符串
		*/
		static CString Float2Text(double num , int dot , bool FullZero);

		/**
		* long 转换成字符串
		* long范围：-2,147,483,648 到 2,147,483,647
		* @param [in] val		要格式化的数值
		* @return	返回long格式化后的字符串
		*/
		static CString Long2Text(long val);

		/**
		* __int64 转换成字符串
		* -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
		* @param [in] val		要格式化的数值
		* @return	返回__int64格式化后的字符串
		*/
		static CString I642Text(__int64 val);

		/**
		* bool 转换成字符串
		* @param [in] val		要格式化的数值
		* @return	返回bool格式化后的字符串
		*/
		static LPCTSTR Bool2Text(BOOL val);

		/**
		* 字符串 转换成bool
		* val转为大写情况下，val是“TRUE”或“T”返回true，否则返回false
		* @param [in] val		要格式化的数值
		* @return	返回字符串格式化后的bool
		*/
		static bool Text2Bool(LPCTSTR val);

		/**
		*  @brief    double 取整
		*
		*  @param    double dNum 被取整的数字
		*  @return   double		 结果
		*/
		static double FloatQuZheng(double dNum);

		/**
		* 浮点数四舍五入成字符串
		* @param [in] num		要格式化的数值
		* @param [in] dot		小数位数，为 -1 表示是不四舍五入
		* @param [in] FullZero	如果小数位不够，是否用 0 填充
		* @return	返回浮点数格式化后的字符串
		*/
		static CString Float2NoZeroText(double num , int dot , bool FullZero);
	};
}