// 通用Tab面板对话框实现
// _______________________________________________________________
//|	tab页 | tab页 |												  |
//|_______________________________________________________________|
//|																  |
//|																  |
//|																  |
//|					子页面内容ITabPageContext					  |
//|																  |															
//|																  |	
//|																  |
//|_______________________________________________________________|
#pragma once
#include <memory>

class ITabPageContext : public IObject
{
public:
	ITabPageContext() {};
	virtual ~ITabPageContext() {};

	virtual void Leave() {};
	virtual void Enter() {};
	virtual void PutParam(LPCTSTR key, IObject *pObject) = 0;
	virtual HWND GetWndHandle(HWND parent) { return NULL; }
	virtual LPCTSTR GetTabName() { return L""; }
};

class ITabPageContextWithCfgName : public ITabPageContext
{
public:
	void SetCaptionName(LPCTSTR strName) { _strCaptionName = strName; }
	virtual LPCTSTR GetTabName() { return _strCaptionName; }
	virtual void PutParam(LPCTSTR key, IObject *pObject)
	{
		_mapKeyParam[key] = pObject;
	}
	IObject* GetParam(LPCTSTR key)
	{
		auto finder = _mapKeyParam.find(key);
		return finder == _mapKeyParam.end() ? NULL : finder->second;
	}
protected:
	CString _strCaptionName;
	std::map<CString, IObject*> _mapKeyParam;	// 输入参数
};

typedef std::vector<ITabPageContext*> TabPageContextArray;

class ITabPageContextFactory
{
public:
	ITabPageContextFactory() {}
	virtual ~ITabPageContextFactory() {}
	virtual ITabPageContext* CreateObject() { return NULL; }
};

// 页面对话框
class COMMONCONTROLS_API CTabPagedDialog
{
public:
	CTabPagedDialog();
	~CTabPagedDialog();
	
public:
	// 通过配置文件创建对话框
	static CTabPagedDialog* CreateDialogByXmlCfg(HWND hWnd, LPCTSTR szXmlCfg);
	static void RegistPageFactory(LPCTSTR szPageClassName, ITabPageContextFactory *pFactory);
	
	int DoModal();
	HWND ShowWnd();
	void PutParam(LPCTSTR key, IObject *pObject);

protected:
	static ITabPageContext* CreatePageByKey(LPCTSTR szPageClassName);

protected:
	CDialog				*m_pImpl;			// 主页面
	TabPageContextArray m_arrSubPages;		// 子页面
	static std::map<CString, ITabPageContextFactory*> s_mapFactory;
};

typedef std::shared_ptr<CTabPagedDialog> CTabPagedDialogPtr;

#define REGIST_PAGE_CONTEXT(key,className)\
class className##factory : public ITabPageContextFactory\
{\
public:\
	className##factory()\
	{\
		CTabPagedDialog::RegistPageFactory(key, this);\
	}\
	virtual ITabPageContext* CreateObject() { return new className(); }\
} s_##className##factoryInst;