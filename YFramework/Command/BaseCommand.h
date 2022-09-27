#pragma once

namespace YFramework
{
	class ICommand : public IGetUtility, public IGetModel, public IGetSystem, public ISetApp
	{
		friend class BaseApp;
	public:
		virtual ~ICommand() {}
	protected:
		virtual void Init() = 0;
		virtual void Execute() = 0;
	};

	class BaseCommand : public ICommand
	{
	public:
		virtual ~BaseCommand() {}
	private:
		virtual BaseApp * GetApp()
		{
			return _app;
		}
		virtual void SetApp(BaseApp *app)
		{
			_app = app;
		}
	private:
		BaseApp *_app;
	};
}