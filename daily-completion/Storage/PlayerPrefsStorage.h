#pragma once
#include "IStorage.h"
class PlayerPrefsStorage :
	public IIStorage
{
public:
	void SaveInt(std::string key, int val);
	int LoadInt(std::string key);
private:
	CXmlDocument _doc;
	CString _strDataPath;
};

