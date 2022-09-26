//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           PlatformKernel.h
//    @author         JZL
//    @data           2017/11/27 10:13
//    @brief          平台接口头文件
//
//*****************************************************

#pragma once

// 定义导出API
#ifdef COMMBASE_MODULE
#define COMMBASE_API __declspec(dllexport)
#else
#define COMMBASE_API __declspec(dllimport)
#ifdef DEBUG
#pragma comment(lib, "KNodeDlld.lib")
#else
#pragma comment(lib, "KNodeDll.lib")
#endif
#endif

#include "CommBaseDefine.h"

#include "PathConfig/PathConfig.h"

#include "Sdk/AllocMemoryPool.h"
#include "Sdk/AllocMemoryPoolDef.h"
#include "Sdk/CriticalSection.h"
#include "Sdk/DataPtrMapImpl.h"
#include "Sdk/Exception.h"
#include "Sdk/FileDataStream.h"
#include "Sdk/IDataStream.h"
#include "Sdk/MemoryDataStream.h"
#include "Sdk/ObjectFastNew.h"
#include "Sdk/XmdxAccess.h"
#include "Sdk/XmdxDefines.h"
#include "Sdk/XmdxDocument.h"
#include "Sdk/XmdxElement.h"
#include "Sdk/XmlDocument.h"

#include "Tool/Convert.h"
#include "Tool/DynEnum.h"
#include "Tool/EnumTool.h"
#include "Tool/FileTool.h"
#include "Tool/LogOutput.h"
#include "Tool/Md5Tool.h"
#include "Tool/StringTool.h"
#include "Tool/Factory.h"
#include "Tool/FormulaAnalysis.h"
#include "Tool/ResOverride.h"

#include "Type/Any.h"
#include "Type/Assert.h"
#include "Type/Guid.h"
#include "Type/KNode.h"
#include "Type/TypeDefine.h"
