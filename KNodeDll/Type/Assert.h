//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           Assert.H
//    @author         fanHong
//    @date           2017/11/2 16:58
//    @brief          Release版本断言宏
//
//*****************************************************

#pragma once

#include <crtdefs.h>
#include "Tool/LogOutput.h"

#undef  KASSERT

#ifdef  __cplusplus
extern "C" {
#endif

	_CRTIMP void __cdecl _wassert(const wchar_t * _Message, const wchar_t *_File, unsigned _Line);

#ifdef  __cplusplus
}
#endif

#ifdef _DEBUG
#define KASSERT(_Expression, ...) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression###__VA_ARGS__), _CRT_WIDE(__FILE__), __LINE__), 0) )
#else
#define KASSERT(_Expression, ...) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression###__VA_ARGS__), _CRT_WIDE(__FILE__), __LINE__), 0) ); if (!(_Expression)) {CLog::LogErrorV(L"断言：%s %s - %d", _CRT_WIDE(#_Expression###__VA_ARGS__), _CRT_WIDE(__FILE__), __LINE__);}
#endif
