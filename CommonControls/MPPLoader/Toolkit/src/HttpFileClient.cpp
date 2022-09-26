#include "StdAfx.h"
#include "..\include\HttpFileClient.h"
#include "..\include\LibToolkit.h"
#include "..\include\MemoryDataStream.h"
#include <wininet.h>

using namespace BwProj::toolkit;

CHttpFileClient::CHttpFileClient(void)
{
}
CHttpFileClient::~CHttpFileClient(void)
{
}

BOOL CHttpFileClient::CanWebsiteVisit(CString sURI)
{
	if (sURI == "") return FALSE;

	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	DWORD dwStateCode = 0;
	INTERNET_PORT wPort = 0;
	CString sServer = _T("");
	CString sObject = _T("");
	const int nTimeOut = 5000;
	try
	{
		if (!AfxParseURL(sURI, dwType, sServer, sObject, wPort)) return FALSE;

		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);
		pHttpConn = cis.GetHttpConnection(sServer, wPort);

		if (pHttpConn)
		{
			pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, sObject);
			if (pHttpFile->SendRequest())
			{
				pHttpFile->QueryInfoStatusCode(dwStateCode);
				if (dwStateCode == HTTP_STATUS_CREATED || dwStateCode == HTTP_STATUS_OK)
				{
					bResult = TRUE;
				}
			}
		}
	}
	catch (...)
	{
	}

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}
	cis.Close();

	return bResult;
}

BOOL CHttpFileClient::UploadFile(LPCTSTR szRemoteURI, BYTE* btsIn, int btsLength)
{
	if ((btsIn == NULL) || (btsLength <= 0)) return FALSE;

	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	DWORD dwPostSize = 0;
	INTERNET_PORT wPort = 0;
	CString sServer = _T("");
	CString sObject = _T("");
	const int nTimeOut = 5000;
	try
	{
		if(!AfxParseURL(szRemoteURI, dwType, sServer, sObject, wPort)) return FALSE;

		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);
		pHttpConn = cis.GetHttpConnection(sServer, wPort, NULL, NULL);

		if (pHttpConn)
		{
			pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, sObject);

			if(UseHttpSendReqEx(pHttpFile, btsIn, btsLength))
			{
				DWORD dwStateCode = 0;
				pHttpFile->QueryInfoStatusCode(dwStateCode);
				if(dwStateCode == HTTP_STATUS_CREATED || dwStateCode == HTTP_STATUS_OK)
				{
					bResult = TRUE;
				}
			}
		}
	}
	catch (...)
	{
	}

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}
	cis.Close();

	return bResult;
}

BOOL CHttpFileClient::UploadFileRequest(LPCTSTR szRemoteURI, BYTE* btsIn, int btsLength, 
										BYTE** btsOut, int* btsOutLength)
{
	if ((btsIn == NULL) || (btsLength <= 0)) return FALSE;
	if (*btsOut != NULL) return FALSE;

	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	DWORD dwPostSize = 0;
	INTERNET_PORT wPort = 0;
	CString sServer = _T("");
	CString sObject = _T("");
	const int nTimeOut = 5000;

	DWORD btsSize = 0;
	try
	{
		if(!AfxParseURL(szRemoteURI, dwType, sServer, sObject, wPort)) return FALSE;

		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);
		pHttpConn = cis.GetHttpConnection(sServer, wPort, NULL, NULL);

		if (pHttpConn)
		{
			pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, sObject);

			if(UseHttpSendReqEx(pHttpFile, btsIn, btsLength))
			{
				DWORD dwStateCode = 0;
				pHttpFile->QueryInfoStatusCode(dwStateCode);

				if(dwStateCode == HTTP_STATUS_CREATED || dwStateCode == HTTP_STATUS_OK)
				{
					CMemoryDataWriteStream buffer;

					BYTE pszBuffer[__BUFFER_SIZE] = { 0 };
					DWORD dwWrite = 0;
					UINT nRead = 0;
					do
					{
						memset(&pszBuffer, 0, __BUFFER_SIZE);
						nRead = pHttpFile->Read(pszBuffer, __BUFFER_SIZE);
						if (nRead <= 0) break;

						buffer.Write(pszBuffer, nRead);
						dwWrite += nRead;

					} while (TRUE);

					//if (dwWrite != btsSize) goto _err_handler;

					*btsOutLength = buffer.GetLength();
					*btsOut = new BYTE[buffer.GetLength()];
					buffer.CopyBuffer(*btsOut);
					bResult = TRUE;
				}
			}
		}
	}
	catch (...)
	{
	}

//_err_handler:
	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}
	cis.Close();

	return bResult;
}

