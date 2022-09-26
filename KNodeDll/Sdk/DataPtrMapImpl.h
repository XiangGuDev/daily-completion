////*****************************************************
////    
////    @copyright      变电三维设计组
////    @version        v4.0
////    @file           ObjectFastNew.H
////    @author         fanHong
////    @date           2017/11/3 11:18
////    @brief          本文件是用于提供数据映射的功能，实现合并相同数据的存储及访问，减少
////					  内存消耗。 支持多线程
////
////*****************************************************
//
//#pragma once
//
//#include "CriticalSection.h"
//
//namespace CommBase
//{
//	//! 数据映射功能实现模板类
//	template <class T, class SrcDataImpl, class MapDataImpl = SrcDataImpl>
//	class CDataPtrMapImpl
//	{
//	public:
//		/** 定义数据指针类型 */
//		typedef SrcDataImpl* SrcDataPtr;
//		typedef MapDataImpl* MapDataPtr;
//		typedef CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl> ThisDefines;
//
//		/** 定义只读访问句柄（4个字节） */
//		class ATL_NO_VTABLE RHandle
//		{
//		public:
//			RHandle(void)
//			{
//				_DataVal = NULL;
//			}
//
//			RHandle(MapDataPtr val)
//			{
//				_DataVal = val;
//			}
//
//			// 赋值
//			RHandle& operator=(const RHandle& pszSrc)
//			{
//				_DataVal = pszSrc._DataVal;
//				return *this;
//			}
//
//			// 比较
//			bool operator==(const RHandle& pszSrc) const
//			{
//				return _DataVal == pszSrc._DataVal;
//			}
//
//			__inline const MapDataImpl& operator*() const _THROW0()
//			{
//				return (*get());
//			}
//
//			__inline const MapDataPtr operator->() const _THROW0()
//			{
//				return (get());
//			}
//
//			// 取得值
//			__inline const MapDataPtr get(void) const
//			{
//				return _DataVal;
//			}
//
//		protected:
//			MapDataPtr		_DataVal;
//		};
//
//		/** 定义可写的访问句柄（8个字节） */
//		class ATL_NO_VTABLE RWHandle : public RHandle
//		{
//		public:
//			RWHandle(void)
//			{
//				_DataMap = NULL;
//			}
//
//			RWHandle(ThisDefines* map, MapDataPtr val)
//				: RHandle(val)
//			{
//				_DataMap = map;
//			}
//
//			RWHandle(RWHandle& pszSrc)
//			{
//				operator = (pszSrc);
//			}
//
//			// 赋值
//			RWHandle& operator = (const RWHandle& pszSrc)
//			{
//				_DataVal = pszSrc._DataVal;
//				_DataMap = pszSrc._DataMap;
//				return *this;
//			}
//
//			// 比较
//			bool operator == (const RWHandle& pszSrc)
//			{
//				return _DataVal == pszSrc._DataVal && _DataMap == pszSrc._DataMap;
//			}
//
//			// 设置值
//			__inline void set(SrcDataImpl srcData)
//			{
//				set(&srcData);
//			}
//
//			__inline void set(const SrcDataPtr srcData)
//			{
//				(*(RHandle*)this) = _DataMap->MapData(srcData);
//			}
//
//		protected:
//			ThisDefines*	_DataMap;
//		};
//
//		/**
//		* 比较原始数据与映射数据是否相等，必须重载此函数
//		* @param [in] srcData	原始数据
//		* @param [in] mapData	映射数据
//		* @return 返回原始数据与映射数据是否相等
//		* - true 相等， false 不相等
//		*/
//		bool DataEquap(const SrcDataPtr srcData, const MapDataPtr mapData);
//		/**
//		* 复制原始数据到映射数据，必须重载此函数
//		* @param [in] srcData	原始数据
//		* @param [in] mapData	映射数据
//		*/
//		void CopyToMap(const SrcDataPtr srcData, MapDataPtr mapData);
//		/**
//		* 创建映射数据（可重载）
//		* @param [in] srcData	原始数据
//		* @return 返回映射数据对象
//		*/
//		MapDataPtr AllocMapData(const SrcDataPtr srcData);
//		/**
//		* 释放映射数据（可重载）
//		* @param [in] mapData	映射数据
//		*/
//		void FreeMapData(const SrcDataPtr mapData);
//		/**
//		* 删除所有共享数据
//		*/
//		virtual ~CDataPtrMapImpl(void);
//		/**
//		* 删除所有数据
//		*/
//		void DeleteAll(void);
//		/**
//		* 添加数据，如果数据已存在，则返回已存在的句柄
//		*/
//		__inline RHandle MapData(SrcDataImpl srcData, bool CancelEqual = false)
//		{
//			return MapData(&srcData, CancelEqual);
//		}
//		/**
//		* 添加数据，如果数据已存在，则返回已存在的句柄
//		*/
//		RHandle MapData(const SrcDataPtr srcData, bool CancelEqual = false);
//		/**
//		* 把只读句柄转换成可写句柄
//		* @param [in] handle	只读句柄
//		* @return 返回转换后的可写句柄
//		*/
//		RWHandle R2RWHandle(RHandle handle);
//		/**
//		* 取得数据数量
//		*/
//		size_t GetDataCount(void);
//		/**
//		* 取得数据 MapID
//		* @param [in] handle	只读句柄
//		* @return 返回数据 MapID
//		* - 没找到返回 -1
//		*/
//		int GetHandleId(RHandle handle);
//		/**
//		* 根据 MapID 返回句柄
//		* @param [in] id	数据 MapID
//		* @return 返回数据句柄
//		*/
//		RHandle GetHandleOfId(int id);
//
//	protected:
//		CAtlArray<MapDataPtr>	_MapArray;
//		CCriticalSectionHandle	_DataMutex;
//	};
//
//
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	bool CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::DataEquap(const SrcDataPtr srcData, const MapDataPtr mapData)
//	{
//		// 必须重载此函数
//		//ATLASSERT(FALSE);
//		return false;
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	void CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::CopyToMap(const SrcDataPtr srcData, MapDataPtr mapData)
//	{
//		// 必须重载此函数
//		//ATLASSERT(FALSE);
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	typename CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::MapDataPtr CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::AllocMapData(const SrcDataPtr srcData)
//	{
//		// 默认创建无参的数据
//		return new MapDataImpl;
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	void CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::FreeMapData(const SrcDataPtr mapData)
//	{
//		delete mapData;
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::~CDataPtrMapImpl(void)
//	{
//		DeleteAll();
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	void CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::DeleteAll(void)
//	{
//		T* pT = static_cast<T*>(this);
//
//		for (size_t i = 0; i < _MapArray.GetCount(); i++)
//		{
//			pT->FreeMapData(_MapArray[i]);
//		}
//
//		_MapArray.RemoveAll();
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	typename CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::RHandle CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::MapData(const SrcDataPtr srcData, bool CancelEqual = false)
//	{
//		try
//		{
//			CCriticalSectionSync mutex(_DataMutex);
//
//			T* pT = static_cast<T*>(this);
//
//			if (!CancelEqual)
//			{
//				// 循环当前映射数据，查找是否存在相等的值
//				for (size_t i = 0; i < _MapArray.GetCount(); i++)
//				{
//					if (pT->DataEquap(srcData, _MapArray[i]))
//					{
//						return RHandle(_MapArray[i]);
//					}
//				}
//			}
//
//			// 如果在上面的循环没有找到相同的值，则增加此值
//			MapDataPtr newMap = pT->AllocMapData(srcData);
//			ATLASSERT(newMap);
//
//			// 复制数据并添加到对列
//			pT->CopyToMap(srcData, newMap);
//			_MapArray.Add(newMap);
//
//			return RHandle(newMap);
//		}
//		catch (...)
//		{
//			//::MessageBox (::GetActiveWindow (), _T("严重错误，请关闭工程。"), _T("MapData"), MB_ICONERROR);
//		}
//		return RHandle(NULL);
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	typename CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::RWHandle CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::R2RWHandle(RHandle handle)
//	{
//		T* pT = static_cast<T*>(this);
//		return RWHandle(pT, handle.get());
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	size_t CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::GetDataCount(void)
//	{
//		CCriticalSectionSync mutex(_DataMutex);
//		return _MapArray.GetCount();
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	int CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::GetHandleId(RHandle handle)
//	{
//		CCriticalSectionSync mutex(_DataMutex);
//
//		T* pT = static_cast<T*>(this);
//
//		// 循环当前映射数据，查找是否存在相等的值
//		for (size_t i = 0; i < _MapArray.GetCount(); i++)
//		{
//			if (pT->DataEquap(handle.get(), _MapArray[i]))
//			{
//				return (int)i;
//			}
//		}
//
//		return -1;
//	}
//
//	template <class T, class SrcDataImpl, class MapDataImpl>
//	typename CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::RHandle CDataPtrMapImpl<T, SrcDataImpl, MapDataImpl>::GetHandleOfId(int id)
//	{
//		CCriticalSectionSync mutex(_DataMutex);
//
//		ATLASSERT(id >= 0 && id < (int)_MapArray.GetCount());
//		if (id < 0 || id >= (int)_MapArray.GetCount())
//			return RHandle();
//
//		return RHandle(_MapArray[id]);
//	}
//
//	//! 字符串影射管理 */
//	class COMMBASE_API CStringShareControl : public CDataPtrMapImpl <CStringShareControl, CString>
//	{
//	public:
//		/**
//		* 比较原始数据与影射数据是否相等
//		* @param [in] srcData	原始数据
//		* @param [in] mapData	映射数据
//		* @return 返回原始数据与映射数据是否相等
//		* - true 相等， false 不相等
//		*/
//		bool DataEquap(const SrcDataPtr srcData, const MapDataPtr mapData);
//
//		/**
//		* 复制原始数据到映射数据
//		* @param [in] srcData	原始数据
//		* @param [in] mapData	映射数据
//		*/
//		void CopyToMap(const SrcDataPtr srcData, MapDataPtr mapData);
//	};
//
//	/** 定义CShareString类型 */
//	typedef CStringShareControl::RWHandle CShareString;
//	/** 定义CShareStringR类型 */
//	typedef CStringShareControl::RHandle CShareStringR;
//
//	/** 定义工程量字符串值类型转换 */
//	ULONGLONG ShareString2ULong(CShareString ws);
//	CShareString ULong2ShareString(ULONGLONG ul);
//	ULONG ShareString2ULong(CShareStringR ws);
//	CShareStringR ULong2ShareString(ULONG ul);
//}