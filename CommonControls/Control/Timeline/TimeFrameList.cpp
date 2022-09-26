#include "stdafx.h"
#include "TimeFrameList.h"

IMPLEMENT_DYNAMIC(CTimeFrameListCtrl, CWnd)

BEGIN_MESSAGE_MAP(CTimeFrameListCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CTimeFrameListCtrl::CTimeFrameListCtrl(void)
{
	_nLength = 0;
	_nItemCount = 0;
	_nWidth = 0;
	_nHeadHeight = 0;
	_nCellHeight = LIST_ROW_HEIGHT;
	_nKeyFrameSpotSizePercent = 60;

	_hNofigyWnd = NULL;

	_nCurrentFrameCol = 0;
	_bPressDownCurrentFrame = false;

	_pMouseResponseJudgment = NULL;
	_nSelRow = -1;
	_bResponseMoveKeyFrame = false;

	_colorHead = RGB(200, 191, 231);
	_colorTimePoint = RGB(230, 230, 240);
	_colorCurrentFrame = RGB(255, 0, 0);
	_colorCurrentFrameBack = RGB(255, 150, 150);
	_vecColorKeyFrame.push_back(RGB(0, 162, 232));
	_vecColorTweenAnimation.push_back(RGB(210, 210, 210));

	_nHeadText = 0;

	_arrSelCellRowAndCol[0] = -1;
	_arrSelCellRowAndCol[0] = -1;
}

CTimeFrameListCtrl::~CTimeFrameListCtrl(void)
{

}

bool CTimeFrameListCtrl::Create(const CRect &rect, CWnd *pParentWnd, CTimeLineCtrl *pTimeLineCtrl, UINT nID /*= NULL*/,
	DWORD dwStyle /*= WS_BORDER | WS_CHILD | WS_VISIBLE*/)
{
	bool bSuccess = false;

	// 注册控件类型
	if (RegisterTimeFrameList())
	{
		bSuccess = !!CWnd::Create(L"TimeFrameListCtrl", NULL, dwStyle | WS_CLIPCHILDREN, rect, pParentWnd, nID);
	}

	_timeLineCtrl = pTimeLineCtrl;

	return bSuccess;
}

void CTimeFrameListCtrl::LoadConfig(const CString& strCfg)
{
	CXmlDocument doc;
	if (!doc.LoadFile(strCfg, DocFormatEnum::fmtXML))
	{
		return;
	}
	CXmlElement *pRoot = doc.GetElementRoot();
	CXmlElement *pTimeFrameNode = pRoot->GetChildElementAt(L"时间帧区", FALSE);

	CXmlElement *pTimeNode = pTimeFrameNode->GetChildElementAt(L"Time", FALSE);
	_nLength = StrToInt(pTimeNode->GetAttrValue(L"Length"));

	CXmlElement *pSizeNode = pTimeFrameNode->GetChildElementAt(L"Size", FALSE);
	_nWidth = StrToInt(pSizeNode->GetAttrValue(L"TimeFrameWidth"));
	_nHeadHeight = StrToInt(pSizeNode->GetAttrValue(L"HeadHeight"));

	CXmlElement *pKeyFrameSpotNode = pTimeFrameNode->GetChildElementAt(L"KeyFrameSpot", FALSE);
	_nKeyFrameSpotSizePercent= StrToInt(pKeyFrameSpotNode->GetAttrValue(L"SizePercent"));

	CXmlElement *pColorNode = pTimeFrameNode->GetChildElementAt(L"Color", FALSE);
	CXmlElement *pHeadColorNode = pColorNode->GetChildElementAt(L"Head", FALSE);
	_colorHead = GetStringColor(pHeadColorNode->GetAttrValue(L"HeadColor"));
	CXmlElement *pTimePointColorNode = pColorNode->GetChildElementAt(L"TimePoint", FALSE);
	_colorTimePoint = GetStringColor(pTimePointColorNode->GetAttrValue(L"TimePointColor"));
	CXmlElement *pCurrentFrameColorNode = pColorNode->GetChildElementAt(L"CurrentFrame", FALSE);
	_colorCurrentFrame = GetStringColor(pCurrentFrameColorNode->GetAttrValue(L"FrameColor"));
	_colorCurrentFrameBack = GetStringColor(pCurrentFrameColorNode->GetAttrValue(L"BackColor"));
	CXmlElement *pKeyFrameColorNode = pColorNode->GetChildElementAt(L"KeyFrame", FALSE);
	_vecColorKeyFrame = GetStringColorVec(pKeyFrameColorNode->GetAttrValue(L"KeyFrameColor"));
	CXmlElement *pTweenAnimationColorNode = pColorNode->GetChildElementAt(L"TweenAnimation", FALSE);
	_vecColorTweenAnimation = GetStringColorVec(pTweenAnimationColorNode->GetAttrValue(L"TweenAnimationColor"));
}


void CTimeFrameListCtrl::UpdateCtrl(void)
{
	InvalidateRect(NULL);
}

void CTimeFrameListCtrl::SetNofityWnd(HWND hNotifyWnd)
{
	_hNofigyWnd = hNotifyWnd;
}

void CTimeFrameListCtrl::SetMouseResponseJudgment(IMouseResponseJudgment *pMouseResponseJudgement)
{
	_pMouseResponseJudgment = pMouseResponseJudgement;
}

void CTimeFrameListCtrl::SetTimeFrameLength(int nTimeFrameLength)
{
	_nLength = nTimeFrameLength;

	if (_nCurrentFrameCol >= _nLength)
	{
		_nCurrentFrameCol = 0;
	}

	if (_arrSelCellRowAndCol[0] >= _nItemCount || _arrSelCellRowAndCol[1] >= _nLength)
	{
		_arrSelCellRowAndCol[0] = -1;
		_arrSelCellRowAndCol[1] = -1;
	}

	UpdateScrollBar();
}

int CTimeFrameListCtrl::GetTimeFrameLength(void)
{
	return _nLength;
}

void CTimeFrameListCtrl::SetItemCount(int nItemCount)
{
	_nItemCount = nItemCount;
}

int CTimeFrameListCtrl::GetCellHeight(void)
{
	return _nCellHeight;
}

int CTimeFrameListCtrl::GetHeadHeight(void)
{
	return _nHeadHeight;
}

int CTimeFrameListCtrl::GetItemCount(void)
{
	return _nItemCount;
}

int CTimeFrameListCtrl::GetWidth(void)
{
	return _nWidth;
}

void CTimeFrameListCtrl::SetKeyFrame(int nRow, TimeFrameRowInfoMap &mapTimeFrameInfo, CMemoryDC& dcMemory)
{
	for (auto timeFrameInfo : mapTimeFrameInfo)
	{
		if (timeFrameInfo.second->bKeyFrame)
		{
			SetKeyFrame(nRow, timeFrameInfo.first, dcMemory);
		}
	}
}

void CTimeFrameListCtrl::SetKeyFrame(int nRow, int nCol, CMemoryDC& dcMemory)
{
	CRect rcList;
	GetClientRect(rcList);
	rcList.top += _nHeadHeight;

	CPoint point = GetCellPoint(nRow, nCol);

	if (rcList.PtInRect(point))
	{
		int nColorSize = _vecColorKeyFrame.size();
		DrawSpot(point, _vecColorKeyFrame[(nRow % nColorSize)], dcMemory);
	}
}

void CTimeFrameListCtrl::SetTweenAnimation(int nRow, TimeFrameRowInfoMap &mapTimeFrameInfo, CMemoryDC& dcMemory)
{
	for (auto timeFrameInfo : mapTimeFrameInfo)
	{
		if (timeFrameInfo.second->bTweenAnimation)
		{
			SetTweenAnimation(nRow, timeFrameInfo.first, dcMemory);
		}
	}
}

void CTimeFrameListCtrl::SetTweenAnimation(int nRow, int nCol, CMemoryDC& dcMemory)
{
	if (nRow >= _nItemCount || nCol >= _nLength)
	{
		return;
	}

	CRect rcList;
	GetClientRect(rcList);
	rcList.top += _nHeadHeight;

	CRect rcTweenAnimation = GetCellRect(nRow, nCol);
	if (rcTweenAnimation.top < rcList.top)
	{
		if (rcTweenAnimation.bottom < rcList.top)
		{
			return;
		}

		rcTweenAnimation.top = rcList.top;
	}

	if (rcTweenAnimation.top > rcList.bottom)
	{
		return;
	}

	rcTweenAnimation.top += 1;
	rcTweenAnimation.left += 1;

	int nColorSize = _vecColorTweenAnimation.size();
	dcMemory.FillSolidRect(rcTweenAnimation, _vecColorTweenAnimation[(nRow % nColorSize)]);
}

CRect CTimeFrameListCtrl::GetCellRect(int nRow, int nCol)
{
	if (nRow >= _nItemCount || nCol >= _nLength || (_setCollapseRow.find(nRow) != _setCollapseRow.end()))
	{
		return NULL;
	}

	// 列递增宽度
	int nHorzPos = GetScrollPos(SB_HORZ);
	// 行递增高度
	int nIncreaseHeight = 0;
	int nVertPos = GetOwner()->GetScrollPos(SB_VERT);

	CRect rcList;
	GetClientRect(rcList);
	rcList.top += _nHeadHeight;

	CRect rect = rcList;
	rect.top += (GetCollapseDisplayRow(nRow) * _nCellHeight + nIncreaseHeight - nVertPos);
	rect.bottom = rect.top + _nCellHeight;
	rect.left += nCol * _nWidth - nHorzPos;
	rect.right = rect.left + _nWidth;

	return rect;
}

CPoint CTimeFrameListCtrl::GetCellPoint(int nRow, int nCol)
{
	if (nRow >= _nItemCount || nCol >= _nLength)
	{
		return NULL;
	}

	CRect rect = GetCellRect(nRow, nCol);
	return GetCellPoint(rect);
}

CPoint CTimeFrameListCtrl::GetCellPoint(CRect rect)
{
	CPoint point;
	point.x = rect.left + _nWidth / 2;
	point.y = rect.top + _nCellHeight / 2;
	return point;
}

void CTimeFrameListCtrl::DrawSpot(CPoint centerPoint, COLORREF &colorSpot, CMemoryDC &dcMemory)
{
	CBrush brush;
	brush.CreateSolidBrush(colorSpot);
	CBrush *oldBrush;
	oldBrush = dcMemory.SelectObject(&brush);

	int nHeight = _nCellHeight * _nKeyFrameSpotSizePercent / 2 / 100;
	int nWidth = _nWidth * _nKeyFrameSpotSizePercent / 2 / 100;

	CPoint ptTop = centerPoint;
	ptTop.y -= nHeight;
	CPoint ptBottom = centerPoint;
	ptBottom.y += nHeight;
	CPoint ptLeft = centerPoint;
	ptLeft.x -= nWidth;
	CPoint ptRight = centerPoint;
	ptRight.x += nWidth;

	CPoint points[4] = { ptTop,ptLeft ,ptBottom  ,ptRight };
	dcMemory.Polygon(points, 4);

	dcMemory.SelectObject(oldBrush);

	brush.DeleteObject();
	oldBrush->DeleteObject();
}

LRESULT CTimeFrameListCtrl::NotifyParent(UINT nMessage, int nRow /*= -1*/, int nCol /*= -1*/,
	ITimeLineCtrlDataSource *timeLineCtrlDataSource /*= NULL*/)
{
	CTimeFrameNotify notify;
	notify.hdrNotify.hwndFrom = _timeLineCtrl->m_hWnd;
	notify.hdrNotify.idFrom = _timeLineCtrl->GetDlgCtrlID();
	notify.hdrNotify.code = nMessage;
	notify.row = nRow;
	notify.col = nCol;
	notify.timeLineCtrlDataSource = timeLineCtrlDataSource;

	// 发送NOTIFY消息;
	return ::SendMessage(_hNofigyWnd, WM_NOTIFY, 0, (LPARAM)&notify);
}

bool CTimeFrameListCtrl::GetFocusPos(int &nRow, int &nCol, CPoint* ptMs /*= NULL*/)
{
	if (NULL == ptMs)
	{
		return false;
	}

	int nHorzPos = GetScrollPos(SB_HORZ);
	int nVertPos = GetOwner()->GetScrollPos(SB_VERT);

	//判断点击的位置（当点击在表头区域时，行数为-1）
	CRect rcHead;
	GetClientRect(rcHead);
	rcHead.bottom = rcHead.top + _nHeadHeight;
	if (rcHead.PtInRect(*ptMs))
	{
		nRow = -1;
	}
	else
	{
		nRow = (ptMs->y - rcHead.top - _nHeadHeight + nVertPos) / _nCellHeight;
	}
	nCol = (ptMs->x - rcHead.left + nHorzPos) / _nWidth;

	nRow = GetRowAfterCollapse(nRow);

	return false;
}

ITimeLineCtrlDataSource* CTimeFrameListCtrl::GetFocusDataSource(int nRow)
{
	DataSourceArray datasourceArray;
	_timeLineCtrl->GetDataSourceArray(datasourceArray);

	if (nRow >= datasourceArray.size())
	{
		return NULL;
	}

	return (ITimeLineCtrlDataSource*)datasourceArray[nRow];
}

int CTimeFrameListCtrl::GetRowAfterCollapse(int nRow)
{
	if (_setCollapseRow.size() > 0)
	{
		SetCollapseRowIter collapseRowIter = _setCollapseRow.begin();
		while (*collapseRowIter <= nRow && collapseRowIter != _setCollapseRow.end())
		{
			++nRow;
			++collapseRowIter;
		}
	}

	return nRow;
}

int CTimeFrameListCtrl::GetCollapseDisplayRow(int nRow)
{
	if (_setCollapseRow.size() > 0)
	{
		int nCollapseRow = 0;
		SetCollapseRowIter collapseRowIter = _setCollapseRow.begin();
		while (*collapseRowIter < nRow && collapseRowIter != _setCollapseRow.end())
		{
			++nCollapseRow;
			++collapseRowIter;
		}

		nRow -= nCollapseRow;
	}
	

	return nRow;
}

void CTimeFrameListCtrl::MoveCurrentFrame(CPoint ptMs)
{
	int nRow;
	int nCol;
	CRect rect;
	GetFocusPos(nRow, nCol, &ptMs);

	if (nCol >= _nLength)
	{
		return;
	}

	if (nRow == -1 && nCol != _nCurrentFrameCol)
	{
		_nCurrentFrameCol = nCol;
		ITimeLineCtrlDataSource *timeLineCtrlDataSource = ((nRow == -1) ? NULL : GetFocusDataSource(nRow));
		NotifyParent(TIMELINE_MOVECURFRAME, nRow, nCol, timeLineCtrlDataSource);

		UpdateCtrl();
	}
}

void CTimeFrameListCtrl::FillHeadBkColor(CMemoryDC &dcMemory, CRect rcHead)
{
	COLORREF colorHead = _colorHead - 25;
	CRect rcTop = rcHead;
	CRect rcBottom = rcHead;
	for (int i = 0; i < 5; ++i)
	{
		rcTop.top = rcHead.top + i;
		rcTop.bottom = rcTop.top + 1;
		dcMemory.FillSolidRect(rcTop, colorHead);
		
		rcBottom.bottom = rcHead.bottom - i;
		rcBottom.top = rcBottom.bottom - 1;
		dcMemory.FillSolidRect(rcBottom, colorHead);

		colorHead += 5;
	}

	CRect rcHeadBack = rcHead;
	rcHeadBack.top += 5;
	rcHeadBack.bottom -= 5;
	dcMemory.FillSolidRect(rcHeadBack, _colorHead);
}

void CTimeFrameListCtrl::DrawTimeFrameHeadLine(CMemoryDC &dcMemory, CRect rcHead, bool bFirst, bool bLast)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(127, 127, 127));
	dcMemory.SelectObject(&pen);

	//绘制时间帧列表列头的刻度
	CPoint lineStartPoint = rcHead.TopLeft();
	lineStartPoint.y += _nHeadHeight;
	CPoint lineEndPoint = rcHead.BottomRight();
	dcMemory.MoveTo(lineStartPoint);
	dcMemory.LineTo(lineEndPoint);

	CPoint topStartPoint = rcHead.TopLeft();
	CPoint topEndPoint = topStartPoint;
	topEndPoint.y += _nHeadHeight / 4;
	CPoint bottomStartPoint = lineStartPoint;
	bottomStartPoint.y -= _nHeadHeight / 4;
	CPoint bottomEndPoint = lineStartPoint;
	int nCount = (rcHead.right - rcHead.left) / _nWidth;
	for (int i = 0; i < nCount; ++i)
	{
		if (bFirst)
		{
			bFirst = false;
			topStartPoint.x += _nWidth;
			topEndPoint.x += _nWidth;
			bottomStartPoint.x += _nWidth;
			bottomEndPoint.x += _nWidth;
			continue;
		}

		dcMemory.MoveTo(topStartPoint);
		topStartPoint.x += _nWidth;
		dcMemory.LineTo(topEndPoint);
		topEndPoint.x += _nWidth;
		
		dcMemory.MoveTo(bottomStartPoint);
		bottomStartPoint.x += _nWidth;
		dcMemory.LineTo(bottomEndPoint);
		bottomEndPoint.x += _nWidth;
	}

	if (bLast)
	{
		dcMemory.MoveTo(topStartPoint);
		dcMemory.LineTo(bottomEndPoint);
	}
	else
	{
		dcMemory.MoveTo(topStartPoint);
		dcMemory.LineTo(topEndPoint);
		dcMemory.LineTo(bottomEndPoint);
		dcMemory.LineTo(bottomEndPoint);
	}

	pen.DeleteObject();
}

