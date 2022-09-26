#include "stdafx.h"
#include "IUnityMsgListener.h"

IUnityMsgListener::IUnityMsgListener()
{
	CUnityViewPort::Instance()->AddUnityMsgListener(this);
}

IUnityMsgListener::~IUnityMsgListener()
{
	CUnityViewPort::Instance()->RemoveUnityMsgListener(this);
}
