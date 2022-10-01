#include "pch.h"
#include "../Model/Task.h"
#include "TaskSaveUtility.h"

CTaskSaveUtility::CTaskSaveUtility()
{
	// ¶ÁÈ¡ÎÄ¼þ
	SYSTEMTIME st;
	CString filename = L"";
	GetLocalTime(&st);
	filename.Format(L"%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	_strDataPath = CPathConfig::GetAppStartPath() + L"Output/" + filename + L".xml";
	if (CFileTool::FileExist(_strDataPath) && !_doc.LoadFile(_strDataPath, fmtXMLUTF8))
	{
		ASSERT(FALSE);
		return;
	}
}

void CTaskSaveUtility::SaveData(const std::vector<std::shared_ptr<Task>>& inVec)
{
	_doc.Clear();
	auto pRoot = _doc.GetElementRoot();
	if (pRoot == NULL)return;
	auto elems = pRoot->GetChildElements();
	for (int i = 0; i < inVec.size(); ++i)
	{
		auto task = inVec[i];
		auto elem = elems->InsertAt(i);
		elem->SetAttrValue(L"name", task->strName);
		elem->SetAttrValue(L"complete", CConvert::Bool2Text(task->bComplete));
	}
	_doc.SaveFile(_strDataPath, fmtXMLUTF8);
}

void CTaskSaveUtility::LoadData(std::vector<std::shared_ptr<Task>>& outVec)
{
	auto pRoot = _doc.GetElementRoot();
	if (pRoot == NULL)return;
	auto elems = pRoot->GetChildElements();
	for (int i = 0; i < elems->GetCount(); ++i)
	{
		auto elem = elems->GetAt(i);
		auto task = std::make_shared<Task>();
		task->strName = elem->GetAttrValue(L"name");
		task->bComplete = CConvert::Text2Bool(elem->GetAttrValue(L"complete"));
		outVec.push_back(task);
	}
}