void CTimeFrameListCtrl::DrawTimeFrameHeadText(CMemoryDC &dcMemory, CRect rcHeadText, int nText)
{
	LOGFONT tempFont;
	HFONT hDefFont = HFONT(GetStockObject(OEM_FIXED_FONT));
	CFont* pFont = CFont::FromHandle(hDefFont);
	pFont->GetLogFont(&tempFont);
	dcMemory.SelectObject(&tempFont);

	dcMemory.SetBkMode(TRANSPARENT);
	dcMemory.SetTextColor(RGB(90, 90, 90));
	UINT nFormat = DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX | DT_LEFT | DT_NOPREFIX;

	int nCount = (rcHeadText.right - rcHeadText.left) / (5 * _nWidth);
	CString strTimeFrame;
	for (int i = 0; i < nCount; ++i)
	{
		strTimeFrame.Format(L"%d", nText + 5 * i);
		dcMemory.DrawText(strTimeFrame, &rcHeadText, nFormat);

		rcHeadText.left += 5 * _nWidth;
	}

	pFont->DeleteObject();
}

void CTimeFrameListCtrl::DrawTimeFrameHead(CMemoryDC &dcMemory)
{
	// 列递增宽度
	int nIncreaseWidth = 0;
	int nHorzPos = GetScrollPos(SB_HORZ);

	// 遍历绘制列
	CRect rcHead;
	GetClientRect(rcHead);
	rcHead.bottom = rcHead.top + _nHeadHeight;

	CRect rcHeadPaint = rcHead;
	bool bFirstPaint = true;
	bool bFirst = false;
	bool bLast = false;
	int nText = -1;
	for (size_t i = 0; i < _nLength; ++i)
	{
		// 计算列绘制区域
		CRect rcColumn = rcHead;
		rcColumn.left = nIncreaseWidth - nHorzPos + rcHead.left;
		rcColumn.right = rcColumn.left + _nWidth;

		nIncreaseWidth += _nWidth;

		// 不在显示区域范围内则不绘制(加快性能)
		if ((rcColumn.left < rcHead.left && rcColumn.right < rcHead.left)
			|| (rcColumn.left > rcHead.right && rcColumn.right > rcHead.right))
		{
			continue;
		}

		// 找到需绘制区域的左边
		if (bFirstPaint)
		{
			rcHeadPaint.left = rcColumn.left;
			nText = i + 1;
			bFirstPaint = false;
		}
		rcHeadPaint.right = rcColumn.right;

		// 临时存储最后列
		if (i == 0)
		{
			bFirst = true;
		}
		else if (i == (_nLength - 1))
		{
			bLast = true;
		}
	}
	FillHeadBkColor(dcMemory, rcHeadPaint);
	DrawTimeFrameHeadLine(dcMemory, rcHeadPaint, bFirst, bLast);

	_rcHeadText = rcHeadPaint;
	_rcHeadText.top+= _nHeadHeight / 4;
	_rcHeadText.bottom -= _nHeadHeight / 4;
	int nRemainder = nText % 5;
	if (nRemainder != 0)
	{
		int nAdd = 5 - nRemainder;
		_nHeadText = nText + nAdd;
		_rcHeadText.left += nAdd * _nWidth;
	}
}

