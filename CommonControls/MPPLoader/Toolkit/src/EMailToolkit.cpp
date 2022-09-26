#include "stdafx.h"
#include "..\include\EMailToolkit.h"
#include <Mapi.h>

using namespace BwProj::toolkit;

// 调用本地邮件程序发送附件
bool CEMailToolkit::SendFileToEmail(LPCTSTR filePath)
{
	// 获取 MAPISendMail 函数
	HMODULE hMod = LoadLibrary(L"MAPI32.DLL");
	if (hMod == NULL) return false;

	ULONG(PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessageW*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(hMod, "MAPISendMailW");
	if (lpfnSendMail == NULL) {
		FreeLibrary(hMod);
		return false;
	}

	CAtlArray<CString> m_list;

	m_list.Add(filePath);
	int nFileCount = m_list.GetCount ();
	//分配内存保存附件信息 不能使用静态数组，因为不知道要发送附件的个数
	MapiFileDescW* pFileDesc = (MapiFileDescW*)malloc(sizeof(MapiFileDescW) * nFileCount);
	memset(pFileDesc, 0, sizeof(MapiFileDescW) * nFileCount);

	//分配内存保存附件文件路径
	TCHAR pTchPath[10][MAX_PATH] = { 0 };// new TCHAR[nFileCount][MAX_PATH];// (TCHAR**)malloc(MAX_PATH * nFileCount);
	CString szText;
	for (int i = 0; i < min(nFileCount, 10); i++)
	{
		szText = m_list.GetAt(i);
		_tcscpy_s(pTchPath[i], MAX_PATH, szText);
		(pFileDesc + i)->nPosition = (ULONG)-1;
		(pFileDesc + i)->lpszPathName = pTchPath[i];
		(pFileDesc + i)->lpszFileName = pTchPath[i];
	}

	//收件人结构信息
	MapiRecipDescW recip = { 0 };
	//memset(&recip, 0, sizeof(MapiRecipDescW));
	//recip.lpszAddress = L"";// m_szEmailMAPI.GetBuffer(0);
	recip.ulRecipClass = MAPI_TO;

	//邮件结构信息
	MapiMessageW message;
	memset(&message, 0, sizeof(message));
	message.nFileCount = min(nFileCount, 10);
	//文件个数
	message.lpFiles = pFileDesc;
	////文件信息
	//message.nRecipCount = 1;
	////收件人个数
	//message.lpRecips = &recip;
	////收件人
	//message.lpszSubject = L"";// m_szSubject.GetBuffer(0);
	////主题
	//message.lpszNoteText = L"";// m_szText.GetBuffer(0);
	//正文内容

	//发送邮件
	int nError = lpfnSendMail(0, 0, &message, MAPI_LOGON_UI | MAPI_DIALOG, 0);
	bool bOk = !(nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE);

	//不要忘了释放分配的内存
	free(pFileDesc);
	//delete pTchPath[];
	FreeLibrary(hMod);

	return bOk;
}