#pragma once

using namespace YFramework;

class IBaseStorage : public IUtility
{
public:
	virtual ~IBaseStorage(){}
	virtual void SaveInt(const CString &key, int val) = 0;
	virtual int LoadInt(const CString &key) = 0;
	virtual bool DataExist(const CString &key) = 0;
};