void CTimeFrameListCtrl::DrawTimeFrameCell(CMemoryDC &dcMemory)
{
	if (_nItemCount == 0)
	{
		return;
	}

	if (_setCollapseRow.size() != 0)
	{
		_setCollapseRow.clear();
	}

	// 列递增宽度
	int nHorzPos = GetScrollPos(SB_HORZ);
	// 行递增高度
	int nIncreaseHeight = 0;
	int nVertPos = GetOwner()->GetScrollPos(SB_VERT);

	// 记录行
	size_t nRow = 0;
	// 遍历绘制行
	CRect rcList;
	GetClientRect(rcList);
	rcList.top += _nHeadHeight;

	CRect rcCell = rcList;
	CRect rcPaint;
	bool bFistPaint = true;
	bool nLastRightCell = true;
	int nCollapseRow = 0;
	ListDataSource listDataSource = _timeLineCtrl->GetListDataSource();
	for (ListDataSourceIter iter = listDataSource.begin(); iter != listDataSource.end(); ++iter, ++nRow)
	{
		HITEMDATA hItemData = iter->second;
		HITEMDATA hParentData = hItemData->hParentData;

		// 获取单元格数据
		IListDataSource* pDataSource = hItemData->GetDataSource();
		if (pDataSource == NULL)
			return;

		// 所在节点上层已收缩,则不展现
		if (NULL != hParentData && hParentData->IsCollapse())
		{
			_setCollapseRow.insert(nCollapseRow);
			++nCollapseRow;
			continue;
		}
		++nCollapseRow;

		// 计算行区域
		CRect rcHead;
		GetClientRect(rcHead);
		rcHead.bottom = rcHead.top + _nHeadHeight;
		
		CRect rcRow;
		rcRow.left = rcList.left - nHorzPos;
		rcRow.right = rcList.left + _nLength * _nWidth;
		rcRow.top = nIncreaseHeight - nVertPos + rcHead.bottom;
		rcRow.bottom = rcRow.top + _nCellHeight;

		nIncreaseHeight += _nCellHeight;

		// 不在显示区域范围内则不绘制(加快性能)
		if ((rcRow.top < rcList.top && rcRow.bottom < rcList.top)
			|| (rcRow.top > rcList.bottom && rcRow.bottom > rcList.bottom))
		{
			continue;
		}

		// 初始化单元格数据
		for (size_t i = 0; i < _nLength; ++i)
		{
			// 计算单元格区域
			CRect rcCell = rcRow;
			rcCell.left = rcRow.left + i * _nWidth;
			rcCell.right = rcCell.left + _nWidth;

			// 列头不在显示区域范围内则不绘制(加快性能)
			if ((rcCell.left < rcList.left && rcCell.right < rcList.left)
				|| (rcCell.left > rcList.right && rcCell.right > rcList.right))
			{
				continue;
			}

			if (bFistPaint)
			{
				bFistPaint = false;
				rcPaint.top = rcCell.top;
				rcPaint.left = rcCell.left;
			}
			rcPaint.bottom = rcCell.bottom;
			if (nLastRightCell)
			{
				rcPaint.right = rcCell.right;
			}

			if ((i + 1) % 5 == 0)
			{
				dcMemory.FillSolidRect(rcCell, _colorTimePoint);
			}
		}
		nLastRightCell = false;
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(195, 195, 195));
	dcMemory.SelectObject(&pen);

	CPoint rowStartPoint = rcPaint.TopLeft();
	CPoint rowEndPoint = rowStartPoint;
	rowEndPoint.x += rcPaint.Width();
	int nRowCount = rcPaint.Height() / _nCellHeight;
	for (int i = 0; i < nRowCount + 1; ++i)
	{
		dcMemory.MoveTo(rowStartPoint);
		rowStartPoint.y += _nCellHeight;
		dcMemory.LineTo(rowEndPoint);
		rowEndPoint.y += _nCellHeight;
	}

	CPoint colStartPoint = rcPaint.TopLeft();
	CPoint colEndPoint = colStartPoint;
	colEndPoint.y += rcPaint.Height();
	int nColCount = rcPaint.Width() / _nWidth;
	for (int i = 0; i < nColCount + 1; ++i)
	{
		dcMemory.MoveTo(colStartPoint);
		colStartPoint.x += _nWidth;
		dcMemory.LineTo(colEndPoint);
		colEndPoint.x += _nWidth;
	}

	pen.DeleteObject();
}

