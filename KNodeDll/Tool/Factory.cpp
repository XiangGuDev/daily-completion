#include "StdAfx.h"
#include "Factory.h"

namespace CommBase
{
	/**
	*  @class     Object创建工厂实现
	*
	*  @brief
	*/
	class KObjectFactoryImpl : public KObjectFactory
	{
		/*
		*  Object类创建接口AMP
		*/
		typedef std::map<CString, KObjectCreator*> ObjectCreaterMap;
		typedef ObjectCreaterMap::iterator ObjectCreaterMapIter;

	public:
		/**
		*  @brief    从工厂中获取Object对象(需要自行释放指针)
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   KObject* 创建失败返回NULL
		*/
		virtual KObject* NewObject(LPCWSTR lpName) override
		{
			return FindObject(lpName);
		}

		/**
		*  @brief    从工厂中获取Service对象(单例)
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   KObject* 创建失败返回NULL
		*/
		virtual KObject* GetService(LPCWSTR lpName) override
		{
			return FindObject(lpName);
		}

		/**
		*  @brief    添加Object创建
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @param    IKObjectCreator * pObjCreater 创建类的对象
		*  @param    bool bCoverExist 是否覆盖已注册的
		*  @return   bool 成功返回true
		*/
		virtual bool AddFactory(LPCWSTR lpName, KObjectCreator* pObjCreater,
			bool bCoverExist = false) override
		{
			if (NULL == lpName || (NULL == pObjCreater))
				return false;

			ObjectCreaterMapIter findIter = _objectCreatorMap.find(lpName);
			if (findIter != _objectCreatorMap.end())
			{
				// 不覆盖则返回
				if (!bCoverExist)
					return false;

				// 覆盖不需要删除原有，因为注册的地方会自己删除
				//delete findIter->second;
			}

			_objectCreatorMap[lpName] = pObjCreater;

			return true;
		}

		/**
		*  @brief    删除Object创建
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @param    IKObjectCreator * pObjCreater 创建类的对象
		*  @return   bool 成功返回true
		*/
		virtual bool RemoveFactory(LPCWSTR lpName, KObjectCreator* pObjCreater) override
		{
			if (NULL == lpName || (NULL == pObjCreater))
				return false;

			ObjectCreaterMapIter findIter = _objectCreatorMap.find(lpName);
			if (findIter != _objectCreatorMap.end())
			{
				_objectCreatorMap.erase(findIter);
				return true;
			}

			return false;
		}

		/**
		*  @brief    删除Object创建
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   bool 成功返回true
		*/
		virtual bool DeleteFactory(LPCWSTR lpName) override
		{
			if (NULL == lpName)
				return false;

			ObjectCreaterMapIter findIter = _objectCreatorMap.find(lpName);
			if (findIter != _objectCreatorMap.end())
			{
				delete findIter->second;
				_objectCreatorMap.erase(findIter);
				return true;
			}

			return false;
		}

	private:
		/**
		*  @brief    从工厂中创建对象
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   KObject* 失败返回NULL
		*/
		virtual KObject* FindObject(LPCWSTR lpName)
		{
			ObjectCreaterMapIter findIter = _objectCreatorMap.find(lpName);
			if (findIter != _objectCreatorMap.end())
			{
				return findIter->second->NewInstance();
			}

			return NULL;
		}

	protected:
		ObjectCreaterMap	_objectCreatorMap;	///< Object类创建工厂
	};

	static KObjectFactoryImpl* g_pObjectFactory = NULL;

	void InitKObjectFactory()
	{
		if (NULL == g_pObjectFactory)
		{
			g_pObjectFactory = new KObjectFactoryImpl();
		}
	}

	KObjectFactory* GetKObjectFactory()
	{
		return g_pObjectFactory;
	}

	void UninitKObjectFactory()
	{
		if (NULL != g_pObjectFactory)
		{
			delete g_pObjectFactory;
			g_pObjectFactory = NULL;
		}
	}
}