#pragma once

namespace YFramework 
{
	template<typename>
	class BindableProperty;

	template<typename T>
	class BindableProperty 
	{
	public:
		BindableProperty(T val)
		{
			_val = val;
		}
		BindableProperty()
		{
		}
		inline T Get()
		{
			return _val;
		}
		inline void Set(T val)
		{
			if (val != _val)
			{
				_val = val;
				OnCountChanged.Invoke(_val);
			}
		}
	public:
		Delegate<void(T)> OnCountChanged;
	private:
		T _val;
	};
}