void CTimeFrameListCtrl::DrawCurrentFrame(CMemoryDC& dcMemory)
{
	if (_nCurrentFrameCol >= _nLength)
	{
		return;
	}

	int nHorzPos = GetScrollPos(SB_HORZ);
	CRect rcHead;
	GetClientRect(rcHead);
	rcHead.bottom = rcHead.top + _nHeadHeight;
	CRect rect = rcHead;
	rect.left += (_nCurrentFrameCol * _nWidth - nHorzPos);
	rect.right = rect.left + _nWidth;
	dcMemory.FillSolidRect(rect, _colorCurrentFrameBack);

	CPen pen;
	pen.CreatePen(PS_SOLID, 2, _colorCurrentFrame);
	dcMemory.SelectObject(&pen);

	CPoint ptFrame = rect.TopLeft();
	dcMemory.MoveTo(ptFrame);
	ptFrame.x += _nWidth;
	dcMemory.LineTo(ptFrame);
	ptFrame.y += _nHeadHeight;
	dcMemory.LineTo(ptFrame);
	ptFrame.x -= _nWidth;
	dcMemory.LineTo(ptFrame);
	ptFrame.y -= _nHeadHeight;
	dcMemory.LineTo(ptFrame);

	CPoint startPoint = rect.BottomRight();
	startPoint.x -= _nWidth / 2;
	CPoint endPoint = startPoint;
	endPoint.y += _nItemCount * _nCellHeight;
	dcMemory.MoveTo(startPoint);
	dcMemory.LineTo(endPoint);

	pen.DeleteObject();
}

