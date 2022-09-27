#pragma once

namespace YFramework 
{
	class ISystem
	{
		friend class BaseApp;
	public:
		virtual ~ISystem() {}
		virtual BaseApp * GetApp() = 0;
		virtual void SetApp(BaseApp *app) = 0;
	protected:
		virtual void Init() = 0;
	};

	class BaseSystem : public ISystem
	{
	public:
		virtual ~BaseSystem() {}
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