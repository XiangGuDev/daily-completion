#pragma once
#include "IBaseStorage.h"

class PlayerPrefsStorage :
	public IBaseStorage
{
public:
	PlayerPrefsStorage();
	void SaveInt(std::string key, int val);
	int LoadInt(std::string key);
private:
	CXmlDocument _doc;
	CString _strDataPath;
};