void CTimeFrameListCtrl::DrawSelectCell(CMemoryDC &dcMemory)
{
	if (_arrSelCellRowAndCol[0] >= _nItemCount
		|| _arrSelCellRowAndCol[1] >= _nLength
		|| (_setCollapseRow.find(_arrSelCellRowAndCol[0]) != _setCollapseRow.end()))
	{
		return;
	}

	CRect rcSelCell = GetCellRect(_arrSelCellRowAndCol[0], _arrSelCellRowAndCol[1]);

	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));
	dcMemory.SelectObject(&pen);

	CPoint point = rcSelCell.TopLeft();
	dcMemory.MoveTo(point);
	point.x += rcSelCell.Width();
	dcMemory.LineTo(point);
	point.y += rcSelCell.Height();
	dcMemory.LineTo(point);
	point.x -= rcSelCell.Width();
	dcMemory.LineTo(point);
	point.y -= (rcSelCell.Height() + 1);
	dcMemory.LineTo(point);

	pen.DeleteObject();
}

void CTimeFrameListCtrl::UpdateScrollBar(void)
{
	SCROLLINFO scrInfo;
	scrInfo.cbSize = sizeof(scrInfo);

	int nSumColumnWidth = _nLength * _nWidth + sizeof(scrInfo);
	CRect rect;
	GetClientRect(&rect);

	scrInfo.fMask = SIF_PAGE | SIF_RANGE;
	scrInfo.nPage = rect.Width();
	scrInfo.nMin = 0;
	scrInfo.nMax = nSumColumnWidth - 1;

	SetScrollInfo(SB_HORZ, &scrInfo);
}

