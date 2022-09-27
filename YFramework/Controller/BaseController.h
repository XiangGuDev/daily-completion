#pragma once

namespace YFramework
{
	class IController
	{
	public:
		virtual ~IController(){}
	protected:
		virtual BaseApp * GetApp() = 0;
	};
}
