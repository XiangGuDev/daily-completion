#pragma once

namespace YFramework
{
	class BaseApp;

	class IGetApp
	{
		friend class IGetUtility;
		friend class IGetModel;
		friend class IGetSystem;
		friend class ISendCommand;
		friend class ISendEvent;
		friend class IRegisterEvent;
	public:
		virtual ~IGetApp() {}
	private:
		virtual BaseApp * GetApp() = 0;
	};

	class ISetApp
	{
		friend class BaseApp;
	public:
		virtual ~ISetApp() {}
	private:
		virtual void SetApp(BaseApp *app) = 0;
	};

	class ISendEvent : public IGetApp
	{
	public:
		virtual ~ISendEvent() {}
		template<typename TEvent>
		void SendEvent()
		{
			this->GetApp()->SendEvent<TEvent>();
		}

		template<typename TEvent>
		void SendEvent(TEvent e)
		{
			this->GetApp()->SendEvent<TEvent>(e);
		}
	};

	class IRegisterEvent : public IGetApp
	{
	public:
		virtual ~IRegisterEvent() {}
		template<typename TEvent>
		void RegisterEvent(std::function<void(std::shared_ptr<TEvent>)> onEvent)
		{
			this->GetApp()->RegisterEvent<TEvent>(onEvent);
		}

		template<typename TEvent>
		void UnRegisterEvent(std::function<void(std::shared_ptr<TEvent>)> onEvent)
		{
			this->GetApp()->UnRegisterEvent<TEvent>(onEvent);
		}
	};

	class IGetUtility : public IGetApp
	{
	public:
		virtual ~IGetUtility(){}
		template <typename TUtility>
		shared_ptr<TUtility> GetUtility()
		{
			return this->GetApp()->GetUtility<TUtility>();
		}
	};

	class IGetModel : public IGetApp
	{
	public:
		virtual ~IGetModel() {}
		template <typename TModel>
		shared_ptr<TModel> GetModel()
		{
			return this->GetApp()->GetModel<TModel>();
		}
	};

	class IGetSystem : public IGetApp
	{
	public:
		virtual ~IGetSystem() {}
		template<typename TSystem>
		shared_ptr<TSystem> GetSystem()
		{
			return this->GetApp()->GetSystem<TSystem>();
		}
	};

	class ISendCommand : public IGetApp
	{
	public:
		virtual ~ISendCommand() {}
		template<typename TCommand>
		void SendCommand()
		{
			this->GetApp()->SendCommand<TCommand>();
		}

		template<typename TCommand>
		void SendCommand(shared_ptr<TCommand> command)
		{
			this->GetApp()->SendCommand(command);
		}
	};
}