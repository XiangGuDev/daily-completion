//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           AllocMemoryPool.H
//    @author         fanHong
//    @date           2017/11/3 11:16
//    @brief          内存池
//
//*****************************************************

#pragma once

#include "CriticalSection.h"
#include "AllocMemoryPoolDef.h"
#include <atlcoll.h>

namespace CommBase
{
	//! 内存池分配对象
	class COMMBASE_API CAllocMemoryPool : public IAllocMemoryPool
	{
	public:
		CAllocMemoryPool(size_t ChunkSizeRadix = 32);

		virtual ~CAllocMemoryPool(void);

		/**
		* 分配指定长度的内存
		* nGrowBy会自动向上取最小的平方数(开平方得整数的数)，不能超过 MAXBYTE
		* 将会分配 至少小 size * nGrowBy 的内存
		* @param [in] size				每块内存长度
		* @param [in] nGrowBy			要分配的内存块数
		* @param [in] CalcBestBlockSize	默认为TRUE，对齐
		*/
		virtual LPVOID Alloc(size_t size, size_t nGrowBy, BOOL CalcBestBlockSize = TRUE);

		/**
		* 释放内存块
		* @param [in] buffer			内存缓存
		* @param [in] FreeEmptyMemory	默认为FALSE
		*/
		virtual void Free(LPVOID buffer, BOOL FreeEmptyMemory = FALSE);

		/** 释放所有　*/
		virtual void FreeAll(void);

		/** 清楚所有 */
		virtual void ClearUp(void);

		/** 统计总共分配的字节数　*/
		virtual ULONGLONG TotalAllocBytes(void);

	protected:
#pragma pack(push, 1)
		/** 定义结构 */
		typedef struct tagMEMORYHEAD
		{
			/** 内存块总数量 */
			size_t ChunkCount;
			/** 内存块分配长度（不包括结点头CHUNKHEAD的长度） */
			size_t ChunkSize;
			/** 没有分配出去的内存块数量 */
			size_t NotAllocCount;
			/** 内存块分组数量 */
			size_t GroupCount;
			/** 每组的内存块数量 */
			size_t GroupLength;
			/** 最后一次分配／释放的组索引 */
			size_t LastOpGroup;
			/** 最后一次分配／释放的内存块索引 */
			size_t LastOpChunk;
		} MEMORYHEAD, *PMEMORYHEAD;

		/** 定义内存结点头部信息( 8 byte ) */
		typedef struct tagCHUNKHEAD
		{
			/** 是否已分配出去 */
			UCHAR		bAlloc;
			/** 所在的组索引 */
			UCHAR		nGroupIndex;
			/** 组内的内存块索引 */
			UCHAR		nChunkIndex;
			/** 当前组没有分配的内存块数量 */
			UCHAR		nGroupNotAlloc;
			/** 内存数组头 */
			MEMORYHEAD* pMemory;
		} CHUNKHEAD, *PCHUNKHEAD;
#pragma pack(pop)

		/*
		内存分配结构图
		+--------------------------------------------------------------------------------------+
		|    数组长度        |   内存块 1                     |    内存块 2                    |
		+--------------------------------------------------------------------------------------+
		| sizeof(MEMORYHEAD) | sizeof(CHUNKHEAD) + ChunkSize | sizeof(CHUNKHEAD) + ChunkSize |
		+--------------------------------------------------------------------------------------+
		*/
		/** 内存列表数组 */
		CAtlArray<PMEMORYHEAD>	_MemoryArray;
		/** 保存没分配的内存块的内存列表 */
		CAtlArray<PMEMORYHEAD>	_CanAllocMemory;

		/** 默认最小内存块大小 */
		size_t					_ChunkSizeRadix;

		/** 共分配的次数 */
		ULONGLONG				_AllocCount;
		/** 线程互斥器 */
		CCriticalSectionHandle	_CriticalMutex;

	protected:

		/**
		* 释放指定索引的内存块
		* @param [out] index	内存块索引
		*/
		void FreeMemoryAt(const size_t& index);
		/**
		* 引用内存块，返回内存块地址
		* @param [in] chunk	内存结构对象
		*/
		LPVOID AllocChunk(PCHUNKHEAD chunk);
		/**
		* 释放内存块
		* @param [in] chunk	内存结构对象
		*/
		void FreeChunk(PCHUNKHEAD chunk);
		/**
		* 增加指定长度的内存块
		* @param [out] RequestedSize		每一块的需求大小
		* @param [out] nGrowBy				内存块数
		* @param [out] CalcBestBlockSize
		*/
		void ExtandMemoryChunk(const size_t& RequestedSize, const size_t& nGrowBy, const BOOL& CalcBestBlockSize);
		/**
		* 取得第N组的组结点
		* @param [in] memory		内存结构对象
		* @param [in] group			组数
		* @return 返回租结点
		*/
		__inline PCHUNKHEAD GetGroupChunkHead(PMEMORYHEAD memory, size_t group);
		/**
		* 取得第N个内存块结点
		* @param [in] memory		内存结构对象
		* @param [in] GroupIndex	组的索引
		* @param [in] ChunkIndex	内存块索引
		* @return 返回租结点
		*/
		__inline PCHUNKHEAD GetMemoryChunkHead(PMEMORYHEAD memory, size_t GroupIndex, size_t ChunkIndex);
		/**
		* 查找可用的内存块
		* @param [out] chunk			内存块结构
		* @param [out] RequestedSize	需求大小
		* @return 返回查找可用的内存块是否成功
		* - TRUE 成功， FALSE 失败
		*/
		BOOL LookupMemoryChunk(PCHUNKHEAD& chunk, const size_t& RequestedSize);
		/**
		* 查找容量最适合并且存在可用内存块的内内存数组索引
		* @param [out] RequestedSize	需求大小
		* @param [out] bestMemory		最适合内存块结构
		* @return 返回查找容量最适合并且存在可用内存块的内内存数组索引是否成功
		* - TRUE 成功， FALSE 失败
		*/
		BOOL FindBestMemoryHead(const size_t& RequestedSize, PMEMORYHEAD& bestMemory);
		/**
		* 查找可用内存块
		* @param [in] memory	内存块结构
		* @param [out] index	可用内存块索引
		* @return 返回查找可用内存块是否成功
		* - TRUE 成功， FALSE 失败
		*/
		BOOL FindCanAllocMemory(const PMEMORYHEAD memory, int& index);
		/**
		* 计算最佳的内存块长度
		* @param [out] sRequestedMemoryBlockSize	需求内存块长度
		* @return 返回最佳的内存块长度
		*/
		size_t CalculateBestMemoryBlockSize(const size_t &sRequestedMemoryBlockSize);
		/**
		* 计算需要的内存块数量
		* @param [out] sMemorySize	内存块长度
		* @return 返回需要的内存块数量
		*/
		size_t CalculateNeededChunks(const size_t &sMemorySize);
		/**
		* 计算最佳的块数量
		* @param [out] GrowBy		内存总块数
		* @param [out] GroupCount	内存块的组数
		* @return 返回组内最佳的块数量
		*/
		size_t CalculateBestMemoryChunkCount(const size_t& GrowBy, size_t& GroupCount);
	};
}