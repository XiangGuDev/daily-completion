#pragma once
#include<string>
#include<unordered_map>
#include<memory>
#include<functional>
#include "Any.h"
#include "NonCopyable.h"

namespace YFramework
{
	class IocContainer : NonCopyable
	{
	public:
		IocContainer() = default;
		~IocContainer() = default;

		template<class T, typename Depend, typename... Args>
		typename std::enable_if<!std::is_base_of<T, Depend>::value>::type RegisterSub(const std::string& strKey) {
			std::function<T* (Args&&...)> function = [](Args&&... args) { return new T(new Depend(std::forward<Args>(args)...)); };//通过闭包擦除了参数类型
			RegisterSub(strKey, std::move(function));
		}

		template<class T, typename Depend, typename... Args>
		typename std::enable_if<std::is_base_of<T, Depend>::value>::type RegisterSub(const std::string& strKey) {
			std::function<T* (Args&&...)> function = [](Args&&... args) { return new Depend(std::forward<Args>(args)...); };
			RegisterSub(strKey, std::move(function));
		}

		template <class T, typename... Args>
		void Register(const std::string& strKey) {
			std::function<T*(Args&&...)> function = [](Args&&... args) {return new T(std::forward<Args>(args)...);};
			RegisterSub(strKey, std::move(function));
		}

		template<class T, typename... Args>
		T* Get(const std::string& strKey, Args&&... args) {
			if (m_creatorMap.find(strKey) == m_creatorMap.end())
				return nullptr;

			Any resolver = m_creatorMap[strKey];
			std::function<T* (Args&&...)> function = resolver.AnyCast<std::function<T* (Args&&...)>>();
			return function(std::forward<Args>(args)...);
		}

		template<class T, typename... Args>
		std::shared_ptr<T> GetShared(const std::string& strKey, Args&&... args) {
			T* t = Get<T>(strKey, std::forward<Args>(args)...);
			return std::shared_ptr<T>(t);
		}

	private:
		void RegisterSub(const std::string& strKey, Any&& constructor) {
			if (m_creatorMap.find(strKey) != m_creatorMap.end())
				throw std::invalid_argument("this key has already exist!");

			//通过Any擦除了不同类型的构造器
			m_creatorMap.emplace(strKey, constructor);
		}

	private:
		std::unordered_map<std::string, Any> m_creatorMap;
	};
}

//----------------使用方法---------------------------------------------------------
//IocContainer ioc;
////注册继承对象
//ioc.RegisterSub<A, DerivedC>("C");      //配置依赖关系
//ioc.RegisterSub<A, DerivedB, int, double>("B");   //注册时要注意DerivedB的参数int和double
//
////注册普通对象
//ioc.Register<Bus>("Bus");
//ioc.Register<Car>("Car");
//
//auto c = ioc.GetShared<A>("C");
//c->Func();
//auto b = ioc.GetShared<A>("B", 1, 2.0); //还要传入参数
//b->Func();
//
//auto bus = ioc.GetShared<Bus>("Bus");
//bus->Func();
//auto car = ioc.GetShared<Car>("Car");
//car->Func();