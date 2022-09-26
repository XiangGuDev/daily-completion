#pragma once

#ifndef _WININET_
#include <wininet.h>
#endif

#define _AFXINET_INLINE inline
#define AFXAPI		__stdcall
#ifndef AFX_STATIC_DATA
#define AFX_STATIC_DATA
#endif
#ifndef AFX_STATIC
#define AFX_STATIC
#endif
#ifndef ASSERT
#define ASSERT(x)	ATLASSERT(x)
#endif

#define ASSERT_VALID(x)
#define ASSERT_POINTER(x, y)
#define ASSERT_NULL_OR_POINTER(x, y)
#define ASSERT_KINDOF(x, y)
#define UNUSED_ALWAYS(x)
#define TRACE(x, y, z)
#define THROW(x)
#define ENSURE(x)

//#ifdef _AFXDLL
#pragma comment(lib, "wininet.lib")
//#endif

class CStdioFile;

class CInternetSession; // from CObject


class CInternetFile;    // from CStdioFile (FILETXT.CPP)
	class CHttpFile;


class CInternetConnection;
	class CHttpConnection;

class CInternetException;

/////////////////////////////////////////////////////////////////////////////
// Global Functions

BOOL AFXAPI AfxParseURL(LPCTSTR pstrURL, DWORD& dwServiceType,
	CString& strServer, CString& strObject, INTERNET_PORT& nPort);
BOOL AFXAPI AfxParseURLEx(LPCTSTR pstrURL, DWORD& dwServiceType,
	CString& strServer, CString& strObject, INTERNET_PORT& nPort,
	CString& strUsername, CString& strPassword, DWORD dwFlags = 0);

DWORD AFXAPI AfxGetInternetHandleType(HINTERNET hQuery);

// see CInternetException at the bottom of this file

void AFXAPI AfxThrowInternetException(DWORD_PTR dwContext, DWORD dwError = 0);

// these are defined by WININET.H

#define AFX_INET_SERVICE_FTP        INTERNET_SERVICE_FTP
#define AFX_INET_SERVICE_HTTP       INTERNET_SERVICE_HTTP
#define AFX_INET_SERVICE_GOPHER     INTERNET_SERVICE_GOPHER

// these are types that MFC parsing functions understand

#define AFX_INET_SERVICE_UNK        0x1000
#define AFX_INET_SERVICE_FILE       (AFX_INET_SERVICE_UNK+1)
#define AFX_INET_SERVICE_MAILTO     (AFX_INET_SERVICE_UNK+2)
#define AFX_INET_SERVICE_MID        (AFX_INET_SERVICE_UNK+3)
#define AFX_INET_SERVICE_CID        (AFX_INET_SERVICE_UNK+4)
#define AFX_INET_SERVICE_NEWS       (AFX_INET_SERVICE_UNK+5)
#define AFX_INET_SERVICE_NNTP       (AFX_INET_SERVICE_UNK+6)
#define AFX_INET_SERVICE_PROSPERO   (AFX_INET_SERVICE_UNK+7)
#define AFX_INET_SERVICE_TELNET     (AFX_INET_SERVICE_UNK+8)
#define AFX_INET_SERVICE_WAIS       (AFX_INET_SERVICE_UNK+9)
#define AFX_INET_SERVICE_AFS        (AFX_INET_SERVICE_UNK+10)
#define AFX_INET_SERVICE_HTTPS      (AFX_INET_SERVICE_UNK+11)



/////////////////////////////////////////////////////////////////////////////
// classes that are declared in this file

class CInternetSession
{
public:
	/* explicit */ CInternetSession(LPCTSTR pstrAgent = NULL,
		DWORD_PTR dwContext = 1,
		DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS,
		LPCTSTR pstrProxyName = NULL,
		LPCTSTR pstrProxyBypass = NULL,
		DWORD dwFlags = 0);

	BOOL QueryOption(DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufLen) const;
	BOOL QueryOption(DWORD dwOption, DWORD& dwValue) const;