void CTimeFrameListCtrl::DrawTimeFrame(CMemoryDC &dcMemory)
{
	ListDataSource listDataSource = _timeLineCtrl->GetListDataSource();

	int num = 0;
	for (ListDataSourceIter mit = listDataSource.begin(); mit != listDataSource.end(); ++mit)
	{
		ITimeLineCtrlDataSource *dataSource = (ITimeLineCtrlDataSource*)mit->second->GetDataSource();

		if (NULL == dataSource)
		{
			return;
		}
		
		// 如果没有收缩就绘制
		if (_setCollapseRow.find(num) == _setCollapseRow.end())
		{
			TimeFrameRowInfoMap mapTimeFrameInfo = dataSource->GetTimeFrameRowInfoMap();
			SetTweenAnimation(num, mapTimeFrameInfo, dcMemory);
			SetKeyFrame(num, mapTimeFrameInfo, dcMemory);
		}
		
		++num;
	}
}

COLORREF CTimeFrameListCtrl::GetStringColor(const CString& strColor)
{
	COLORREF color = RGB(0, 0, 0);

	int nFirst = strColor.Find(L",");
	if (-1 == nFirst)
		return color;

	int nSecond = strColor.Find(L",", nFirst + 1);
	if (-1 == nSecond)
		return color;

	return RGB(_ttoi(strColor.Mid(0, nFirst)),
		_ttoi(strColor.Mid(nFirst + 1, nSecond - nFirst - 1)),
		_ttoi(strColor.Mid(nSecond + 1, strColor.GetLength() - nSecond - 1)));
}

