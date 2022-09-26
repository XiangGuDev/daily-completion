#pragma once

#include <string>

class IIStorage
{
public:
	virtual ~IIStorage(){}
	virtual void SaveInt(std::string key, int val) = 0;
	virtual int LoadInt(std::string key) = 0;
};