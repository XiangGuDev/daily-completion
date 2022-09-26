#include "stdafx.h"
#include "LockClientService.h"
#include "InbreakProtect.h"
#include "LicenceAccess.h"
#include "LockClientDialog.h"

IMPL_SINGLE_CLASS(CLockClientService)
UINT_PTR CLockClientService::s_nCheckTm = 0; ///< 检测锁定时器
int CLockClientService::s_nLockInitStatus = 0; ///< 锁初始化状态
bool CLockClientService::_bNetLogin = false;

CLockClientService::CLockClientService()
{
	InitLockDetect();
}

CLockClientService::~CLockClientService()
{
	UninitLockDetect();
}

bool CLockClientService::CheckLicence(int nMode)
{
	VMProtectBegin("Lic5");

#ifndef USE_LOCK//未使用锁判断
	return true;	// 不进行锁检测时，直接返回
#endif

	CLicenceAccess* pLicAccess = CLicenceAccess::GetInstance();
	if (pLicAccess == NULL)
		return false;

	SupportLicMap licMap = pLicAccess->GetAppSupperLicenceMap();
	SupportLicMap::iterator iter = licMap.find(nMode);
	if (iter == licMap.end())
		return false;

	return pLicAccess->CheckFunctionLicence(nMode);

	VMProtectEnd();
}

void CLockClientService::InitLockDetect()
{
	VMProtectBegin("CommonTools");

#ifdef USE_LOCK//未使用锁判断
	// 锁初始化
	void* pVoid = NULL;
	LockInitial(pVoid);

	// 开启锁检测定时器
	LockFirst();
	//s_nCheckTm = SetTimer(NULL, 0, 500, LockDetectProc);

#endif

	VMProtectEnd();
}

UINT CLockClientService::LockInitial(LPVOID lpParam)
{
	VMProtectBegin("LockInit");

	CLicenceAccess* pLicAccess = CLicenceAccess::GetInstance();
	bool bResult = pLicAccess->CreateLock();

	// 创建锁失败
	if (!bResult)
	{
		s_nLockInitStatus = -1;
		pLicAccess->ReleaseLock();
		toolkit::commkit::CLog::WriteLog(L"锁控件创建失败-" + pLicAccess->GetLastError());
	}
	else
	{
		s_nLockInitStatus = 1;
	}

	VMProtectEnd();

	return 0;
}

void CLockClientService::LockFirst()
{
	VMProtectBegin("LockDetect");

	if (1 != s_nLockInitStatus)
	{
		return;
	}

	CLicenceAccess* pLicAccess = CLicenceAccess::GetInstance();
	try
	{
		do
		{
			bool bResult = false;

			// 检查单机锁或网络锁
			if (!_bNetLogin)
			{
				bResult = pLicAccess->CheckNatLock();
			}
			else
			{
				bResult = pLicAccess->CheckNetLock();

				// 重置网络锁登陆
				_bNetLogin = bResult;
			}

			// 找到许可证
			if (bResult)
			{
				s_nCheckTm = SetTimer(NULL, 0, 20 * 1000, LockDetectProc);
				return;
			}

#ifdef USE_LOCK//未使用锁判断
			// 加入调试检测(先注释掉，在Realease版下才可使用)
			//CInbreakProtect apiProtect;
			//if (apiProtect.DebugDetection(1) == 0)
			//{
			//	CLog::LogError(L"检测到反调试异常！");
			//	exit(0);
			//}
#endif

			// 没有可用许可证显示提示信息
			INIT_RESHANDLE();
			CWnd* pMainWnd = CWnd::GetActiveWindow();
			CNoFoundLienceDlg dlg(pMainWnd);
			dlg.DoModal();

			// 根据提示操作进行进一步处理
			ENotFoundMode noFoundMode = dlg.GetNoFoundMode();
			if (noFoundMode == eShowLic)
			{
				// 显示许可证管理(注册锁)
				CServerLicenceMgrDlg dlgServerLicenceMgrDlg(pMainWnd);
				dlgServerLicenceMgrDlg.DoModal();
			}
			else if (noFoundMode == eNetLogin)
			{
				if (dlg.IsManualLoginIn())
				{
					CNetLoginDlg LoginDlg(pLicAccess);
					LoginDlg.DoModal();
					if (LoginDlg.LoginSucceed() != TRUE)
					{
						continue;
					}
					_bNetLogin = true;
					s_nCheckTm = SetTimer(NULL, 0, 5 * 1000, LockDetectProc);
					return;
				}
				else
				{
					// 网络登录并进行检测
					if (pLicAccess->AppNetLogin())
					{
						_bNetLogin = true;
						s_nCheckTm = SetTimer(NULL, 0, 20 * 1000, LockDetectProc);
						return;
					}
					else
					{
						toolkit::commkit::CLog::WriteLog(L"网络登陆失败-" + pLicAccess->GetLastError());
						::MessageBox(GetActiveWindow(), pLicAccess->GetLastError(), L"错误", MB_OK | MB_ICONINFORMATION);
					}
				}

			}
			else if (noFoundMode == eQuitLock)
			{
				exit(1);
				return;
			}
		} while (true);
	}
	catch (_com_error& er)
	{
		CString strMsg;
		strMsg.Format(L"锁组件访问异常，错误码:%d", er.Error());
		::MessageBox(GetActiveWindow(), strMsg, L"错误", MB_OK | MB_ICONINFORMATION);
	}
	catch (...)
	{
		CString strMsg = L"锁组件访问异常，未知错误!";
		::MessageBox(GetActiveWindow(), strMsg, L"错误", MB_OK | MB_ICONINFORMATION);
	}
	VMProtectEnd();
}

