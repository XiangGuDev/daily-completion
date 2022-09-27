#pragma once

namespace YFramework
{
	class ICommand
	{
		friend class BaseApp;
	public:
		virtual ~ICommand() {}
	protected:
		virtual void Init() = 0;
		virtual void Execute() = 0;
		virtual BaseApp * GetApp() = 0;
		virtual void SetApp(BaseApp *app) = 0;
	};

	class BaseCommand : public ICommand
	{
	public:
		virtual ~BaseCommand() {}
	protected:
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