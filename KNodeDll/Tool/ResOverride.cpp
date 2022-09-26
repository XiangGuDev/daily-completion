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

#include "stdafx.h"

using namespace CommBase;

	CResourceOverride::CResourceOverride()
	{
		_oldResModule = GetModuleHandle(NULL);
		AfxSetResourceHandle(gInstance);
	}

	CResourceOverride::~CResourceOverride()
	{
		AfxSetResourceHandle(_oldResModule);
	}
