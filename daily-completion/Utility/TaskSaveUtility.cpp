#include "pch.h"
#include "../Model/Task.h"
#include "TaskSaveUtility.h"

CTaskSaveUtility::CTaskSaveUtility()
{
	// ¶ÁÈ¡ÎÄ¼þ
	SYSTEMTIME st;
	CString filename = L"";
	GetLocalTime(&st);
	filename.Format(L"TaskData_%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	_strDataPath = CPathConfig::GetAppStartPath() + L"Data/" + filename + L".xml";
	_strFixedPath = CPathConfig::GetAppStartPath() + L"Data/FixedTask.xml";
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
		elem->SetElementName(L"Task");
		elem->SetAttrValue(L"name", task->strName);
		elem->SetAttrValue(L"fixed", CConvert::Bool2Text(task->bFixed));
		elem->SetAttrValue(L"complete", CConvert::Bool2Text(task->bComplete));
	}
	_doc.SaveFile(_strDataPath, fmtXMLUTF8);
	SaveFixed(inVec);
}

void CTaskSaveUtility::LoadData(std::vector<std::shared_ptr<Task>>& outVec)
{
	outVec.clear();
	_doc.Clear();
	_bDataEmpty = !CFileTool::FileExist(_strDataPath);
	if (!_doc.LoadFile(_strDataPath, fmtXMLUTF8))
	{
		OutputDebugString(L"_doc load failed");
		return;
	}
	auto pRoot = _doc.GetElementRoot();
	if (pRoot == NULL)return;
	auto elems = pRoot->GetChildElements();
	for (int i = 0; i < elems->GetCount(); ++i)
	{
		auto elem = elems->GetAt(i);
		auto task = std::make_shared<Task>();
		task->strName = elem->GetAttrValue(L"name");
		task->bComplete = CConvert::Text2Bool(elem->GetAttrValue(L"complete"));
		task->bFixed = CConvert::Text2Bool(elem->GetAttrValue(L"fixed"));
		outVec.push_back(task);
	}
}

void CTaskSaveUtility::SaveFixed(const std::vector<std::shared_ptr<Task>> &inVec)
{
	_docFixed.Clear();
	auto pRoot = _docFixed.GetElementRoot();
	if (pRoot == NULL)return;
	auto elems = pRoot->GetChildElements();
	if (inVec.empty()) return;
	int idx = 0;
	for (int i = 0; i < inVec.size(); ++i)
	{
		auto task = inVec[i];
		if (!task->bFixed)continue;
		auto elem = elems->InsertAt(idx++);
		elem->SetElementName(L"FixedTask");
		elem->SetAttrValue(L"name", task->strName);
		elem->SetAttrValue(L"fixed", CConvert::Bool2Text(task->bFixed));
		elem->SetAttrValue(L"complete", CConvert::Bool2Text(task->bComplete));
	}
	if (pRoot->GetChildElementCount() == 0)return;
	_docFixed.SaveFile(_strFixedPath, fmtXMLUTF8);
}

void CTaskSaveUtility::SetDate(int y, int m, int d)
{
	CString filename = L"";
	filename.Format(L"TaskData_%04d-%02d-%02d", y, m, d);
	_strDataPath = CPathConfig::GetAppStartPath() + L"Data/" + filename + L".xml";
}

void CTaskSaveUtility::LoadFixed(std::vector<std::shared_ptr<Task>> &outVec)
{
	outVec.clear();
	_docFixed.Clear();
	if (!_docFixed.LoadFile(_strFixedPath, fmtXMLUTF8))
	{
		OutputDebugString(L"_docFixed load failed");
		return;
	}

	auto pRoot = _docFixed.GetElementRoot();
	if (pRoot == NULL)return;
	auto elems = pRoot->GetChildElements();
	for (int i = 0; i < elems->GetCount(); ++i)
	{
		auto elem = elems->GetAt(i);
		auto task = std::make_shared<Task>();
		task->strName = elem->GetAttrValue(L"name");
		task->bFixed = CConvert::Text2Bool(elem->GetAttrValue(L"fixed"));
		task->bComplete = CConvert::Text2Bool(elem->GetAttrValue(L"complete"));
		outVec.push_back(task);
	}
}