	BOOL SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength,
			DWORD dwFlags = 0);
	BOOL SetOption(DWORD dwOption, DWORD dwValue, DWORD dwFlags = 0);

	CHttpFile* OpenURL(LPCTSTR pstrURL, CString sObject,
		DWORD_PTR dwContext = 1, DWORD dwFlags = INTERNET_FLAG_TRANSFER_ASCII,
		LPCTSTR pstrHeaders = NULL, DWORD dwHeadersLength = 0);

	//CFtpConnection* GetFtpConnection(LPCTSTR pstrServer,
	//	LPCTSTR pstrUserName = NULL, LPCTSTR pstrPassword = NULL,
	//	INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
	//	BOOL bPassive = FALSE);

	CHttpConnection* GetHttpConnection(LPCTSTR pstrServer,
		INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
		LPCTSTR pstrUserName = NULL, LPCTSTR pstrPassword = NULL);
	CHttpConnection* GetHttpConnection(LPCTSTR pstrServer, DWORD dwFlags,
		INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
		LPCTSTR pstrUserName = NULL, LPCTSTR pstrPassword = NULL);

	//CGopherConnection* GetGopherConnection(LPCTSTR pstrServer,
	//	LPCTSTR pstrUserName = NULL, LPCTSTR pstrPassword = NULL,
	//	INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER);

	BOOL EnableStatusCallback(BOOL bEnable = TRUE);

// Operations

	DWORD_PTR GetContext() const;
	operator HINTERNET() const;
	virtual void Close();

	// cookies
	static BOOL SetCookie(LPCTSTR pstrUrl, LPCTSTR pstrCookieName, LPCTSTR pstrCookieData);
	static BOOL GetCookie(_In_z_ LPCTSTR pstrUrl, _In_z_ LPCTSTR pstrCookieName, _Out_z_cap_(dwBufLen) LPTSTR pstrCookieData, _In_ DWORD dwBufLen);
	static DWORD GetCookieLength(LPCTSTR pstrUrl, LPCTSTR pstrCookieName);
	static BOOL GetCookie(LPCTSTR pstrUrl, LPCTSTR pstrCookieName, CString& strCookieData);

// Overridables
	virtual void OnStatusCallback(DWORD_PTR dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);

// Implementation
	//DECLARE_DYNAMIC(CInternetSession)
	~CInternetSession();

protected:
	DWORD_PTR m_dwContext;
	HINTERNET m_hSession;
	INTERNET_STATUS_CALLBACK m_pOldCallback;
	BOOL m_bCallbackEnabled;

public:
//#ifdef _DEBUG
//	virtual void Dump(CDumpContext& dc) const;
//#endif
};


////////////////////////////////////////////////////////////////////////////
// Internet File Access Wrapper

class CInternetFile/* : public CStdioFile*/
{
// Constructors
protected:
	CInternetFile(HINTERNET hFile, LPCTSTR pstrFileName,
		CInternetConnection* pConnection, BOOL bReadMode);
	CInternetFile(HINTERNET hFile, HINTERNET hSession,
		LPCTSTR pstrFileName, LPCTSTR pstrServer, DWORD_PTR dwContext,
		BOOL bReadMode);

// Attributes
protected:
	HINTERNET m_hFile;
public:
	operator HINTERNET() const;
	DWORD_PTR GetContext() const;

// Operations
	BOOL SetWriteBufferSize(UINT nWriteSize);
	BOOL SetReadBufferSize(UINT nReadSize);

	BOOL QueryOption(DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufLen) const;
	BOOL QueryOption(DWORD dwOption, DWORD& dwValue) const;

	BOOL SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength,
			DWORD dwFlags = 0);
	BOOL SetOption(DWORD dwOption, DWORD dwValue, DWORD dwFlags = 0);

