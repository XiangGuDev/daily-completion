#pragma once
class COMMONCONTROLS_API CResourceOverride
{
public:
	CResourceOverride(void *pAddress);
	~CResourceOverride();

protected:
	HMODULE _curModule;
	HMODULE _lastModule;
};

#define INIT_RESHANDLE()\
static int curModuleVariable = 0;\
CResourceOverride resOverride(&curModuleVariable);
