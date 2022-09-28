#pragma once

namespace YFramework
{
	class BaseApp 
	{
	public:
		BaseApp()
		{
			_bInit = false;
		}
		virtual ~BaseApp(){}

		// 初始化App（需要在程序入口点调用）
		void Init()
		{
			if(_bInit) return;

			// 子类初始化
			OnInit();

			// 提供外部配置化接口
			OnRegisterConfig.Invoke(this);

			// 初始化所有model
			for (int i = 0; i < _modelList.size(); i++)
			{
				_modelList[i]->Init();
			}

			// 初始化所有system
			for (int i = 0; i < _systemList.size(); i++)
			{
				_systemList[i]->Init();
			}

			_bInit = true;
		}

		template<typename TEvent>
		void SendEvent() 
		{
			_eventSystem.Send<TEvent>();
		}

		template<typename TEvent>
		void SendEvent(TEvent e) 
		{
			_eventSystem.Send<TEvent>(e);
		}

		template<typename TEvent>
		void RegisterEvent(std::function<void(std::shared_ptr<TEvent>)> onEvent)
		{
			_eventSystem.Register<TEvent>(onEvent);
		}

		template<typename TEvent>
		void UnRegisterEvent(std::function<void(std::shared_ptr<TEvent>)> onEvent)
		{
			_eventSystem.UnRegister<TEvent>(onEvent);
		}

		template<typename TCommand>
		inline void SendCommand()
		{
			static_assert(std::is_base_of<ICommand, TCommand>::value, "类型必须继承自ICommand");
			shared_ptr<ICommand> cmd = std::make_shared<TCommand>();
			cmd->SetApp(this);
			cmd->Init();
			cmd->Execute();
		}

		template<typename TCommand>
		inline void SendCommand(shared_ptr<TCommand> command)
		{
			static_assert(std::is_base_of<ICommand, TCommand>::value, "类型必须继承自ICommand");
			shared_ptr<ICommand> cmd = command;
			cmd->SetApp(this);
			cmd->Init();
			cmd->Execute();
		}

		template<typename TUtility>
		inline void RegisterUtility(shared_ptr<TUtility> utility)
		{
			static_assert(std::is_base_of<IUtility, TUtility>::value, "类型必须继承自IUtility");
			_ioc.RegisterInstance<TUtility>(utility);
		}

		template<typename TModel>
		inline void RegisterModel(shared_ptr<TModel> model)
		{
			static_assert(std::is_base_of<IModel, TModel>::value, "类型必须继承自IModel");
			_ioc.RegisterInstance<TModel>(model);
			shared_ptr<IModel> mdl = model;
			mdl->SetApp(this);
			if (_bInit)
				mdl->Init();
			else
				_modelList.push_back(mdl);
		}

		template<typename TSystem>
		inline void RegisterSystem(shared_ptr<TSystem> system)
		{
			static_assert(std::is_base_of<ISystem, TSystem>::value, "类型必须继承自ISystem");
			_ioc.RegisterInstance<TSystem>(system);
			shared_ptr<ISystem> sys = system;
			sys->SetApp(this);
			if (_bInit)
				sys->Init();
			else
				_systemList.push_back(sys);
		}

		template <typename TModel>
		shared_ptr<TModel> GetModel()
		{
			static_assert(std::is_base_of<IModel, TModel>::value, "类型必须继承自IModel");
			return _ioc.Get<TModel>();
		}

		template<typename TSystem>
		shared_ptr<TSystem> GetSystem()
		{
			static_assert(std::is_base_of<ISystem, TSystem>::value, "类型必须继承自ISystem");
			return _ioc.Get<TSystem>();
		}

		template <typename TUtility>
		shared_ptr<TUtility> GetUtility()
		{
			static_assert(std::is_base_of<IUtility, TUtility>::value, "类型必须继承自IUtility");
			return _ioc.Get<TUtility>();
		}
	protected:
		// 子类实现
		virtual void OnInit() = 0;
	public:
		Delegate<void(BaseApp *)> OnRegisterConfig;
	private:
		std::vector<std::shared_ptr<IModel>> _modelList;
		std::vector<std::shared_ptr<ISystem>> _systemList;
		IocContainer _ioc;
		TypeEventSystem _eventSystem;
		bool _bInit;
	};

	
}
// ----------------------注意-------------------------
// 继承该类需要添加单例基类为友元类
// 	friend BaseSingleton<类名>;