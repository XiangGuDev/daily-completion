#pragma once

namespace YFramework
{
	class ICommand
	{
	public:
		virtual ~ICommand() {}
		virtual void Excute() = 0;
	};
}