#pragma once

// 锁检测服务
#define LINCENCE_ACCESS L"LicAcc"

/*
* 锁检测服务接口
*/
class ILicenceAccess
{
public:
	virtual ~ILicenceAccess() {}

	/**
	*  @brief    检查指定的许可证是否存在;
	*
	*  @param    nMode 代表可证的宏定义，参考GetSupportLicenceByMode实现
	*/
	virtual bool CheckLicence(int nMode) = 0;
};