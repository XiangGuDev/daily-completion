#pragma once

#include <string>

class IBaseStorage
{
public:
	virtual ~IBaseStorage(){}
	virtual void SaveInt(std::string key, int val) = 0;
	virtual int LoadInt(std::string key) = 0;
};