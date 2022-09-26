#pragma once

//-------------------------------------------------------------------
// ʵ��ö��ֵ���ַ�����ת��
//-------------------------------------------------------------------


/****************************************************************************************
*								** ʹ������ **
*
* 1��������ö��ֵ��
*
*	enum FileCreateTypeEnum
*	{
*		fctStdProject = 1,
*		fctUserProject,
*		fctGroup2wareLib,
*	};
*
*	DECLARE_ENUMTYPENAME_CONVERSION2 (FileCreateTypeEnum);
*
* 2����ʵ��ö�����ַ�����ʹ�ã�
*
BEGIN_ENUMTYPENAME_CONVERSION2 (FileCreateTypeEnum)
{
{ fctStdProject, _T("��׼����") },
{ (FileCreateTypeEnum) NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2 (FileCreateTypeEnum);
*
* 3����ʹ��ת��������
*
*	// ö��ֵת�ַ���
*	CString EnumText = FileCreateTypeEnum2Text (fctStdProject);
*
*	// �ַ���תö��ֵ
*	FileCreateTypeEnum EnumValue = Text2FileCreateTypeEnum (_T("��׼����"));
*
*	// ȡ��ö��ֵ����
*	CEnumTextDefines2 EnumNameList;
*	GetFileCreateTypeEnumDefines (EnumNameList);
*
****************************************************************************************/


// ����ö��ֵ���ַ�����ת����������
//   ע:������ַ���ת����ö��ʧ��,�򷵻�0,���򷵻ؾ����ö��ֵ
//		�����ö��ת�����ַ���ʧ��,�򷵻�NULL,���򷵻ؾ��������
//
// ����: ö������ ProjectTypeEnum������������������
//
//		1. LPCTSTR ProjectTypeEnum2Text (ProjectTypeEnum val);
//		2. ProjectTypeEnum Text2ProjectTypeEnum (LPCTSTR txt);
//		3. int GetProjectTypeEnumDefines (CEnumTextDefines2& arr);

struct ENUMTYPETEXT2
{
	int value;
	const wchar_t* text;
	DWORD data; 
};
typedef CSimpleArray<ENUMTYPETEXT2> CEnumTextDefines2;

#define DECLARE_ENUMTYPENAME_CONVERSION2(ENUMNAME) \
	LPCTSTR ENUMNAME##2Text (ENUMNAME val); \
	DWORD ENUMNAME##2Data (ENUMNAME val); \
	ENUMNAME Text2##ENUMNAME(LPCTSTR txt, int def = 0); \
	int Get##ENUMNAME##Defines (CEnumTextDefines2& arr);

/*
���嶨��ö��ֵֵ���ַ�����ת������
*/
#define BEGIN_ENUMTYPENAME_CONVERSION2(ENUMNAME) \
	ENUMTYPETEXT2 ENUMNAME##TextList[] =

#define END_ENUMTYPENAME_CONVERSION2(ENUMNAME) \
	;\
	LPCTSTR ENUMNAME##2Text (ENUMNAME val)\
	{\
		for (int i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i ++)\
		{\
			if (ENUMNAME##TextList[i].value == val)\
			return ENUMNAME##TextList[i].text;\
		}\
		return NULL;\
	}\
	DWORD ENUMNAME##2Data (ENUMNAME val)\
	{\
		for (int i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i++)\
		{\
		if (ENUMNAME##TextList[i].value == val)\
			return ENUMNAME##TextList[i].data; \
		}\
		return NULL; \
	}\
	ENUMNAME Text2##ENUMNAME (LPCTSTR txt,int def)\
{\
	if (txt == NULL) return (ENUMNAME) def;\
	int i = 0;\
	for (i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i ++)\
{\
	if (!_tcscmp (ENUMNAME##TextList[i].text, txt))\
	return (ENUMNAME) ENUMNAME##TextList[i].value;\
}\
	return (ENUMNAME) ENUMNAME##TextList[i].value;\
}\
	int Get##ENUMNAME##Defines (CEnumTextDefines2& arr)\
{\
	arr.RemoveAll ();\
	for (int i = 0; ENUMNAME##TextList[i].text != NULL && i < 1000; i ++)\
{\
	arr.Add (ENUMNAME##TextList[i]);\
}\
	return arr.GetSize ();\
}