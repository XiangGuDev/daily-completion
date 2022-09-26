//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           ResOverride.H
//    @author         JZL
//    @date           2018/5/15 09:32
//    @brief          DLL资源切换
//
//*****************************************************

#pragma once

namespace CommBase
{
	/*
	* 自动切换资源到当前DLL中
	*/
	class COMMBASE_API CResourceOverride
	{
	public:
		CResourceOverride();
		~CResourceOverride();
	protected:
		HMODULE _oldResModule;
	};

	#define RESOURCE_OVERRIDE CResourceOverride resOverride;
}