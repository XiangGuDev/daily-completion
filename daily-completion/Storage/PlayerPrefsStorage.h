#pragma once
#include "IBaseStorage.h"

class PlayerPrefsStorage :
	public IBaseStorage
{
public:
	PlayerPrefsStorage();
	void SaveInt(const CString &key, int val);
	int LoadInt(const CString &key);
	bool DataExist(const CString &key);
private:
	CXmlDocument _doc;
	CString _strDataPath;
};

