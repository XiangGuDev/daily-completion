#include "StdAfx.h"
#include "IProgress.h"
#include "ProgressDlg.h"


IProgress::IProgress()
{
	_useStep = 0;
	_pListener = NULL;
}

IProgress::~IProgress()
{

}

void IProgress::OnStart()
{
	_useStep = 0;
	if (_pListener != NULL)
	{
		_pListener->OnStep(_useStep);
	}
}

void IProgress::OnStop()
{
	_useStep = GetStep();
}

void IProgress::OnComplete()
{
	_useStep = 100;
	if (_pListener != NULL)
	{
		_pListener->OnStep(_useStep);
	}
}

void IProgress::OnStep(int nStep)
{
	_useStep = nStep;
	if (_pListener != NULL)
	{
		_pListener->OnStep(_useStep);
	}
}

void IProgress::IncrementStep(int nStep, int nMaxLimit /*= 100*/)
{
	_useStep += nStep;
	int nMax = nMaxLimit > 100 ? 100 : nMaxLimit;

	_useStep = min(_useStep, nMax);

	if (_pListener != NULL)
	{
		_pListener->OnStep(_useStep);
	}
}

int IProgress::GetStep()
{
	return _useStep;
}

bool IProgress::IsComplete()
{
	if (_useStep == 100)
		return true;

	return false;
}

void IProgress::SetNotify(IProgressListener* pListener)
{
	_pListener = pListener;
}




CProgressAgent::CProgressAgent(const CString& sTitle /*= L""*/, 
	const CString& sText /*= L"正在加载，请稍等..."*/, CWnd *pParent /*= NULL*/)
{
	_useStep = 0;
	_pListener = NULL;

	INIT_RESHANDLE();
	_pdlg = new CProgressDlg(pParent);
	_pdlg->Create(IDD_PROGRESS_DLG);
	_pdlg->ShowWindow(SW_SHOW);
	_pdlg->OnStep(0);
	_pdlg->SetTitle(sTitle);
	_pdlg->SetText(sText);
	_pdlg->SendMessage(WM_PAINT, 0XFFCFFFCF, 0XFFCFFFCF);
	SetNotify(_pdlg);
}

CProgressAgent::~CProgressAgent()
{
	if (_pdlg != NULL)
	{
		_pdlg->DestroyWindow();
		delete _pdlg;
		_pdlg = NULL;
	}
}

int CProgressAgent::ModalProgress(ModalProgressInfo& proInfo, bool bCancel, CProgressAgentStopEvent* pEvent)
{
	INIT_RESHANDLE();

	if (proInfo.hParent == 0)
	{
		if (AfxGetMainWnd() != nullptr)
			proInfo.hParent = AfxGetMainWnd()->m_hWnd;
		else
			proInfo.hParent = ::GetActiveWindow();
	}

	CProgressDlg cDlg(&proInfo, bCancel, pEvent);
	return cDlg.DoModal();	
}

LRESULT CProgressAgent::ModalMessageBox(ModalProgressInfo* proInfo, CString strContent, CString strTitle, UINT uType)
{	
	MessageBoxInfo msgBox;
	msgBox.strContent = strContent;
	msgBox.strTitle = strTitle;
	msgBox.uType = uType;
	::SendMessage(proInfo->hProgressHwnd, USER_PROGRESSMSGBOX, NULL, (LPARAM)&msgBox);
	return msgBox.lResult;
}
