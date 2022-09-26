#include "StdAfx.h"
#include "DynEnum.h"
#include "../Sdk/XmlDocument.h"

namespace CommBase
{
	///< 定义枚举名与信息Map
	static CString s_strLastError;
	static EnumTextInfoMap s_enumTextInfoMap;

	void ParseEnumInfo(CXmlElement* pElement, EnumTextInfoArray& enumTextInfos)
	{
		// 遍历解析EnumInfo节点
		CXmlElements *pEnumInfoElms = pElement->GetChildElements();
		size_t nElmCount = pEnumInfoElms->GetCount();
		for (size_t i = 0; i < nElmCount; ++i)
		{
			CXmlElement *pEnumElmInfo = pEnumInfoElms->GetAt(i);

			EnumTextInfo info;
			info.Value = _ttoi(pEnumElmInfo->GetAttrValue(L"Value"));
			info.Text = pEnumElmInfo->GetAttrValue(L"Text");
			// show属性如果不存在则默认读取text属性
			info.Show = pEnumElmInfo->GetAttrValue(L"Show", info.Text);

			enumTextInfos.push_back(info);
		}
	}

	bool ParseEnum(CXmlElement* pElement)
	{
		// 遍历解析Enum节点
		CXmlElements *pEnumElms = pElement->GetChildElements();
		size_t nElmCount = pEnumElms->GetCount();
		for (size_t i = 0; i < nElmCount; ++i)
		{
			CXmlElement *pEnumElm = pEnumElms->GetAt(i);
			CString strEnumName = pEnumElm->GetAttrValue(L"Name");

			// 检测枚举信息是否存在重复
			EnumTextInfoArray& enumTextInfos = s_enumTextInfoMap[strEnumName];
			if (!enumTextInfos.empty())
			{
				// 后期改为抛出异常
				CString strError;
				strError.Format(L"枚举(%s)信息，存在重复配置！", strEnumName);
				s_strLastError = strError;
				continue;
			}

			// 解析EnumInfo节点
			ParseEnumInfo(pEnumElm, enumTextInfos);
		}

		return true;
	}

	EnumTextInfoArray* GetInfosByEnumName(const CString& strEnumName)
	{
		// 枚举名
		CString strRealName = strEnumName;

		// 先查找指定工程枚举名
		EnumTextInfoMapIter findIter = s_enumTextInfoMap.find(strRealName);
		if (findIter != s_enumTextInfoMap.end())
			return &(findIter->second);

		return NULL;
	}

	CDynEnum::CDynEnum()
	{
	}

	CDynEnum::~CDynEnum()
	{
	}

	CString CDynEnum::Enum2Text(const CString& strEnumName, int nEnumValue)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL == pInfos)
			return L"";

		// 遍历查找枚举字符串
		EnumTextInfoArrayIter endIter = pInfos->end();
		for (EnumTextInfoArrayIter iter = pInfos->begin(); iter != endIter; ++iter)
		{
			if (iter->Value == nEnumValue)
				return iter->Text;
		}

		return L"";
	}

	CString CDynEnum::Enum2Show(const CString& strEnumName, int nEnumValue)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL == pInfos)
			return L"";

		// 遍历查找枚举值
		EnumTextInfoArrayIter endIter = pInfos->end();
		for (EnumTextInfoArrayIter iter = pInfos->begin(); iter != endIter; ++iter)
		{
			if (iter->Value == nEnumValue)
				return iter->Show;
		}

		return L"";
	}

	int CDynEnum::Text2Enum(const CString& strEnumName, const CString& strEnumText)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL == pInfos)
			return 0;

		// 遍历查找枚举字符串
		EnumTextInfoArrayIter endIter = pInfos->end();
		for (EnumTextInfoArrayIter iter = pInfos->begin(); iter != endIter; ++iter)
		{
			if (iter->Text == strEnumText)
				return iter->Value;
		}

		return 0;
	}

	int CDynEnum::Show2Enum(const CString& strEnumName, const CString& strEnumShow)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL == pInfos)
			return 0;

		// 遍历查找枚举显示字符串
		EnumTextInfoArrayIter endIter = pInfos->end();
		for (EnumTextInfoArrayIter iter = pInfos->begin(); iter != endIter; ++iter)
		{
			if (iter->Show == strEnumShow)
				return iter->Value;
		}

		return 0;
	}

	CString CDynEnum::Show2Text(const CString& strEnumName, const CString& strEnumShow)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL == pInfos)
			return L"";

		// 遍历查找枚举显示字符串
		EnumTextInfoArrayIter endIter = pInfos->end();
		for (EnumTextInfoArrayIter iter = pInfos->begin(); iter != endIter; ++iter)
		{
			if (iter->Show == strEnumShow)
				return iter->Text;
		}

		return L"";
	}

	CString CDynEnum::Text2Show(const CString& strEnumName, const CString& strEnumText)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL == pInfos)
			return L"";

		// 遍历查找枚举字符串
		EnumTextInfoArrayIter endIter = pInfos->end();
		for (EnumTextInfoArrayIter iter = pInfos->begin(); iter != endIter; ++iter)
		{
			if (iter->Text == strEnumText)
				return iter->Show;
		}

		return L"";
	}

	EnumTextInfoArray CDynEnum::GetEnumInfoArray(const CString& strEnumName)
	{
		EnumTextInfoArray *pInfos = GetInfosByEnumName(strEnumName);
		if (NULL != pInfos)
			return *pInfos;

		return EnumTextInfoArray();
	}

	bool CDynEnum::LoadEnumConfig(const CString& strXmlFile)
	{
		CXmlDocument xmlDoc;
		if (!xmlDoc.LoadFile(strXmlFile, fmtXML))
		{
			s_strLastError = L"加载枚举配置文件失败！";
			return false;
		}

		CXmlElement* pRootElm = xmlDoc.GetElementRoot();
		if (NULL == pRootElm)
			return false;

		// 解析Enum节点
		if (!ParseEnum(pRootElm))
			return false;

		return true;
	}

	CString CDynEnum::GetLastError()
	{
		return s_strLastError;
	}
}