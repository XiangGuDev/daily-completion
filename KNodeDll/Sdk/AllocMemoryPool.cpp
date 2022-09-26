#include "stdafx.h"
#include "AllocMemoryPool.h"
#include "CriticalSection.h"
#include <malloc.h>
#include <math.h>

namespace CommBase
{
	CAllocMemoryPool::CAllocMemoryPool(size_t ChunkSizeRadix)
	{
		_ChunkSizeRadix = ChunkSizeRadix;
		_AllocCount = 0;
	}

	CAllocMemoryPool::~CAllocMemoryPool(void)
	{
		ATLASSERT(_AllocCount == 0);

		FreeAll();
	}

	/** 分配指定长度的内存　*/
	LPVOID CAllocMemoryPool::Alloc(size_t size, size_t nGrowBy, BOOL CalcBestBlockSize)
	{
		ATLASSERT(nGrowBy > 0);

		if (size <= 0) return NULL;

		CCriticalSectionSync mutex(_CriticalMutex);

		// 查找可用内存块，如果没找到则扩充
		PCHUNKHEAD chunk = NULL;
		if (!LookupMemoryChunk(chunk, size))
		{
			ExtandMemoryChunk(size, nGrowBy, CalcBestBlockSize);

			// 如果扩充容量后还是没有找到可用内存，则返回NULL
			if (!LookupMemoryChunk(chunk, size))
				return NULL;
		}

		ATLASSERT(chunk->pMemory != NULL);

		return AllocChunk(chunk);
	}

	/** 释放内存块　*/
	void CAllocMemoryPool::Free(LPVOID buffer, BOOL FreeEmptyMemory)
	{
		if (buffer == NULL) return;

		CCriticalSectionSync mutex(_CriticalMutex);

		// 释放内存块
		PCHUNKHEAD chunk = (PCHUNKHEAD)(((char*)buffer) - sizeof(CHUNKHEAD));

		ATLASSERT(chunk != NULL && chunk->pMemory != NULL);

		FreeChunk(chunk);

		// 释放内存
		if (FreeEmptyMemory && chunk->pMemory->NotAllocCount == 0)
		{
			for (size_t i = 0; i < _MemoryArray.GetCount(); i++)
			{
				if (_MemoryArray[i] == chunk->pMemory)
				{
					FreeMemoryAt(i);
					break;
				}
			}
		}
	}

	/** 释放所有　*/
	void CAllocMemoryPool::FreeAll(void)
	{
		CCriticalSectionSync mutex(_CriticalMutex);

		while (_MemoryArray.GetCount())
			FreeMemoryAt(_MemoryArray.GetCount() - 1);
	}

	void CAllocMemoryPool::ClearUp(void)
	{
		CCriticalSectionSync mutex(_CriticalMutex);

		for (int i = (int)_MemoryArray.GetCount() - 1; i >= 0; i--)
		{
			PMEMORYHEAD memory = _MemoryArray[i];
			if (memory->NotAllocCount == memory->ChunkCount)
				FreeMemoryAt(i);
		}
	}

	/** 统计总共分配的字节数　*/
	ULONGLONG CAllocMemoryPool::TotalAllocBytes(void)
	{
		CCriticalSectionSync mutex(_CriticalMutex);

		ULONGLONG count = 0;

		for (int i = (int)_MemoryArray.GetCount() - 1; i >= 0; i--)
		{
			PMEMORYHEAD memory = _MemoryArray[i];
			count += sizeof(MEMORYHEAD) + memory->ChunkCount *
				(sizeof(CHUNKHEAD) + memory->ChunkSize);
		}

		return count;
	}

	// 释放指定索引的内存块
	void CAllocMemoryPool::FreeMemoryAt(const size_t& index)
	{
		ATLASSERT(index >= 0 && index < _MemoryArray.GetCount());

		// 从可用列表中移出
		int CanAllocIndex = -1;
		if (FindCanAllocMemory(_MemoryArray[index], CanAllocIndex))
			_CanAllocMemory.RemoveAt(CanAllocIndex);

		// 释放内存
		char* buffer = (char*)_MemoryArray[index];
		delete[] buffer;
		//free (_MemoryArray[index]);
		_MemoryArray.RemoveAt(index);
	}

