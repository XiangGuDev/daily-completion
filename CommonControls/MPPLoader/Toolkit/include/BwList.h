//////////////////////////////////////////////////////////////////////
// 文件名称：List.h
// 功能描述：通用顺序列表类，继承自CSimpleArray
// 创建标识：zengjiangyou	2018.05.17
// 修改标识：
// 修改描述：
// 文件版权：江西博微新技术有限公司
//////////////////////////////////////////////////////////////////////
#pragma once

class CStringElementTraitsEx :public CElementTraits<CString>
{
public:
	static ULONG Hash(_In_ const CString& element) throw()
	{
		ULONG nHash = 0;
		LPCTSTR pch = element;
		while (*pch != 0)
		{
			nHash = (nHash << 5) + nHash + (*pch);
			pch++;
		}

		return(nHash);
	}
};

namespace toolkit
{
	namespace data
	{
		/** @addtogroup Toolkit
		* @{
		*/
		//! 通用顺序列表模板类
		template<class T>
		class CBwList : public CSimpleArray<T>
		{
		public:
			/**
			* 比较函数指针回调函数
			* @param [in] src CBwList列表
			*/
			typedef bool (*COMPARE)(T pNodeA, T pNodeB, PVOID field);

			CBwList(){}

			/**
			* 拷贝构造数
			* @param [in] src CBwList列表
			*/
			CBwList(const CBwList<T> &src):CSimpleArray(src){}

			/**
			* 拷贝构造数(兼容CSimpleArray)
			* @param [in] src CSimpleArray列表
			*/
			CBwList(const CSimpleArray<T>& src):CSimpleArray(src){}

			/**
			* 拷贝构造数(兼容CAtlArray)
			* @param [in] src CAtlArray列表
			*/
			CBwList(const CAtlArray<T>& src)
			{
				Changelist(src);
			}

			/**
			* 赋值函数
			* @param [in] src 参数可以是BwList，也可以是CSimpleArray
			*/
			CBwList<T>& operator=(const CSimpleArray<T>& src)
			{
				__super::operator = (src);
				return *this;
			}

			/**
			* 赋值函数（兼容CAtlArray）
			* @param [in] src CAtlArray列表
			*/
			CBwList<T>& operator=(const CAtlArray<T>& src)
			{
				Changelist(src);
				return *this;
			}

			/**
			* 获取数量
			* @return 数量
			*/
			inline int GetCount(void) const
			{
				return this->GetSize();
			}

			/**
			* 获取指定索引的项目内容
			* @param [in] index 索引
			* @return 项目内容(不支持修改)
			*/
			inline const T& GetAt(int index) const
			{
				return __super::operator [](index);
			}

			/**
			* 添加列表
			* @param [in] list	列表
			*/
			void AddList(const CBwList<T> &list)
			{
				int intCount = list.GetCount();
				for (int i = 0; i < intCount; i++)
				{
					this->Add(list.GetAt(i));
				}
			}

			/**
			* 在指定的索引下插入一项值
			* @param [in] nIndex 索引
			* @param [in] t	值
			* @return 
			*/
			BOOL InsertAt( int nIndex, const T& t )
			{
				if ( nIndex < 0 )
					return FALSE;

				if ( nIndex >= m_nSize )
					return ( Add( t ) != -1 );

				if ( m_nSize == m_nAllocSize )
				{
					int nNewAllocSize = ( m_nAllocSize == 0 ) ? 1 : ( m_nSize * 2 );
					T* aT = static_cast<T*>(realloc( m_aT, nNewAllocSize * sizeof( T ) ));
					if ( aT == NULL )
						return FALSE;
					m_nAllocSize = nNewAllocSize;
					m_aT = aT;
				}
				MoveMemory( (LPVOID)( m_aT + nIndex + 1 ), (LPVOID)( m_aT + nIndex ), ( m_nSize - nIndex ) * sizeof( T ) );
				InternalSetAtIndex( nIndex, t );
				m_nSize++;
				return TRUE;
			}

			/**
			* 删除列表中指针（当T不是指针类型时，此方法的功能等同于RemoveAll）
			*/
			void DeleteAll()
			{
				int count = GetCount();
				if (count == 0)
					return;

				//获取T的类型,如果最后一个字符为*，则T为指针(例如：int *)
				CString TType = typeid(T).name();				
				if (TType.Right(1) == L"*")
				{
					for (int i = 0; i < count; i++)
					{
						T t = GetAt(i);
						delete t ;
						t = NULL;
					}
				}				
				RemoveAll();
			}			


			/**
			* 排序函数
			* @param [in] compareFun 排序比较函数
			* @param [in] field 扩展参数
			*/
			void Sort(COMPARE compareFun = DefaultLessCompare, PVOID field = NULL)
			{
				if (GetSize() > 0)
				{
					SortOfMid(compareFun, 0, GetSize() - 1, field);
				}
			}

		private:

			/**
			* 默认小于比较
			* @param [in] left		比较左边值
			* @param [in] right	比较右边值
			* @param [in] field	扩展字段
			* @return 是否小于
			*/
			static bool DefaultLessCompare(T left, T right, PVOID field = NULL) 
			{
				return left < right;
			}

			/**
			* 排序
			* @param [in] compareFun	比较函数
			* @param [in] left	        比较左边值
			* @param [in] right	    比较右边值
			*/
			void SortOfMid(COMPARE compareFun, int left, int right, PVOID field)
			{
				int l_hold = left;
				int r_hold = right;

				T pivot = __super::operator [](left);

				while (left < right)
				{
					T rightvalue = __super::operator [](right);
					T leftvalue = __super::operator [](left);

					while (!compareFun(rightvalue, pivot, field) && (left < right))
					{
						right --;
						rightvalue = __super::operator [](right);
					}

					if (left != right)
					{
						__super::operator [](left) = __super::operator [](right);
						left ++;
						leftvalue = __super::operator [](left);
					}

					while (compareFun(leftvalue, pivot, field) && (left < right))
					{
						left ++;
						leftvalue = __super::operator [](left);
					}

					if (left != right)
					{
						__super::operator [](right) = __super::operator [](left);
						right--;
						rightvalue = __super::operator [](right);
					}
				}

				__super::operator [](left) = pivot;

				int cur = left;
				left = l_hold;
				right = r_hold;

				if (left < cur)
					SortOfMid (compareFun, left, cur - 1, field);
				if (right > cur)
					SortOfMid (compareFun, cur + 1, right, field);
			}

			/**
			* 列表转换
			* @param [in] src CAtlArray列表
			*/
			void Changelist(const CAtlArray<T> &src)
			{
				this->RemoveAll();
				for (size_t i = 0; i < src.GetCount(); i++)
				{
					this->Add(src.GetAt(i));
				}
			}
		};/** * @} */
	}
}