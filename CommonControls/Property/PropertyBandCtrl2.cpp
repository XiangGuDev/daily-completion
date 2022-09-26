#include "stdafx.h"
#include "PropertyBandCtrl2.h"

using namespace PropEx;

IMPLEMENT_DYNAMIC(CPropertyBandCtrl2, CPropertyBandCtrl)

BEGIN_MESSAGE_MAP(CPropertyBandCtrl2, CPropertyBandCtrl)
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPL_SINGLE_CLASS(CPropertyBandCtrl2)

CPropertyBandCtrl2::CPropertyBandCtrl2():CPropertyBandCtrl()
{
}

CPropertyBandCtrl2::~CPropertyBandCtrl2()
{
	Release();
	SAFE_DELETE(_pStyle);
	_pStyle = nullptr;
	SAFE_DELETE(_propList);
	_propList = nullptr;
}

void CPropertyBandCtrl2::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void CPropertyBandCtrl2::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	__super::PostNcDestroy();
	CPropertyBandCtrl2::ResetInstance();
}