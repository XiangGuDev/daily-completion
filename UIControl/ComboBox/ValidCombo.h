//********************************************
//
//	@copyright		江西博微新技术有限公司
//	@file			ValidCombo.h
//	@author			郭永强
//	@date			2019/6/6 8:30
//	@brief		    可编辑校验下拉框
//********************************************
#pragma once
namespace ControlUI
{
	class CONTROL_UI CValidCombo : public CAutoWidthComboBox
	{
		DECLARE_DYNAMIC(CValidCombo)
	public:
		CValidCombo();
	public:
		DECLARE_MESSAGE_MAP()
		
	public:
		/**
		*  @brief    设置编辑标志

		*  @param	 int flag	标志
		*/
		void SetEditFlag(int nFlag);

		/**
		* @brief 设置小数点位数
		*
		* @param int nDotNum
		*
		* @return void
		*/
		void SetDotNum(int nDotNum);

		/**
		*  @brief    获取编辑标志

		*  @param	 int flag	标志
		*/
		int GetEditFlag() { return _nFlag; }

		/**
		*  @brief    禁用冒泡提示

		*  @param	 int flag	标志
		*/
		void DisableTip(bool bDisable) { 
			if (m_edit.GetSafeHwnd())
				m_edit.DisableTip(true);
		}

		/**
		*  @brief    是否正在显示提示

		*  @param	 int flag	标志
		*/
		bool IsShowingTip() { 
			if (m_edit.GetSafeHwnd())
				return m_edit.IsShowingTip();
			else
				return false;
			}

		/**
		*  @brief    检验输入值是否正确
		*/
		bool Validate() { 
			if (m_edit.GetSafeHwnd())
				return m_edit.Validate();
			else
				return true;
		}

	protected:
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnDestroy();

	private:
		CValidEditEx      m_edit;
		CListBox   m_listbox;
		int _nFlag;
		int _nDotNum;
	};
}
