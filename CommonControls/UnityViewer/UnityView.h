//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           UnityView.h
//    @author         yf
//    @data           2021/08/24  
//    @brief          unity view用来包括unity对话框（无实际用处，就是一个容器）
//
//*****************************************************

#pragma once

class CUnityView : public CView, public d3s::views::view::IViewPort
{
public:

	CUnityView(d3s::designfile::model::DesignModel *pModel, d3s::share_ptr<d3s::views::view::CViewInfo> pInfo);


	virtual ~CUnityView();


	/**
	* 创建视口
	* @return
	*/
	virtual bool CreateViewport(HWND parent);

	/**
	* 获取工具栏配置路径
	* @return
	*/
	virtual CString GetToolbarConfig();
	
	/**
	* 设置工具栏配置路径
	* @return
	*/
	virtual void SetToolbarConfig(CString strToolBarFile);

	/**
	* 获取线程ID
	* @return
	*/
	virtual int GetThreadID();


	/**
	* 窗口销毁（重载销毁方法，不进行析构，绘制线程中会析构）
	* @return void
	*/
	virtual void PostNcDestroy();


	virtual void OnDraw(CDC* pDC);

	/**
	* 获取视图名称
	* @return
	*/
	virtual CString GetViewName() override;

	/**
	* 获取osg句柄
	* @return
	*/
	virtual HWND GetHwnd() override;

	/**
	* 获取视图标题
	* @return
	*/
	virtual CString GetViewTitle() override;

	/**
	* 设置自定义的视图标题
	* @return
	*/
	virtual void SetViewTitle(CString strViewTitle) override;
protected:
	afx_msg void OnSize(UINT nFlags, int cx, int cy);
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()


private:
	CUnityViewPort *_pUnityDlg; // unity对话框
	CString _strTitle; // 标题名称
	bool _bMouseTracking; // 是否检测鼠标
	CString _strToolConfigBarFile; // 工具栏配置路径
};