// Overridables
	virtual ULONGLONG Seek(LONGLONG lOffset, UINT nFrom);

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);

	virtual void Abort();
	virtual void Flush();

	virtual void Close();
	virtual ULONGLONG GetLength() const;

	virtual BOOL ReadString(CString& rString);
	virtual LPTSTR ReadString(_Out_z_cap_(nMax) LPTSTR pstr, _In_ UINT nMax);
	virtual void WriteString(LPCTSTR pstr);

	// Not supported by CInternetFile
	void LockRange(ULONGLONG dwPos, ULONGLONG dwCount);
	void UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount);
	//CFile* Duplicate() const;
	virtual void SetLength(ULONGLONG dwNewLen);

// Implementation
public:
	virtual ~CInternetFile();

protected:
	CString m_strFileName;
	BOOL m_bReadMode;
	DWORD_PTR m_dwContext;
	HINTERNET m_hConnection;

	CString m_strServerName;

	UINT m_nWriteBufferSize;
	UINT m_nWriteBufferPos;
	LPBYTE m_pbWriteBuffer;

	UINT m_nReadBufferSize;
	UINT m_nReadBufferPos;
	LPBYTE m_pbReadBuffer;
	UINT m_nReadBufferBytes;

//#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
//#endif

	friend class CInternetSession;
	friend class CFtpConnection;
	friend class CHttpConnection;
	friend class CGopherConnection;
	//DECLARE_DYNAMIC(CInternetFile)
};

class CHttpHeaderMapEqualHelperFalse
{
public:
	static bool IsEqualKey(const CString& k1, const CString& k2)
	{
		return k1.CompareNoCase(k2) == 0;
	}

	static bool IsEqualValue(const CString&, const CString&)
	{
		ATLASSERT(FALSE);
		return false;
	}
};

// HTTP头字典
class CHttpHeaderMap : private CSimpleMap<CString, CString, CHttpHeaderMapEqualHelperFalse>
{
	typedef CSimpleMap<CString, CString, CHttpHeaderMapEqualHelperFalse>	baseClass;
public:
	// 解析HTTP头
	void ParseHttpHeader(LPTSTR pstrHeader);

	int GetSize() const;

	CString& GetKeyAt(int nIndex) const;

	CString& GetValueAt(int nIndex) const;

	int FindKey(const CString& key) const;

	// 获取值
	BOOL GetValue(CString strName, CString& strValue);
};

class CHttpFile : public CInternetFile
{
// Constructors
protected:
	CHttpFile(HINTERNET hFile, HINTERNET hSession, CString pstrObject,
		LPCTSTR pstrServer, LPCTSTR pstrVerb, DWORD_PTR dwContext);
	CHttpFile(HINTERNET hFile, LPCTSTR pstrVerb, LPCTSTR pstrObject,
		CHttpConnection* pConnection);

// Operations
public:
	BOOL AddRequestHeaders(LPCTSTR pstrHeaders,
		DWORD dwFlags = HTTP_ADDREQ_FLAG_ADD_IF_NEW, int dwHeadersLen = -1);
	BOOL AddRequestHeaders(CString& str,
		DWORD dwFlags = HTTP_ADDREQ_FLAG_ADD_IF_NEW);