	// 引用内存块，返回内存块地址
	LPVOID CAllocMemoryPool::AllocChunk(PCHUNKHEAD chunk)
	{
		_AllocCount++;

		// 返回内存地址
		chunk->bAlloc = TRUE;
		chunk->pMemory->NotAllocCount--;

		// 计算内存块所在段，并设置段值
		PCHUNKHEAD group = GetGroupChunkHead(chunk->pMemory, chunk->nGroupIndex);
		group->nGroupNotAlloc--;

		chunk->pMemory->LastOpGroup = chunk->nGroupIndex;
		chunk->pMemory->LastOpChunk = chunk->nChunkIndex;

		// 移出可用内存块数组
		if (chunk->pMemory->NotAllocCount == 0)
		{
			int CanAllocIndex = -1;
			if (FindCanAllocMemory(chunk->pMemory, CanAllocIndex))
				_CanAllocMemory.RemoveAt(CanAllocIndex);
		}

		// 返回可写内存地址
		return (void*)(((char*)chunk) + sizeof(CHUNKHEAD));
	}

	// 释放内存块
	void CAllocMemoryPool::FreeChunk(PCHUNKHEAD chunk)
	{
		_AllocCount--;

		// 设置内存块
		chunk->bAlloc = FALSE;
		chunk->pMemory->NotAllocCount++;

		// 计算内存块所在段，并设置段值
		PCHUNKHEAD group = GetGroupChunkHead(chunk->pMemory, chunk->nGroupIndex);
		group->nGroupNotAlloc++;

		chunk->pMemory->LastOpGroup = chunk->nGroupIndex;
		chunk->pMemory->LastOpChunk = chunk->nChunkIndex;

		// 移入可用内存块数组
		if (chunk->pMemory->NotAllocCount == 1)
		{
			int CanAllocIndex = -1;
			if (!FindCanAllocMemory(chunk->pMemory, CanAllocIndex))
				_CanAllocMemory.Add(chunk->pMemory);
		}
	}

	// 增加指定长度的内存块
	void CAllocMemoryPool::ExtandMemoryChunk(const size_t& RequestedSize, const size_t& nGrowBy,
		const BOOL& CalcBestBlockSize)
	{
		// 计算要分配的内存块长度、内存块数量
		size_t bestChunkSize = CalcBestBlockSize ? CalculateBestMemoryBlockSize(RequestedSize) : RequestedSize;

		size_t GroupCount = 0;
		size_t bestChunkCount = CalculateBestMemoryChunkCount(nGrowBy, GroupCount);

		// 分配的组数不能大于 256
		ATLASSERT(GroupCount <= MAXBYTE);

		size_t memSize = sizeof(MEMORYHEAD) + (sizeof(CHUNKHEAD) + bestChunkSize) * bestChunkCount;

		// 分配内存，并填写头结构数据
		PMEMORYHEAD memory = (PMEMORYHEAD) new char[memSize];
		ATLASSERT(memory != NULL);

		memory->ChunkCount = bestChunkCount;
		memory->NotAllocCount = bestChunkCount;
		memory->ChunkSize = bestChunkSize;
		memory->GroupCount = GroupCount;
		memory->GroupLength = GroupCount;
		memory->LastOpGroup = 0;
		memory->LastOpChunk = 0;

		// 初使化每个结点的内容
		for (size_t i = 0; i < memory->GroupCount; i++)
		{
			for (size_t j = 0; j < memory->GroupLength; j++)
			{
				PCHUNKHEAD chunk = GetMemoryChunkHead(memory, i, j);

				if (j == 0)
				{
					chunk->nGroupNotAlloc = (UCHAR)memory->GroupLength;
				}

				chunk->pMemory = memory;
				chunk->nGroupIndex = (UCHAR)i;
				chunk->nChunkIndex = (UCHAR)j;
				chunk->bAlloc = FALSE;
			}
		}

		// 增加到数组列表
		_MemoryArray.Add(memory);

		// 添加到可分配的内存列表中
		_CanAllocMemory.Add(memory);
	}

	// 取得第N组的组结点
	CAllocMemoryPool::PCHUNKHEAD CAllocMemoryPool::GetGroupChunkHead(
		CAllocMemoryPool::PMEMORYHEAD memory, size_t group)
	{
		return GetMemoryChunkHead(memory, group, 0);
	}

