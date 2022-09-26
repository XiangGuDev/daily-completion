//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           CommonTools.H
//    @author         KG
//    @date           2014/12/17 17:26
//    @brief          公共库头文件
//*****************************************************

#pragma once

#define LoadEnumToCombo(EnumName, combo)\
{\
	EnumTextInfoArray arr = Get##EnumName##InfoArray();\
	for (EnumTextInfoArrayIter iter = arr.begin(); iter != arr.end(); ++iter)\
		combo.AddString(iter->Text);\
}
	
#define LoadEnumShowToCombo(EnumName, combo)\
{\
	EnumTextInfoArray arr = Get##EnumName##InfoArray();\
	for (EnumTextInfoArrayIter iter = arr.begin(); iter != arr.end(); ++iter)\
		combo.AddString(iter->Show);\
}
