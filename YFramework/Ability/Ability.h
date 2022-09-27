#pragma once

namespace YFramework
{
	class IUtility;
	class IModel;
	class ISystem;
	class ICommand;
	class BaseApp;

	class IGetApp
	{
		friend class IGetUtility;
		friend class IGetModel;
		friend class IGetSystem;
		friend class ISendCommand;
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