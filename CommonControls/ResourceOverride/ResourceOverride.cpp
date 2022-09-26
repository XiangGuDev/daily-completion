#include "stdafx.h"
#include "ResourceOverride.h"


CResourceOverride::CResourceOverride(void *pAddress)
{
	MEMORY_BASIC_INFORMATION mbi;
	_lastModule = AfxGetResourceHandle();
	if (0 != ::VirtualQuery(pAddress, &mbi, sizeof(mbi)))
	{
		_curModule = (HMODULE)mbi.AllocationBase;
		AfxSetResourceHandle(_curModule);
	}
	else
	{
		_curModule = NULL;
	}
}


CResourceOverride::~CResourceOverride()
{
	AfxSetResourceHandle(_lastModule);
}