	BOOL SendRequest(LPCTSTR pstrHeaders = NULL, DWORD dwHeadersLen = 0,
		LPVOID lpOptional = NULL, DWORD dwOptionalLen = 0);
	BOOL SendRequest(CString& strHeaders,
		LPVOID lpOptional = NULL, DWORD dwOptionalLen = 0);
	BOOL SendRequestEx(DWORD dwTotalLen,
		DWORD dwFlags = HSR_INITIATE,	DWORD_PTR dwContext = 1);
	BOOL SendRequestEx(LPINTERNET_BUFFERS lpBuffIn,
		LPINTERNET_BUFFERS lpBuffOut, DWORD dwFlags = HSR_INITIATE,
		DWORD_PTR dwContext = 1);
	BOOL EndRequest(DWORD dwFlags = 0,
		LPINTERNET_BUFFERS lpBuffIn = NULL, DWORD_PTR dwContext = 1);
	BOOL QueryInfo(DWORD dwInfoLevel, LPVOID lpvBuffer,
		LPDWORD lpdwBufferLength, LPDWORD lpdwIndex = NULL) const;
	BOOL QueryInfo(DWORD dwInfoLevel, CString& str,
		LPDWORD dwIndex = NULL) const;
	BOOL QueryInfo(DWORD dwInfoLevel, SYSTEMTIME* pSysTime,
		LPDWORD dwIndex = NULL) const;
	BOOL QueryInfo(DWORD dwInfoLevel, DWORD& dwResult,
		LPDWORD dwIndex = NULL) const;
	BOOL QueryInfoStatusCode(DWORD& dwStatusCode) const;

	BOOL QueryInfo(CHttpHeaderMap& headerMap) const;

	//DWORD ErrorDlg(CWnd* pParent = NULL,
	//	DWORD dwError = ERROR_INTERNET_INCORRECT_PASSWORD,
	//	DWORD dwFlags = FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
	//	LPVOID* lppvData = NULL);

// Attributes
public:
	CString GetVerb() const;
	void SetVerb(CString Verb);

//#pragma push_macro("GetObject")
//#undef GetObject
//	CString _AFX_FUNCNAME(GetObject)() const;
//	CString GetObject() const;
//#pragma pop_macro("GetObject")

	virtual CString GetFileURL() const;

// Implementation
public:
	virtual ~CHttpFile();
protected:
	CString m_strObject;
	CString m_strVerb;

//#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
//#endif

	friend class CHttpConnection;
	friend class CInternetSession;
	//DECLARE_DYNAMIC(CHttpFile)
};


////////////////////////////////////////////////////////////////////////////
// Connection types

class CInternetConnection
{
public:
	CInternetConnection(CInternetSession* pSession, LPCTSTR pstrServer,
		INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
		DWORD_PTR dwContext = 1);

// Operations
	operator HINTERNET() const;
	DWORD_PTR GetContext() const;
	CInternetSession* GetSession() const;

	CString GetServerName() const;

	BOOL QueryOption(DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufLen) const;
	BOOL QueryOption(DWORD dwOption, DWORD& dwValue) const;

	BOOL SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength,
			DWORD dwFlags = 0);
	BOOL SetOption(DWORD dwOption, DWORD dwValue, DWORD dwFlags = 0);

	virtual void Close();

// Implementation
protected:
	HINTERNET m_hConnection;
	DWORD_PTR m_dwContext;
	CInternetSession* m_pSession;

	CString m_strServerName;
	INTERNET_PORT m_nPort;

public:
	~CInternetConnection();
	//DECLARE_DYNAMIC(CInternetConnection)

//#ifdef _DEBUG
//	virtual void Dump(CDumpContext& dc) const;
//	void AssertValid() const;
//#endif
};



class CHttpConnection : public CInternetConnection
{
public:
	enum {
		_HTTP_VERB_MIN      = 0,
		HTTP_VERB_POST      = 0,
		HTTP_VERB_GET       = 1,
		HTTP_VERB_HEAD      = 2,
		HTTP_VERB_PUT       = 3,
		HTTP_VERB_LINK      = 4,
		HTTP_VERB_DELETE    = 5,
		HTTP_VERB_UNLINK    = 6,
		_HTTP_VERB_MAX      = 6,
	};

public:
	CHttpConnection(CInternetSession* pSession, HINTERNET hConnected,
		LPCTSTR pstrServer, DWORD_PTR dwContext);
	CHttpConnection(CInternetSession* pSession, LPCTSTR pstrServer,
		INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
		LPCTSTR pstrUserName = NULL, LPCTSTR pstrPassword = NULL,
		DWORD_PTR dwContext = 1);
	CHttpConnection(CInternetSession* pSession, LPCTSTR pstrServer,
		DWORD dwFlags, INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
		LPCTSTR pstrUserName = NULL, LPCTSTR pstrPassword = NULL,
		DWORD_PTR dwContext = 1);

