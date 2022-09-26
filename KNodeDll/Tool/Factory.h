//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           Factory.H
//    @author         KG
//    @date           2017/11/06 20:20
//    @brief          对象工厂接口类
//
//*****************************************************

#pragma once

namespace CommBase
{
	/**
	*  @class    平台扩展基类接口
	*
	*  @brief
	*/
	class COMMBASE_API KObject
	{
	public:
		KObject()
		{
			_nVersion = 0x8000;	// VER: 1.0.0.0
		}

		virtual ~KObject() {}

		/**
		*  @brief    获取扩展接口版本(用于后期兼容性判断);
		*
		*  @return   LPCWSTR 返回接口版本
		*/
		int GetVersion()
		{
			return _nVersion;
		}

	protected:
		int	_nVersion;	///< 接口版本
	};


	/**
	*  @class    Object类创建接口
	*
	*  @brief
	*/
	class KObjectCreator
	{
	public:
		virtual ~KObjectCreator() {}

		/**
		*  @brief    创建实例
		*
		*  @return   KObject* 返回新创建的Object指针
		*/
		virtual KObject* NewInstance() = 0;
	};

	/**
	*  @class    Object类创建实现类
	*
	*  @brief
	*/
	template<typename SubT>
	class KObjectCreatorProxy : public KObjectCreator
	{
	public:
		KObjectCreatorProxy(bool bSingleton)
		{
			_pInstance = NULL;
			_bSingleton = bSingleton;
		}

		~KObjectCreatorProxy()
		{
			if (NULL != _pInstance)
				delete _pInstance;
		}

		KObject* NewInstance()
		{
			if (_bSingleton)
			{
				if (NULL == _pInstance)
					_pInstance = new SubT();
				return _pInstance;
			}
			else
			{
				return new SubT();
			}
		}

	private:
		bool _bSingleton;	// 是否单例
		SubT* _pInstance;	// 单例指针
	};

	// 声明获取对象接口方法
#define DECLARE_KOBJECT(ObjectName, ObjectInterface) \
	_inline ObjectInterface* New##ObjectInterface() \
	{\
		return (ObjectInterface*)(GetKObjectFactory()->NewObject(ObjectName)); \
	}

	// 声明获取服务接口方法
#define DECLARE_KSERVICE(ServiceName, ServiceInterface) \
	_inline ServiceInterface* Get##ServiceInterface() \
	{\
		return (ServiceInterface*)(GetKObjectFactory()->GetService(ServiceName)); \
	}

	/**
	* REGISTER_OBJECT(对象ID, 类名);
	* 注册Object对象
	*/
#ifndef _LIB
#define REGISTER_KOBJECT(ObjectName, ObjectClass) \
	class Register##ObjectClass##ObjectCreator \
	{ \
	public: \
		Register##ObjectClass##ObjectCreator () \
			{\
				GetKObjectFactory()->AddFactory(ObjectName, new KObjectCreatorProxy<ObjectClass>(false));\
			} \
		~Register##ObjectClass##ObjectCreator () \
			{\
				GetKObjectFactory()->DeleteFactory(ObjectName);\
			} \
	} Register##ObjectClass##ObjectCreator##Instance;

	/**
	* REGISTER_KSERVICE(服务ID, 类名);
	* 注册Service对象
	*/
#define REGISTER_KSERVICE(ServiceName, ServiceClass) \
	class Register##ServiceClass##ObjectCreator \
	{ \
	public: \
		Register##ServiceClass##ObjectCreator () \
			{\
				GetKObjectFactory()->AddFactory(ServiceName, new KObjectCreatorProxy<ServiceClass>(true));\
			} \
		~Register##ServiceClass##ObjectCreator () \
		{\
				GetKObjectFactory()->DeleteFactory(ServiceName);\
		} \
	} Register##ServiceClass##ObjectCreator##Instance;
#endif

	/**
	*  @class    KObject创建工厂接口
	*
	*  @brief
	*/
	class KObjectFactory
	{
	private:
		virtual ~KObjectFactory() {}
		friend class KObjectFactoryImpl;

	public:
		/**
		*  @brief    从工厂中获取Object对象(需要自行释放指针)
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   KObject* 创建失败返回NULL
		*/
		virtual KObject* NewObject(LPCWSTR lpName) = 0;

		/**
		*  @brief    从工厂中获取Service对象(单例)
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   KObject* 创建失败返回NULL
		*/
		virtual KObject* GetService(LPCWSTR lpName) = 0;

		/**
		*  @brief    添加Object创建
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @param    KObjectCreator * pObjCreater 创建类的对象
		*  @param    bool bCoverExist 是否覆盖已注册的
		*  @return   bool 成功返回true
		*/
		virtual bool AddFactory(LPCWSTR lpName, KObjectCreator* pObjCreater,
			bool bCoverExist = false) = 0;

		/**
		*  @brief    移除Object创建
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @param    KObjectCreator * pObjCreater 创建类的对象
		*  @return   bool 成功返回true
		*/
		virtual bool RemoveFactory(LPCWSTR lpName, KObjectCreator* pObjCreater) = 0;

		/**
		*  @brief    删除Object创建
		*
		*  @param    LPCWSTR lpName 对象名称
		*  @return   bool 成功返回true
		*/
		virtual bool DeleteFactory(LPCWSTR lpName) = 0;
	};

	/**
	*  @brief    初始化Object工厂
	*
	*/
	COMMBASE_API void InitKObjectFactory();

	/**
	*  @brief    获取Object工厂接口指针
	*
	*  @return   IObjectFactory* 返回Object工厂接口
	*/
	COMMBASE_API KObjectFactory* GetKObjectFactory();

	/**
	*  @brief    反初始化Object工厂
	*
	*/
	COMMBASE_API void UninitKObjectFactory();
}