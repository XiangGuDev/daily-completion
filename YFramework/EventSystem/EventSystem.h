#pragma once
#include <unordered_map>
#include <functional>
#include <memory>

namespace YFramework
{
	class TypeEventSystem;
	class IUnRegister
	{
	public:
		virtual ~IUnRegister(){}
		virtual void UnRegister() = 0;
	};

	template<class T>
	class TypeEventSystemUnRegister : public IUnRegister
	{
	public:
		TypeEventSystemUnRegister()
		{
			_typeEventSystem = NULL;
		}
		virtual void UnRegister() override
		{
			if (_typeEventSystem != NULL)
			{
				_typeEventSystem = NULL;
				_onEvent.Clear();
			}
		}
	public:
		TypeEventSystem *_typeEventSystem;
		Delegate<void(std::shared_ptr<T>)> _onEvent;
	};

	class TypeEventSystem
	{
	public:
		struct IEventRegs
		{
			virtual ~IEventRegs() = default;
		};

		template <typename T>
		struct EventRegs : public IEventRegs
		{
			Delegate<void(std::shared_ptr<T>)> _onEvent;
		};
		
		template<typename T>
		void Send()
		{
			Send<T>(std::make_shared<T>());
		}

		template<typename T>
		void Send(std::shared_ptr<T> arg)
		{
			if (_eventRegList.find(typeid(T).hash_code()) != _eventRegList.end())
			{
				auto& e = _eventRegList[typeid(T).hash_code()];
				auto eventRegs = std::static_pointer_cast<EventRegs<T>>(e);
				eventRegs->_onEvent.Invoke(arg);
			}
		}

		template<typename T>
		std::shared_ptr<IUnRegister> Register(std::function<void(std::shared_ptr<T>)> onEvent)
		{
			shared_ptr<EventRegs<T>> eventRegs;
			if (_eventRegList.find(typeid(T).hash_code()) != _eventRegList.end())
			{
				auto& e = _eventRegList[typeid(T).hash_code()];
				eventRegs = std::static_pointer_cast<EventRegs<T>>(e);
			}
			else
			{
				eventRegs = std::make_shared<EventRegs<T>>();
			}

			eventRegs->_onEvent += onEvent;
			_eventRegList[typeid(T).hash_code()] = eventRegs;

			auto unreg = std::make_shared<TypeEventSystemUnRegister<T>>();
			unreg->_onEvent += onEvent;
			unreg->_typeEventSystem = this;
			return unreg;
		}

		template<typename T>
		void UnRegister(std::function<void(std::shared_ptr<T>)> onEvent)
		{
			if (_eventRegList.find(typeid(T).hash_code()) != _eventRegList.end())
			{
				auto& e = _eventRegList[typeid(T).hash_code()];
				auto eventRegs = std::static_pointer_cast<EventRegs<T>>(e);
				eventRegs->_onEvent -= onEvent;
			}
		}

	public:
		static TypeEventSystem Global;
	private:
		
		std::unordered_map<size_t, std::shared_ptr<IEventRegs>> _eventRegList;
	};
}
