////*****************************************************
//    
//    @copyright      ���������
//    @version        v1.0
//    @file           NumEdit.H
//    @author         JCC
//    @date           2018/9/5 17:11
//    @brief		  ������������ı༭��
//*****************************************************
#pragma once

namespace ControlUI
{
	class CONTROL_UI CNumEdit : public CEdit
	{
	public:
		CNumEdit();
		~CNumEdit();

		// ����С��λ��
		int SetDecimalNum(unsigned int iNum);

		// �����Ƿ�ֻ��������
		int SetIsPositiveNumber(bool bIsOrNot);

		// �����Ƿ�ֻ���ǷǸ�����
		void SetIsCanZero(bool bIsOrNot);

		// �����Ƿ�ֻ��������
		int SetIsInteger(bool bIsOrNot);

		// ʵ��  
	protected:
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags); //������Ϣ
		afx_msg void OnKillFocus(CWnd* pNewWnd); // ʧȥ����
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()

		/**
		*  @brief    �����ַ�
		*
		*  @param    const CString & strText
		*  @return   CString
		*/
		CString TrimString(const CString& strText);

	private:
		unsigned int m_decimalNum; // С��λ��
		bool m_bIsPositiveNumber; // �Ƿ�ֻ��������
		bool m_bIsCanZero; // �Ƿ���Ϊ0
		bool m_bIsInteger; // �Ƿ�ֻ��������
	};
}