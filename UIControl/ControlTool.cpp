#include "stdafx.h"
#include "ControlTool.h"

namespace ControlUI
{
	BEGIN_DYN_ENUM(ETreeListEditFlag)
	{
		{ eNoneEdit, _T("无编辑") },
		{ eNormalEdit, _T("普通编辑") },
		{ eValidEdit, _T("检验编辑") },
		{ eFileNameEdit, _T("文件名编辑") },
		{ eDateTimeEdit, _T("日期编辑框") },
		{ eDropListCobEdit, _T("下拉选择框") },
		{ eDropDownCobEdit, _T("可编辑的下拉选择框") },
		{ eCheckBoxEdit, _T("复选框") },
		{ eCheckBox3StateEdit, _T("三态复选框") },
		{ eHyperLinkEdit, _T("超链接") },
		{ eAngleEdit, _T("角度") },
		{ eTreeComboEdit, _T("树下拉框") },
		{ eProgressEdit, _T("进度条") },
		{ eTreeCheckComboEdit, _T("下拉树多选框") },
		{ eRangelEdit, _T("范围编辑控件") },
		{ eAutoSearchCombo, _T("自动搜索组合框") },
		{ eTreeComboManualEdit, _T("可编辑的树下拉框") },
		{ eTreeCheckComboManualEdit, _T("可编辑的树下拉多选框") },
		{ NULL, NULL }
	}
	END_DYN_ENUM(ETreeListEditFlag);

	int ControlUI::CControlTool::GetOptionEnum(const CString& str)
	{
		//当Option关键字对应值为0或为空时
		if (str == L"0" || str == L"")
			return 0;

		std::vector<CString> ArrStr;
		CStringTool::SplitString((LPCTSTR)str, ArrStr, L"|");

		//获取第一个关键字
		int nEditFlag = TextToETreeListEditFlag(ArrStr.at(0));
		int nSubFlag = 0;
		for (int i = 1; i < (int)ArrStr.size(); ++i)
		{
			switch (nEditFlag)
			{
				//若类型为编辑框
			case eNoneEdit:
			case eValidEdit:
			case eRangelEdit:
			{
				//继续分析编辑框专用标志位
				nSubFlag += TextToEValidEditControlFlag(ArrStr.at(i));
				break;
			}
			//若类型为树下拉框
			case eTreeComboEdit:
			case eTreeComboManualEdit:
			{
				//继续分析下拉框专用标志位
				nSubFlag += TextToETreeComboEditControlFlag(ArrStr.at(i));
				break;
			}
			//若类型为日期编辑框
			case eDateTimeEdit:
			{
				//继续分析下拉框专用标志位
				nSubFlag += TextToEDateTimeEditFlag(ArrStr.at(i));
				break;
			}
			//若类型为进度条
			case eProgressEdit:
			{
				//继续分析进度条专用标志位
				nSubFlag += TextToEProgressControlFlag(ArrStr.at(i));
				break;
			}
			case eTreeCheckComboEdit:
			{
				//继续分析下拉树多选框标志位
				nSubFlag += TextToETreeComboEditControlFlag(ArrStr.at(i));
				break;
			}
			default:
				break;
			}
		}
		return nEditFlag + nSubFlag;
	}
}