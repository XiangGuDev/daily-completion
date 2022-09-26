//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           LogOutput.H
//    @author         fanHong
//    @date           2017/11/1 11:12
//    @brief          日志输出
//
//*****************************************************

#pragma once

namespace CommBase
{
	/*
	* 日志记录类型
	*/
	enum ELogType
	{
		eLogError = 1,		  //写错误信息(记录在执行遇到的已知错误)
		eLogWarning = 2,     //写警告信息(记录可能导致软件错误的信息)
		eLogInfo = 4,		  //写一般信息(记录业务逻辑相关信息)
		eLogSystem = 8,	  //写系统信息(记录平台或框架输出信息)
	};

	/**
	*  @class    日志输出类
	*
	*  @brief
	*/
	class COMMBASE_API CLog
	{
	public:
		/**
		*  @brief    初始化日志
		*
		*  @param    LPCWSTR lpLogPath 日志目录(以\结尾)
		*  @param    int nLogType 记录日志类型
		*  @return
		*/
		static bool InitLog(LPCWSTR lpLogPath, int nLogType =
			(eLogError | eLogWarning | eLogInfo | eLogSystem));

		/**
		*  @brief    反初始化日志
		*
		*  @return
		*/
		static void UninitLog();

	public:
		/**
		*  @brief    写错误信息
		*
		*  @param    LPCTSTR pszLog 日志信息
		*  @param    ...
		*/
		static void LogErrorV(LPCTSTR pszLog, ...);

		/**
		*  @brief    写错误信息
		*
		*  @param    LPCWSTR lpLog 日志信息
		*/
		static void LogError(LPCWSTR lpLog);

		/**
		*  @brief    写警告信息
		*
		*  @param    LPCTSTR pszLog 日志信息
		*  @param    ...
		*/
		static void LogWarningV(LPCTSTR pszLog, ...);

		/**
		*  @brief    写警告信息
		*
		*  @param    LPCWSTR lpLog 日志信息
		*/
		static void LogWarning(LPCWSTR lpLog);

		/**
		*  @brief    写一般信息
		*
		*  @param    LPCTSTR pszLog 日志信息
		*  @param    ...
		*/
		static void LogInfoV(LPCTSTR pszLog, ...);

		/**
		*  @brief    写一般信息
		*
		*  @param    LPCWSTR lpLog 日志信息
		*/
		static void LogInfo(LPCWSTR lpLog);

		/**
		*  @brief    写系统信息
		*
		*  @param    LPCTSTR pszLog 日志信息
		*  @param    ...
		*/
		static void LogSystemV(LPCTSTR pszLog, ...);

		/**
		*  @brief    写系统信息
		*
		*  @param    LPCWSTR lpLog 日志信息
		*/
		static void LogSystem(LPCWSTR lpLog);

		/**
		*  @brief    设置写日志级别
		*
		*  @param    int nLogType 日志记录类别
		*/
		static void SetLogType(int nLogType);

		/**
		*  @brief    测试性能
		*
		*  @param    bDone true表示显示结果
		*/
		static void LogPerfermance(bool bDone = false);

	private:
		/**
		*  @brief    写日志到文件
		*
		*  @param    LPCWSTR lpLog
		*/
		static void LogToFile(LPCWSTR lpLog);

		/**
		*  @brief    生成日志文件
		*
		*  @return   bool 成功返回true
		*/
		static bool GenerateLogFile();
	};
}