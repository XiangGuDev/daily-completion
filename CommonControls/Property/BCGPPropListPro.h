/*-----------------------------------------------------
* 文件名称：  BCGPPropEx.H
*  功能描述： 扩展属性面板
* 创建标识：  张志勇  2020/7/6 8:59
*
* 修改标识：
* 修改描述：
*
* 修改标识：
* 修改描述：
-----------------------------------------------------*/

#pragma once

namespace PropEx
{
	class COMMONCONTROLS_API CBCGPPropListPro : public CBCGPPropList
	{
		DECLARE_DYNAMIC(CBCGPPropListPro)
	public:
		CBCGPPropListPro();
		virtual ~CBCGPPropListPro();

		void RemoveAllProperty();
		BOOL IsFocused();
		CString True() { return m_strTrue; };
		CString False() { return m_strFalse; };

		DECLARE_MESSAGE_MAP()
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	private:
		static volatile bool					_bDelete;//标识本控件是否已被释放
	};

}