VecColor CTimeFrameListCtrl::GetStringColorVec(const CString& strColor)
{
	VecColor vecColor;
	std::vector<CString> vStrColor;
	CStringToolkitEx::SplitString(strColor, vStrColor, "/");
	for (CString color : vStrColor)
	{
		vecColor.push_back(GetStringColor(color));
	}

	return vecColor;
}

bool CTimeFrameListCtrl::RegisterTimeFrameList(void)
{
	HINSTANCE hInstance = AfxGetInstanceHandle();

	WNDCLASS wndCls = { 0 };
	if (GetClassInfo(hInstance, L"TimeFrameListCtrl", &wndCls))
		return true;

	wndCls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndCls.lpfnWndProc = ::DefWindowProc;
	wndCls.cbClsExtra = 0;
	wndCls.cbWndExtra = 0;
	wndCls.hInstance = AfxGetInstanceHandle();
	wndCls.hIcon = NULL;
	wndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndCls.lpszMenuName = NULL;
	wndCls.lpszClassName = L"TimeFrameListCtrl";

	return !!AfxRegisterClass(&wndCls);
}

void CTimeFrameListCtrl::OnPaint(void)
{
	CPaintDC dc(this);

	// 使用双缓存绘制
	CRect rcClient;
	GetClientRect(rcClient);

	CMemoryDC dcMemory(&dc, rcClient);
	dcMemory.FillSolidRect(rcClient, RGB(255, 255, 255));

	DrawTimeFrameHead(dcMemory);

	CRect rcHead;
	GetClientRect(rcHead);
	rcHead.bottom = rcHead.top + _nHeadHeight + 1;
	dcMemory.ExcludeClipRect(rcHead);

	DrawTimeFrameCell(dcMemory);
	DrawTimeFrame(dcMemory);

	HRGN hrgn = NULL;
	hrgn = CreateRectRgn(rcHead.left, rcHead.top, rcHead.right, rcHead.bottom);
	ExtSelectClipRgn(dcMemory.m_hDC, hrgn, RGN_OR);

	DrawCurrentFrame(dcMemory);
	DrawTimeFrameHeadText(dcMemory, _rcHeadText, _nHeadText);

	dcMemory.ExcludeClipRect(rcHead);
	DrawSelectCell(dcMemory);

	DeleteObject(hrgn);
}