BOOL CHttpFileClient::DownLoadFile(LPCTSTR szRemoteURI, BYTE** btsOut, int* btsOutLength)
{
	if (btsOut == NULL) return FALSE;

	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	DWORD dwPostSize = 0;
	INTERNET_PORT wPort = 0;
	CString sServer = _T("");
	CString sObject = _T("");
	const int nTimeOut = 2000;

	DWORD btsSize = 0;
	try
	{
		if(!AfxParseURL(szRemoteURI, dwType, sServer, sObject, wPort)) return FALSE;

		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);
		pHttpConn = cis.GetHttpConnection(sServer, wPort, NULL, NULL);

		if (pHttpConn)
		{
			pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, sObject);
			if (!pHttpFile->SendRequest())
			{
				goto _err_handler;
			}
				

			DWORD dwStateCode;
			pHttpFile->QueryInfoStatusCode(dwStateCode);
			if (dwStateCode != HTTP_STATUS_OK) 
				goto _err_handler;

			BOOL bRet = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, btsSize);
			if (!bRet || (btsSize <= 0)) 
				goto _err_handler;

			*btsOut = new BYTE[btsSize];
			memset(*btsOut, 0, btsSize);

			BYTE pszBuffer[__BUFFER_SIZE] = { 0 };
			DWORD dwWrite = 0;
			UINT nRead = 0;
			do
			{
				memset(&pszBuffer, 0, __BUFFER_SIZE);
				nRead = pHttpFile->Read(pszBuffer, __BUFFER_SIZE);
				if (nRead <= 0) break;

				memcpy(*btsOut + dwWrite, pszBuffer, nRead);
				dwWrite += nRead;

			} while (TRUE);

			if (dwWrite != btsSize) goto _err_handler;

			*btsOutLength = (int)btsSize;
			bResult = TRUE;
		}
	}
	catch (...)
	{
	}

_err_handler:
	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}
	cis.Close();

	return bResult;
}

BOOL CHttpFileClient::DownLoadFile2(LPCTSTR szRemoteURI, BYTE** btsOut, int* btsOutLength)
{
	BOOL success = FALSE;
	HINTERNET hSession = InternetOpen(_T("BwProj"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, szRemoteURI, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

		if (hHttp != NULL)
		{
			// »ñÈ¡×´Ì¬
			DWORD dwStatusCode = 0;
			{
				TCHAR szBuffer[80] = { 0 };
				DWORD dwLen = 80;
				if (!HttpQueryInfo(hHttp, HTTP_QUERY_STATUS_CODE, szBuffer, &dwLen, NULL))
					return FALSE;
				dwStatusCode = (DWORD)_ttol(szBuffer);
			}
			if (dwStatusCode != HTTP_STATUS_OK)
				return FALSE;

			CMemoryDataWriteStream buffer;

			char Temp[INTERNET_MAX_PATH_LENGTH];
			ULONG Number = 1;
			while (Number > 0)
			{
				InternetReadFile(hHttp, Temp, INTERNET_MAX_PATH_LENGTH - 1, &Number);
				if (Number > 0)
					buffer.Write(Temp, Number);
			}

			*btsOutLength = buffer.GetLength();
			*btsOut = new BYTE[buffer.GetLength()];
			buffer.CopyBuffer(*btsOut);

			InternetCloseHandle(hHttp);
			hHttp = NULL;

			success = TRUE;
		}

		InternetCloseHandle(hSession);
		hSession = NULL;
	}

	return success;
}

BOOL CHttpFileClient::DeleteFile(LPCTSTR szRemoteURI)
{
	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	DWORD dwPostSize = 0;
	INTERNET_PORT wPort = 0;
	CString sServer = _T("");
	CString sObject = _T("");
	const int nTimeOut = 2000;
	try
	{
		if(!AfxParseURL(szRemoteURI, dwType, sServer, sObject, wPort)) return FALSE;

		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);
		pHttpConn = cis.GetHttpConnection(sServer, wPort, NULL, NULL);

		if (pHttpConn)
		{
			pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_DELETE, sObject);
			if (!pHttpFile->SendRequest()) goto _err_handler;

			DWORD dwStateCode;
			pHttpFile->QueryInfoStatusCode(dwStateCode);
			if (dwStateCode != HTTP_STATUS_OK) goto _err_handler;

			bResult = TRUE;
		}
	}
	catch (...)
	{
	}

_err_handler:
	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}
	cis.Close();

	return bResult;
}

BOOL CHttpFileClient::UseHttpSendReqEx(CHttpFile* httpFile, BYTE* btsIn, int btsLength)
{
	if ((httpFile == NULL) || (btsIn == NULL) || (btsLength <= 0)) return FALSE;

	INTERNET_BUFFERS BufferIn;
	memset(&BufferIn, 0, sizeof(BufferIn));

	DWORD dwTotalWritten = 0;
	BYTE pFileBuffer[__BUFFER_SIZE] = { 0 };

	BufferIn.dwStructSize    = sizeof( INTERNET_BUFFERS ); // Must be set or error will occur
	BufferIn.Next            = NULL;
	BufferIn.lpcszHeader     = NULL;
	BufferIn.dwHeadersLength = 0;
	BufferIn.dwHeadersTotal  = 0;
	BufferIn.lpvBuffer       = NULL;               
	BufferIn.dwBufferLength  = 0;
	BufferIn.dwBufferTotal   = btsLength; // This is the only member used other than dwStructSize
	BufferIn.dwOffsetLow     = 0;
	BufferIn.dwOffsetHigh    = 0;
	if (!httpFile->SendRequestEx(&BufferIn, NULL, HSR_INITIATE, 1))
	{
		return FALSE;
	}

	for (int i = 0; i < btsLength; i = i + __BUFFER_SIZE)
	{
		int nActual = (btsLength - i);
		if (nActual > __BUFFER_SIZE) nActual = __BUFFER_SIZE;

		memset(&pFileBuffer, 0, __BUFFER_SIZE);
		memcpy(pFileBuffer, btsIn + i, nActual);

		httpFile->Write(pFileBuffer, nActual);
		dwTotalWritten += nActual;
	}

	if (dwTotalWritten != btsLength)
	{
		return FALSE;
	}
	if(!httpFile->EndRequest(0, NULL, 1))
	{
		return FALSE;
	}

	return TRUE;
}