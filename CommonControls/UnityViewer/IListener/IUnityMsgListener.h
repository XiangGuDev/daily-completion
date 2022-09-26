//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           IUnityMsgListener.h
//    @author         yf
//    @data           2021/08/16  
//    @brief          unity消息监听接口
//
//*****************************************************


#pragma once

class COMMONCONTROLS_API IUnityMsgListener
{
public:
	IUnityMsgListener();
	virtual ~IUnityMsgListener();
public:
	/**
	*  @brief   接收unity发出的消息
	*
	*  @param    wParam 
	*  @param    wParam 
	*  @return   void
	*/
	virtual void OnReceiveMessage(const std::string szsMsg) = 0;
};