	CHttpFile* OpenRequest(LPCTSTR pstrVerb,    LPCTSTR pstrObjectName,
		LPCTSTR pstrReferer = NULL,DWORD_PTR dwContext = 1,
		LPCTSTR* ppstrAcceptTypes = NULL, LPCTSTR pstrVersion = NULL,
		DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT);

	CHttpFile* OpenRequest(int nVerb, LPCTSTR pstrObjectName,
		LPCTSTR pstrReferer = NULL, DWORD_PTR dwContext = 1,
		LPCTSTR* ppstrAcceptTypes = NULL, LPCTSTR pstrVersion = NULL,
		DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT);

// Implementation
	~CHttpConnection();

protected:
	CString m_strServerName;
	static const LPCTSTR szHtmlVerbs[];

public:
//#ifdef _DEBUG
//	virtual void Dump(CDumpContext& dc) const;
//	virtual void AssertValid() const;
//#endif

	friend class CInternetSession;  // just to access szHtmlVerbs
	//DECLARE_DYNAMIC(CHttpConnection)
};


///////////////////////////////////////////////////////////////////////
// CInternetException

class CInternetException/* : public CException*/
{
public:
// Constructor
	CInternetException(DWORD dwError);

// Attributes
	DWORD m_dwError;
	DWORD_PTR m_dwContext;

// Implementation
public:
	~CInternetException();
//#ifdef _DEBUG
//	virtual void Dump(CDumpContext& dc) const;
//#endif
	virtual BOOL GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpstrError, _In_ UINT nMaxError,
		_Out_opt_ PUINT pnHelpContext = NULL) const;
	//DECLARE_DYNAMIC(CInternetException)
};


_AFXINET_INLINE DWORD_PTR CInternetSession::GetContext() const
	{ return m_dwContext; }

_AFXINET_INLINE CString CInternetConnection::GetServerName() const
	{ return m_strServerName; }

_AFXINET_INLINE CInternetSession* CInternetConnection::GetSession() const
	{ return m_pSession; }

_AFXINET_INLINE CInternetSession::operator HINTERNET() const
	{ return m_hSession; }

_AFXINET_INLINE BOOL CInternetSession::SetOption(DWORD dwOption, DWORD dwValue,
	DWORD dwFlags /* = 0 */)
	{ ASSERT((dwFlags & INTERNET_FLAG_ASYNC) == 0); return SetOption(dwOption, &dwValue, sizeof(dwValue), dwFlags); }



//#pragma push_macro("GetObject")
//#undef GetObject
//_AFXINET_INLINE CString CHttpFile::GetObject() const
//	{ return _AFX_FUNCNAME(GetObject)(); }
//#pragma pop_macro("GetObject")


_AFXINET_INLINE CInternetConnection::operator HINTERNET() const
	{ return m_hConnection; }

_AFXINET_INLINE DWORD_PTR CInternetConnection::GetContext() const
	{ return m_dwContext; }

_AFXINET_INLINE BOOL CInternetConnection::SetOption(DWORD dwOption,
	DWORD dwValue, DWORD dwFlags /* = 0 */)
	{ return SetOption(dwOption, &dwValue, sizeof(dwValue), dwFlags); }


_AFXINET_INLINE DWORD_PTR CInternetFile::GetContext() const
	{ return m_dwContext; }

_AFXINET_INLINE CInternetFile::operator HINTERNET() const
	{ return m_hFile; }

_AFXINET_INLINE BOOL CInternetFile::SetOption(DWORD dwOption, DWORD dwValue,
	DWORD dwFlags /* = 0 */)
	{ ASSERT((dwFlags & INTERNET_FLAG_ASYNC) == 0); return SetOption(dwOption, &dwValue, sizeof(dwValue), dwFlags); }

