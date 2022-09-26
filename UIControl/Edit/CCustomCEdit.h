#pragma once

namespace ControlUI
{
	class CONTROL_UI CCustomCEdit : public CValidEdit
	{
	public:
		CCustomCEdit();
		~CCustomCEdit();

		DECLARE_DYNAMIC(CCustomCEdit)
		DECLARE_MESSAGE_MAP()
	public:

		afx_msg void OnNcPaint();
		afx_msg void OnPaint();

	public:
		void SetStatus(bool bStatus);
	private:
		bool _bStatus;
	};
}