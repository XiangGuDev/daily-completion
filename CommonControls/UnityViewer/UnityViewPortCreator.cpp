#include "stdafx.h"
#include "UnityViewPortCreator.h"
#include "UnityView.h"

d3s::views::view::IViewPort* CUnityViewPortCreator::CreateViewport(d3s::designfile::model::DesignModel *pModel, d3s::share_ptr<d3s::views::view::CViewInfo> pInfo)
{
	return new CUnityView(pModel, pInfo);
}

