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
// BCGPRibbonInfoLoader.cpp: implementation of the CBCGPRibbonInfoLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonInfoLoaderEx.h"
#include "BCGPTagManager.h"
#include "RibbonBarImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef BCGP_EXCLUDE_RIBBON


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonInfoLoaderEx::CBCGPRibbonInfoLoaderEx (CBCGPRibbonInfo& info,
											  DWORD dwFlags)
	: CBCGPBaseInfoLoaderEx(info, _T("BCGP_RIBBON_XML"), dwFlags)
{
	_lpszBuffer = NULL;
}

CBCGPRibbonInfoLoaderEx::~CBCGPRibbonInfoLoaderEx()
{
}

BOOL CBCGPRibbonInfoLoaderEx::LoadFromBuffer (LPCTSTR lpszBuffer)
{
	if (lpszBuffer == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRes = TRUE;

	// 记录自定义区业务信息
	CRibbonBarImage::GetInst()->InitInfo();
	_lpszBuffer = lpszBuffer;
	CBCGPRibbonInfo& info = GetRibbonInfo ();
	if (info.FromTag (lpszBuffer))
	{
		CBCGPRibbonInfo::XArrayImages images;
		info.GetArrayImages (images);

        CBCGPRibbonInfo::XImage* pMainImage = NULL;
        CBCGPRibbonInfo::XImage* pMainImageScenic = NULL;
        if (info.GetRibbonBar().m_btnMain != NULL)
        {
            pMainImage = &info.GetRibbonBar().m_btnMain->m_Image;
			pMainImageScenic = &info.GetRibbonBar().m_btnMain->m_ImageScenic;
        }
		
		int i = 0;
		for (i = 0; i < images.GetSize (); i++)
		{
			if (!LoadImage(*images[i], (images[i] == pMainImage || images[i] == pMainImageScenic)))
			{
				//xhj-暂时先注释
				//ASSERT(FALSE);
			}		
		}
	}

	// 更新图标索引
	CRibbonBarImage::GetInst()->UpdateIndex(info);

	return bRes;
}

BOOL CBCGPRibbonInfoLoaderEx::LoadImage (const CBCGPRibbonInfo::XID& id, CBCGPToolBarImages& image, BOOL bSingle)
{
    if (id.IsEmpty ())
    {
        return TRUE;
    }

    if (id.m_Value > 0)
    {
        image.Load (id.m_Value, GetInstance ());
    }

    if (!image.IsValid () && !id.m_Name.IsEmpty () && (NULL != _lpszBuffer))
    {
		// 加载图标信息
		CRibbonBarImage::GetInst()->LoadImage(id, image, _lpszBuffer);
    }

    if (image.IsValid ())
    {
        if (bSingle)
        {
            image.SetSingleImage (FALSE);
        }

        return TRUE;
    }

    return FALSE;
}

BOOL CBCGPRibbonInfoLoaderEx::LoadImage (CBCGPRibbonInfo::XImage& image, BOOL bSingle)
{
    image.m_Image.Clear();

    if (image.m_ID.IsEmpty())
    {
        return TRUE;
    }

    double dblScale = globalData.GetRibbonImageScale();

	LoadImage(image.m_ID, image.m_Image, bSingle);

	if (!image.m_bDontScale && image.m_Image.IsValid () && dblScale > 1.0)
	{
		if (image.m_Image.GetBitsPerPixel () < 32)
		{
			image.m_Image.ConvertTo32Bits (globalData.clrBtnFace);
		}

		image.m_Image.SetTransparentColor (globalData.clrBtnFace);
		image.m_Image.SmoothResize (dblScale);
	}

	return image.m_Image.IsValid ();
}
#endif // BCGP_EXCLUDE_RIBBON