void CALLBACK CLockClientService::LockDetectProc(HWND, UINT, UINT_PTR, DWORD)
{
	VMProtectBegin("LockDetect");

	// 锁正在初始化
	if (0 == s_nLockInitStatus)
	{
		// 30秒未初始化完成则认为是确解了
		static int nInitCount = 0;
		if (++nInitCount > 20 * 30)
		{
			KillTimer(NULL, s_nCheckTm);
		}
		return;
	}
	// 锁初始化出错
	else if (-1 == s_nLockInitStatus)
	{
		KillTimer(NULL, s_nCheckTm);
		return;
	}

	// 暂停定时器
	KillTimer(NULL, s_nCheckTm);

	CLicenceAccess* pLicAccess = CLicenceAccess::GetInstance();
	try
	{
		do
		{
			bool bResult = false;

			// 检查单机锁或网络锁
			if (!_bNetLogin)
			{
				bResult = pLicAccess->CheckNatLock();
			}
			else
			{
				bResult = pLicAccess->CheckNetLock();

				// 重置网络锁登陆
				_bNetLogin = bResult;
			}

			// 找到许可证
			if (bResult)
			{
				s_nCheckTm = SetTimer(NULL, 0, 20 * 1000, LockDetectProc);
				return;
			}

#ifdef USE_LOCK//未使用锁判断
			// 加入调试检测(先注释掉，在Realease版下才可使用)
			//CInbreakProtect apiProtect;
			//if (apiProtect.DebugDetection(1) == 0)
			//{
			//	CLog::LogError(L"检测到反调试异常！");
			//	exit(0);
			//}
#endif

			// 没有可用许可证显示提示信息
			INIT_RESHANDLE();
			CWnd* pMainWnd = CWnd::GetActiveWindow();
			CNoFoundLienceDlg dlg(pMainWnd);
			dlg.DoModal();

			// 根据提示操作进行进一步处理
			ENotFoundMode noFoundMode = dlg.GetNoFoundMode();
			if (noFoundMode == eShowLic)
			{
				// 显示许可证管理(注册锁)
				CServerLicenceMgrDlg dlgServerLicenceMgrDlg(pMainWnd);
				dlgServerLicenceMgrDlg.DoModal();
			}
			else if (noFoundMode == eNetLogin)
			{
				if (dlg.IsManualLoginIn())
				{
					CNetLoginDlg LoginDlg(pLicAccess);
					LoginDlg.DoModal();
					if (LoginDlg.LoginSucceed() != TRUE)
					{
						continue;
					}
					_bNetLogin = true;
					s_nCheckTm = SetTimer(NULL, 0, 5 * 1000, LockDetectProc);
					return;
				}
				else
				{
					// 网络登录并进行检测
					if (pLicAccess->AppNetLogin())
					{
						_bNetLogin = true;
						s_nCheckTm = SetTimer(NULL, 0, 20 * 1000, LockDetectProc);
						return;
					}
					else
					{
						toolkit::commkit::CLog::WriteLog(L"网络登陆失败-" + pLicAccess->GetLastError());
						::MessageBox(GetActiveWindow(), pLicAccess->GetLastError(), L"错误", MB_OK | MB_ICONINFORMATION);
					}
				}

			}
			else if (noFoundMode == eQuitLock)
			{
				KillTimer(NULL, s_nCheckTm);

				// 通知软件关闭
				// ::PostMessage(d3s::platform::frame::CFrameManager::GetMainFrame()->GetMainWindow()->m_hWnd, WM_CLOSE, (WPARAM)0, (LPARAM)0);
				exit(1);
				return;
			}

		} while (true);
	}
	catch (_com_error& er)
	{
		KillTimer(NULL, s_nCheckTm);
		CString strMsg;
		strMsg.Format(L"锁组件访问异常，错误码:%d", er.Error());
		::MessageBox(GetActiveWindow(), strMsg, L"错误", MB_OK | MB_ICONINFORMATION);
	}
	catch (...)
	{
		KillTimer(NULL, s_nCheckTm);
		CString strMsg = L"锁组件访问异常，未知错误!";
		::MessageBox(GetActiveWindow(), strMsg, L"错误", MB_OK | MB_ICONINFORMATION);
	}

	VMProtectEnd();
}

void CLockClientService::UninitLockDetect()
{
	VMProtectBegin("UninitLock");

	// 移除单例标记
	//s_instanceMapping.Unmap();

#ifdef USE_LOCK//未使用锁判断

	// 关闭锁检测定时器
	::KillTimer(NULL, s_nCheckTm);

	// 释放锁控件
	CLicenceAccess::GetInstance()->ReleaseLock();

	CLicenceAccess::ReleaseInstance();
#endif

	VMProtectEnd();
}
