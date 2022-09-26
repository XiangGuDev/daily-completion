#include "stdafx.h"
#include "ScreenShot.h"

bool CScreenShot::DoScreenShot(LPCTSTR szSaveFileName)
{
	int Status = StartScreenshot(szSaveFileName);
	if (Status == IDOK)
	{
		return true;
	}
	return false;
}
