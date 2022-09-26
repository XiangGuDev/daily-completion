//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           TypeDefine.H
//    @author         fanHong
//    @date           2017/11/2 16:58
//    @brief          公共类型定义
//
//*****************************************************

#include "Guid.h"

namespace CommBase
{
	// 字符串数组
	typedef std::vector<CString> StringArray;
	typedef StringArray::iterator StringArrayIter;
	
	// 字符串数据字典
	typedef std::map<CString, CString>	StringMap;
	typedef StringMap::iterator			StringMapIter;
	typedef StringMap::const_iterator	StringMapCIter;

	// 计算数组大小
	#define ARRAY_SIZE(x) sizeof((x))/sizeof((x)[0])

	// 安全删除对象
	#define SAFE_DELETE(x) if(NULL != (x)){delete (x); (x) = NULL;}

	// 安全删除数组对象
	#define SAFE_DELETE_ARRAY(x) if(NULL != (x)){delete [] (x); (x) = NULL;}

	// 测试是否等于0
	#define TZERO(x) (!((x)!=0))
	#define TZERO2(x) (((x) < 0.01) && ((x) > -0.01))
	#define TZERO3(x) (((x) < 0.001) && ((x) > -0.001))
	#define TZERO4(x) (((x) < 0.0001) && ((x) > -0.0001))
	#define TZERO6(x) (((x) < 0.000001) && ((x) > -0.000001))
	#define TZERO8(x) (((x) < 0.00000001) && ((x) > -0.00000001))
	#define TZERO12(x) (((x) < 0.000000000001) && ((x) > -0.000000000001))

	// 小数相等判断
	#define FLOAT_EQUAL2(x, y) TZERO2((x)-(y))
	#define FLOAT_EQUAL3(x, y) TZERO3((x)-(y))
	#define FLOAT_EQUAL4(x, y) TZERO4((x)-(y))
	#define FLOAT_EQUAL6(x, y) TZERO6((x)-(y))
	#define FLOAT_EQUAL8(x, y) TZERO8((x)-(y))
	#define FLOAT_EQUAL12(x, y) TZERO12((x)-(y))

		// 定义重力加速度
	#define GRAVITATION 9.80665

		// 度分割符号
	#define DEGREE_CHAR _T('°')

		// 分分割符号
	#define MINUTE_CHAR _T('′')

		// 秒分割符号
	#define SECOND_CHAR _T('″')

		//比例分隔符号
	#define SCALE_CHAR _T(':')

	#ifndef AdInt32
	#define AdInt32 long
	#endif

	// 属性名称分割符号，属性名称格式“索引=属性类型=属性英文描述=属性名称”
	#define PROP_SPLITE_CHAR		L"§"
}