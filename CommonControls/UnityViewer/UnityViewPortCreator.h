//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           UnityViewPortCreator.h
//    @author         yf
//    @data           2021/08/06  
//    @brief          unity视图创建器
//
//*****************************************************

#pragma once

#include "IViewPortCreator.h"

class COMMONCONTROLS_API CUnityViewPortCreator : public d3s::views::view::IViewPortCreator
{
public:
	/**
	* 创建视口
	* @return
	*/
	/**
				* 创建视口
				* @return
				*/
	virtual d3s::views::view::IViewPort* CreateViewport(d3s::designfile::model::DesignModel *pModel, d3s::share_ptr<d3s::views::view::CViewInfo> pInfo) override;
};