BOOL CTimeFrameListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTimeFrameListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (_bPressDownCurrentFrame)
	{
		MoveCurrentFrame(point);
	}

	CRect rcHead;
	GetClientRect(rcHead);
	rcHead.bottom = rcHead.top + _nHeadHeight;
	if (!rcHead.PtInRect(point))
	{
		_bPressDownCurrentFrame = false;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CTimeFrameListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	int  nRow = -1, nCol = -1;
	GetFocusPos(nRow, nCol, &point);
	ITimeLineCtrlDataSource *timeLineCtrlDataSource;
	if (nRow == -1)
	{
		timeLineCtrlDataSource = NULL;
		_bPressDownCurrentFrame = true;
		if (nCol < _nLength && _nCurrentFrameCol != nCol)
		{
			_nCurrentFrameCol = nCol;
			UpdateCtrl();
		}
	}
	else
	{
		if (nRow >= _nItemCount || nCol >= _nLength)
		{
			return;
		}

		timeLineCtrlDataSource = GetFocusDataSource(nRow);
		_arrSelCellRowAndCol[0] = nRow;
		_arrSelCellRowAndCol[1] = nCol;
		UpdateCtrl();

		if (_pMouseResponseJudgment != NULL)
		{
			_nSelRow = nRow;
			_bResponseMoveKeyFrame = _pMouseResponseJudgment->BeginDrag(timeLineCtrlDataSource, nCol);
		}
	}

	NotifyParent(TIMELINE_LBUTTONDOWN, nRow, nCol, timeLineCtrlDataSource);

	CWnd::OnLButtonDown(nFlags, point);
}

void CTimeFrameListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	_bPressDownCurrentFrame = false;

	CRect rect;
	int  nRow = -1, nCol = -1;
	GetFocusPos(nRow, nCol, &point);
	ITimeLineCtrlDataSource *timeLineCtrlDataSource;
	if (nRow == -1)
	{
		timeLineCtrlDataSource = NULL;
	}
	else
	{
		timeLineCtrlDataSource = GetFocusDataSource(nRow);

		if (_bResponseMoveKeyFrame && _nSelRow == nRow)
		{
			_pMouseResponseJudgment->EndDrag(timeLineCtrlDataSource, nCol);
			_bResponseMoveKeyFrame = false;
		}
	}
	NotifyParent(TIMELINE_LBUTTONUP, nRow, nCol, timeLineCtrlDataSource);

	CWnd::OnLButtonUp(nFlags, point);
}

void CTimeFrameListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rect;
	int  nRow = -1, nCol = -1;
	GetFocusPos(nRow, nCol, &point);
	ITimeLineCtrlDataSource *timeLineCtrlDataSource;
	if (nRow == -1)
	{
		timeLineCtrlDataSource = NULL;
	}
	else
	{
		timeLineCtrlDataSource = GetFocusDataSource(nRow);
	}
	NotifyParent(TIMELINE_DBLCLICK, nRow, nCol, timeLineCtrlDataSource);
}

void CTimeFrameListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	int  nRow = -1, nCol = -1;
	GetFocusPos(nRow, nCol, &point);
	ITimeLineCtrlDataSource *timeLineCtrlDataSource;
	if (nRow == -1)
	{
		timeLineCtrlDataSource = NULL;
	}
	else
	{
		timeLineCtrlDataSource = GetFocusDataSource(nRow);
	}
	NotifyParent(TIMELINE_RIGHTCLICK, nRow, nCol, timeLineCtrlDataSource);
}

void CTimeFrameListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	SCROLLINFO infoScroll;
	infoScroll.cbSize = sizeof(SCROLLINFO);
	infoScroll.fMask = SIF_ALL;

	if (!GetScrollInfo(SB_HORZ, &infoScroll))
		return;

	int nNewPos = infoScroll.nPos;
	switch (nSBCode)
	{
	case SB_LEFT:
		nNewPos = infoScroll.nMin;
		break;
	case SB_LINELEFT:
		nNewPos = max(infoScroll.nPos - 5, infoScroll.nMin);
		break;
	case SB_PAGELEFT:
		nNewPos = max(infoScroll.nPos - int(infoScroll.nPage), infoScroll.nMin);
		break;
	case SB_RIGHT:
		nNewPos = infoScroll.nMax;
		break;
	case SB_LINERIGHT:
		nNewPos = min(infoScroll.nPos + 5, infoScroll.nMax);
		break;
	case SB_PAGERIGHT:
		nNewPos = min(int(infoScroll.nPos + infoScroll.nPage), infoScroll.nMax);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewPos = infoScroll.nTrackPos;
		break;
	}

	infoScroll.fMask = SIF_POS;
	infoScroll.nPos = nNewPos;
	SetScrollInfo(SB_HORZ, &infoScroll);

	InvalidateRect(NULL);
}

void CTimeFrameListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (NULL != m_hWnd)
	{
		UpdateScrollBar();
	}
}