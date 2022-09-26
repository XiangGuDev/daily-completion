
#pragma once

// 单列类宏定义
#define DECLARE_SINGLE_CLASS(ClassType)\
public:								   \
	static ClassType* Instance(); \
protected:							   \
	static ClassType* _pInstance;

// 单列类宏实现
#define IMPL_SINGLE_CLASS(ClassType)   \
	ClassType* ClassType::_pInstance = NULL; \
	ClassType* ClassType::Instance()  \
{									   \
	if (_pInstance == NULL)			   \
	_pInstance = new ClassType();		   \
	return _pInstance;				   \
}


/*注册命令类宏
*/
#define REGISTER_COMMAND(CmdName,CmdClass,CmdCheckMdl) \
void CmdFun##CmdClass()\
{\
	CmdClass cmdInstance;\
	cmdInstance.StartCommand(CmdName, CmdCheckMdl);\
	CLog::LogInfoV(L"执行命令：%s", CmdName);\
}\
class Register##CmdClass##Cmd \
{ \
	public: \
		Register##CmdClass##Cmd () \
		{\
		d3s::platform::command::CCommandMgr::GetCmdManager()->UnRegistExecuteFunc(CmdName);\
		d3s::platform::command::CCommandMgr::GetCmdManager()->RegistExecuteFunc(CmdName, CmdFun##CmdClass);\
		} \
		~Register##CmdClass##Cmd () \
		{\
		d3s::platform::command::CCommandMgr::GetCmdManager()->UnRegistExecuteFunc(CmdName);\
		} \
} Register##CmdClass##Cmd##Instance;

/*
* 注册命令函数宏(项目命令格式void CmdFunc(CString strCmd, ...)
*/
#define REGISTER_COMMAND_FUNCTION(CmdName,CmdFunc,CmdCheckMdl,...) \
void CmdFun##CmdName()\
{\
	if (CmdCheckMdl & eCheckProject || (CmdCheckMdl & eCheckViewPort))\
	{\
		if (CComandBase::GetState() != 1)\
		{\
			AfxGetMainWnd()->MessageBox(L"未找到许可证信息！", L"错误", MB_ICONINFORMATION | MB_OK);\
			return;\
		}\
		d3s::views::tool::COperatorManager::GetInst()->SetActiveTool(NULL);\
		d3s::platform::frame::IMainFrame *pMainFrame = d3s::platform::frame::CFrameManager::GetMainFrame();\
		if (pMainFrame == NULL)\
			return;\
		d3s::views::view::IOsgViewPort *pActiveView = dynamic_cast<d3s::views::view::IOsgViewPort*>(pMainFrame->GetActiveViewPort());\
		if (pActiveView == NULL)\
		{\
			::MessageBox(GetActiveWindow(), L"请打开工程后再继续操作！", L"提示", MB_OK|MB_ICONINFORMATION);\
			return; \
		}\
		if (!(CmdCheckMdl & eNoEmpSelection))\
		{\
			CommBase::SafeEmtpyAllSelect();\
		}\
	}\
	CmdFunc(L#CmdName, ##__VA_ARGS__);\
	CLog::LogInfoV(L"执行命令：%s", L#CmdName);\
}\
class Register##CmdName \
{ \
	public: \
		Register##CmdName() \
		{\
		d3s::platform::command::CCommandMgr::GetCmdManager()->UnRegistExecuteFunc(L#CmdName);\
		d3s::platform::command::CCommandMgr::GetCmdManager()->RegistExecuteFunc(L#CmdName, CmdFun##CmdName);\
		} \
		~Register##CmdName() \
		{\
		d3s::platform::command::CCommandMgr::GetCmdManager()->UnRegistExecuteFunc(L#CmdName);\
		} \
} Register##CmdName##Instance;