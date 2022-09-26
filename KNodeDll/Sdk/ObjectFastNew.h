//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           ObjectFastNew.H
//    @author         fanHong
//    @date           2017/11/3 11:18
//    @brief          临时对象管理
//
//*****************************************************

#pragma once
#include <atlcoll.h>

namespace CommBase
{
	/*
	*	临时对象管理，用于经常快速创建、释放的对象管理，不支持多线程
	*/
	template<class T>
	class CObjectFastNew
	{
	public:
		CObjectFastNew(size_t bufferObjectCount = 32)
		{
			_bufferCount = bufferObjectCount;
			_recycleList.SetCount(0, int(_bufferCount));
		}

		~CObjectFastNew()
		{
			clear();
		}

		void clear()
		{
			for (size_t i = 0; i < _recycleList.GetCount(); i++)
				delete _recycleList[i];
			_recycleList.RemoveAll();
		}

		// 获取一个新对象，这个对象可能是以前用过的
		T* getnew()
		{
			// 生成空的对空进去
			if (!_recycleList.GetCount())
			{
				for (size_t i = 0; i < _bufferCount; i++)
				{
					_recycleList.Add(new T());
				}
			}

			// 取出最后一个
			int getIndex = int(_recycleList.GetCount() - 1);
			T* newobj = _recycleList[getIndex];
			_recycleList.RemoveAt(getIndex);
			return newobj;
		}

		// 回收对象
		void recycle(T* object)
		{
			if (_recycleList.GetCount() >= _bufferCount)
				delete object;
			else
				_recycleList.Add(object);
		}

	protected:
		CAtlArray<T*>	_recycleList;
		size_t			_bufferCount;
	};
}