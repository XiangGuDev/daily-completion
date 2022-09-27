#pragma once
#include <memory>

namespace YFramework
{
	class IModel : public IGetUtility, public ISetApp
	{
		friend class BaseApp;
	public:
		virtual ~IModel() {}
	protected:
		virtual void Init() = 0;
	};

	class BaseModel : public IModel
	{
	public:
		virtual ~BaseModel() {}
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