	// 取得第N个内存块结点
	CAllocMemoryPool::PCHUNKHEAD CAllocMemoryPool::GetMemoryChunkHead(
		CAllocMemoryPool::PMEMORYHEAD memory, size_t GroupIndex, size_t ChunkIndex)
	{
		// 计算内存块所在索引
		size_t index = GroupIndex * memory->GroupLength + ChunkIndex;

		// 返回内存块所在地址
		return (PCHUNKHEAD)(((char*)memory) + sizeof(MEMORYHEAD) +
			index * (sizeof(CHUNKHEAD) + memory->ChunkSize));
	}

	// 查找可用的内存块
	BOOL CAllocMemoryPool::LookupMemoryChunk(CAllocMemoryPool::PCHUNKHEAD& chunk,
		const size_t& RequestedSize)
	{
		// 查找可用内存头
		PMEMORYHEAD bestMemory = NULL;
		if (!FindBestMemoryHead(RequestedSize, bestMemory))
			return FALSE;

		// 先查找组（一定找得到）
		size_t GroupIndex = bestMemory->LastOpGroup;
		if (GetGroupChunkHead(bestMemory, GroupIndex)->nGroupNotAlloc == 0)
		{
			for (size_t i = 0; i < bestMemory->GroupCount; i++)
			{
				PCHUNKHEAD group = GetGroupChunkHead(bestMemory, i);
				if (group->nGroupNotAlloc)
				{
					GroupIndex = i;
					break;
				}
			}
		}

		// 查找可用的内存块

		// 从 LastOpChunk 开始向后查找
		for (size_t i = bestMemory->LastOpChunk; i < bestMemory->GroupLength; i++)
		{
			chunk = GetMemoryChunkHead(bestMemory, GroupIndex, i);
			if (!chunk->bAlloc)
				return TRUE;
		}

		// 从 0 开始向 LastOpChunk 查找
		for (size_t i = 0; i < bestMemory->LastOpChunk; i++)
		{
			chunk = GetMemoryChunkHead(bestMemory, GroupIndex, i);
			if (!chunk->bAlloc)
				return TRUE;
		}

		return TRUE;
	}

	// 查找容量最适合并且存在可用内存块的内内存数组索引
	BOOL CAllocMemoryPool::FindBestMemoryHead(const size_t& RequestedSize, PMEMORYHEAD& bestMemory)
	{
		bestMemory = NULL;

		// 从可用内存块中查找
		for (size_t i = 0; i < _CanAllocMemory.GetCount(); i++)
		{
			PMEMORYHEAD memory = _CanAllocMemory[i];

			if (memory->NotAllocCount && memory->ChunkSize >= RequestedSize)
			{
				if (bestMemory == NULL)
					bestMemory = memory;
				else if (memory->ChunkSize < bestMemory->ChunkSize)
					bestMemory = memory;
			}
		}

		return bestMemory != NULL;
	}

	// 查找可用内存块
	BOOL CAllocMemoryPool::FindCanAllocMemory(const PMEMORYHEAD memory, int& index)
	{
		for (int i = 0; i < (int)_CanAllocMemory.GetCount(); i++)
		{
			if (_CanAllocMemory[i] == memory)
			{
				index = i;
				return TRUE;
			}
		}

		return FALSE;
	}

	// 计算最佳的内存块长度
	size_t CAllocMemoryPool::CalculateBestMemoryBlockSize(const size_t &sRequestedMemoryBlockSize)
	{
		size_t uiNeededChunks = CalculateNeededChunks(sRequestedMemoryBlockSize);
		return size_t((uiNeededChunks * _ChunkSizeRadix));
	}

	size_t CAllocMemoryPool::CalculateNeededChunks(const size_t &sMemorySize)
	{
		float f = (float)(((float)sMemorySize) / ((float)_ChunkSizeRadix));
		return ((size_t)ceil(f));
	}

	// 计算最佳的块数量
	size_t CAllocMemoryPool::CalculateBestMemoryChunkCount(const size_t& GrowBy, size_t& GroupCount)
	{
		GroupCount = (size_t)ceil(sqrtl((long)GrowBy));
		return (size_t)powl((long)GroupCount, 2);
	}
}