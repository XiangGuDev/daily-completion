#pragma once

namespace YFramework 
{
	class ISystem : public IGetModel, public IGetUtility, public ISetApp, public IRegisterEvent, public ISendEvent
	{
		friend class BaseApp;
	public:
		virtual ~ISystem() {}
	protected:
		virtual void Init() = 0;
		virtual void SetApp(BaseApp *app) = 0;
	};

	class BaseSystem : public ISystem
	{
	public:
		virtual ~BaseSystem() {}
	private:
		virtual BaseApp * GetApp() override
		{
			return _app;
		}
		virtual void SetApp(BaseApp *app) override
		{
			_app = app;
		}
	private:
		BaseApp *_app;
	};
}