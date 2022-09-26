//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           PathConfig.h
//    @author         JZL
//    @data           2017/12/29 10:45
//    @brief          平台路径配置类
//
//*****************************************************

#pragma once

namespace CommBase
{
	class COMMBASE_API CPathConfig
	{
	public:
		/**
		*  @brief    获取插件目录
		*
		*  @param	 返回Plugin路径(最后带\\)
		*/
		static CString GetPluginPath();

		/**
		*  @brief    获取AppStart所在目录
		*
		*  @param	 返回AppStart.exe程序路径(最后带\\)
		*/
		static CString GetAppStartPath();

		/**
		*  @brief    获取平台工作目录
		*
		*  @param	 返回平台Workspace程序路径(最后带\\)
		*/
		static CString GetWorkspacePath();

		/**
		*  @brief    获取软件系统目录
		*
		*  @return   CString 返回对应路径(以\结尾)
		*/
		static CString GetSystemPath();

		/**
		*  @brief    获取工程目录
		*
		*  @param	 返回平台工程路径(最后带\\)
		*/
		static CString GetProjectsPath();

		/**
		*  @brief    获取资源库目录
		*
		*  @return	 返回资源库路径(最后带\\)
		*/
		static CString GetResourceLibPath();

		/**
		*  @brief    获取软件资源目录
		*
		*  @return   CString 软件资源目录
		*/
		static CString GetResourcePath();
		
		/**
		*  @brief    获取材质资源目录
		*
		*  @return   CString 材质资源目录
		*/
		static CString GetMaterialPath();

		/**
		*  @brief    获取出错文件存储目录
		*
		*  @return   CString 出错文件存储目录
		*/
		static CString GetDumpPath();
	};
}