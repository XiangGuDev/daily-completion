#pragma once
 
#include "WinINet.h"

namespace BwProj{
	namespace toolkit{

		#define  __BUFFER_SIZE 1024

		class CHttpFileClient
		{
		public:
			CHttpFileClient(void);
			~CHttpFileClient(void);
		public:
			BOOL UploadFile(LPCTSTR szRemoteURI, BYTE* btsIn, int btsLength);
			BOOL UploadFileRequest(LPCTSTR szRemoteURI, BYTE* btsIn, int btsLength,
				BYTE** btsOut, int* btsOutLength);
			BOOL DownLoadFile(LPCTSTR szRemoteURI, BYTE** btsOut, int* btsOutLength);
			BOOL DownLoadFile2(LPCTSTR szRemoteURI, BYTE** btsOut, int* btsOutLength);

			BOOL DeleteFile(LPCTSTR szRemoteURI);
			BOOL CanWebsiteVisit(CString sURI);
		private:
			BOOL UseHttpSendReqEx(CHttpFile* httpFile, BYTE* btsIn, int btsLength);
		};
	}
}