//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           Guid.H
//    @author         fanHong
//    @date           2017/11/2 15:57
//    @brief          GUID包装
//
//*****************************************************

#pragma once

namespace CommBase
{
	/**
	*  @class    GUID生成Hash值
	*
	*  @brief
	*/
	struct HashGuid
	{
		/**
		*  @brief    Hash值生成
		*
		*/
		size_t operator()(const GUID& guid) const;
	};

	
	/**
	*  @class    GUID包装类
	*
	*  @brief
	*/
	class COMMBASE_API CGuid
	{
	public:
		CGuid();
		CGuid(const GUID& guid);
		CGuid(LPCWSTR strId);

	public:
		/**
		*  @brief    更换当前的GUID
		*
		*/
		void ChangeNew();

		/**
		*  @brief    GUID转String
		*
		*  @param    bool bStand 是否字符串之间使用"-"连接，默认不使用
		*  @return   CString 返回GUID字符串
		*/
		CString ToString(bool bStand = false) const;

		/**
		*  @brief    清除GUID数据(IsEmpty返回TRUE)
		*
		*  @return   const CGuid&
		*/
		CGuid& Clear();

		/**
		*  @brief    判断GUID是否为空
		*
		*  @return   bool 为空返回true
		*/
		bool IsEmpty() const;

		/**
		*  @brief    转换成CString
		*
		*/
		operator CString() const;

	public:
		/**
		*  @brief    转换成GUID
		*
		*/
		operator GUID() const;

		/**
		*  @brief    用于Map判断大小
		*
		*  @param    const CGuid & guid CGuid值
		*  @return   bool 当前值小于参数值返回true
		*/
		bool operator < (const CGuid& guid) const;

		/**
		*  @brief    GUID赋值
		*
		*  @param    const GUID & guid CGuid值
		*  @return   CGuid& 返回引用
		*/
		CGuid& operator = (const CGuid& guid);

		/**
		*  @brief    对象相等判断
		*
		*  @param    const CGuid & guid
		*  @return   bool 相等返回true
		*/
		bool operator == (const CGuid& guid) const;

		/**
		*  @brief    对象不相等判断
		*
		*  @param    const CGuid & guid
		*  @return   bool 不相等返回true
		*/
		bool operator != (const CGuid& guid) const;

	private:
		GUID	_guid;	///<GUID
	};
}