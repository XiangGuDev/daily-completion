#include "pch.h"
#include "PlayerPrefsStorage.h"

PlayerPrefsStorage::PlayerPrefsStorage()
{
	// ¶ÁÈ¡ÎÄ¼þ
	_strDataPath = CPathConfig::GetAppStartPath() + L"Output/GameData.xml";
	if (CFileTool::FileExist(_strDataPath) && !_doc.LoadFile(_strDataPath, fmtXMLUTF8))
	{
		ASSERT(FALSE);
		return;
	}
}

void PlayerPrefsStorage::SaveInt(const CString &key, int val)
{
	auto pRoot = _doc.GetElementRoot();
	if (pRoot == NULL)return;
	pRoot->SetAttrValue(key, CConvert::I642Text(val));
	_doc.SaveFile(_strDataPath, fmtXMLUTF8);
}

int PlayerPrefsStorage::LoadInt(const CString &key)
{
	auto pRoot = _doc.GetElementRoot();
	if (pRoot == NULL)return 0;

	return CConvert::Text2Int(pRoot->GetAttrValue(key)) ;
}

bool PlayerPrefsStorage::DataExist(const CString & key)
{
	auto pRoot = _doc.GetElementRoot();
	if (pRoot == NULL)return false;
	int idx;
	return pRoot->FindAttribute(key, idx);
}


