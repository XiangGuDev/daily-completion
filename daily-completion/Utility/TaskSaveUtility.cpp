#include "pch.h"
#include "../Model/Task.h"
#include "TaskSaveUtility.h"

CTaskSaveUtility::CTaskSaveUtility()
{
	SYSTEMTIME st;
	CString filename = L"";
	GetLocalTime(&st);
	filename.Format(L"TaskData_%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);

	CString strPersistent = CPathConfig::GetPersistentPath();
	CString strData = CPathConfig::GetPersistentPath() + L"Data\\";
	if (!CFileTool::DirectoryExist(strPersistent)) CFileTool::CreateDirectory(strPersistent);
	if (!CFileTool::DirectoryExist(strData)) CFileTool::CreateDirectory(strData);

	_strDataPath = CPathConfig::GetPersistentPath() + L"Data/" + filename + L".xml";
	_strFixedPath = CPathConfig::GetPersistentPath() + L"Data/FixedTask.xml";
}

void CTaskSaveUtility::SaveData(const std::vector<std::shared_ptr<Task>>& inVec)
{
	CXmlDocument doc;
	doc.Clear();
	auto pRoot = doc.GetElementRoot();
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
	doc.SaveFile(_strDataPath, fmtXMLUTF8);
	SaveFixed(inVec);
}

void CTaskSaveUtility::LoadData(std::vector<std::shared_ptr<Task>>& outVec)
{
	CXmlDocument doc;
	outVec.clear();
	doc.Clear();
	_bDataEmpty = !CFileTool::FileExist(_strDataPath);
	if (!doc.LoadFile(_strDataPath, fmtXMLUTF8))
	{
		OutputDebugString(L"doc load failed");
		return;
	}
	auto pRoot = doc.GetElementRoot();
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
	CXmlDocument docFixed;
	docFixed.Clear();
	auto pRoot = docFixed.GetElementRoot();
	if (pRoot == NULL)return;
	auto elems = pRoot->GetChildElements();
	int idx = 0;
	for (int i = 0; i < inVec.size(); ++i)
	{
		auto task = inVec[i];
		if (!task->bFixed)continue;
		auto elem = elems->InsertAt(idx++);
		elem->SetElementName(L"FixedTask");
		elem->SetAttrValue(L"name", task->strName);
		elem->SetAttrValue(L"fixed", CConvert::Bool2Text(task->bFixed));
		elem->SetAttrValue(L"complete", CConvert::Bool2Text(false));
	}
	docFixed.SaveFile(_strFixedPath, fmtXMLUTF8);
}

void CTaskSaveUtility::SetDate(int y, int m, int d)
{
	CString filename = L"";
	filename.Format(L"TaskData_%04d-%02d-%02d", y, m, d);
	_strDataPath = CPathConfig::GetPersistentPath() + L"Data/" + filename + L".xml";
}

void CTaskSaveUtility::LoadFixed(std::vector<std::shared_ptr<Task>> &outVec)
{
	CXmlDocument docFixed;
	outVec.clear();
	docFixed.Clear();
	if (!docFixed.LoadFile(_strFixedPath, fmtXMLUTF8))
	{
		OutputDebugString(L"docFixed load failed");
		return;
	}

	auto pRoot = docFixed.GetElementRoot();
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

