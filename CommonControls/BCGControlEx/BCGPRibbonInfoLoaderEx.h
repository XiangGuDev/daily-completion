//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2016 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonInfoLoader.h: interface for the CBCGPRibbonInfoLoader class.
//
//////////////////////////////////////////////////////////////////////


#pragma once
#include "BCGPRibbonInfo.h"
#include "BCGPBaseInfoEx.h"
#include <map>

#ifndef BCGP_EXCLUDE_RIBBON

class CBCGPRibbonInfoLoaderEx: public controls::CBCGPBaseInfoLoaderEx
{
public:
	CBCGPRibbonInfoLoaderEx (CBCGPRibbonInfo& info, 
		DWORD dwFlags = (CBCGPRibbonInfo::e_UseRibbon | CBCGPRibbonInfo::e_UseStatus));
	virtual ~CBCGPRibbonInfoLoaderEx();

	virtual BOOL LoadFromBuffer (LPCTSTR lpszBuffer);

	virtual BOOL LoadImage (CBCGPRibbonInfo::XImage& image, BOOL bSingle = FALSE);

protected:
	virtual BOOL LoadImage (const CBCGPRibbonInfo::XID& id, CBCGPToolBarImages& image, BOOL bSingle);

	CBCGPRibbonInfo& GetRibbonInfo () const
	{
		return (CBCGPRibbonInfo&)GetInfo ();
	}
protected:
	LPCTSTR _lpszBuffer;
};

#endif // BCGP_EXCLUDE_RIBBON

