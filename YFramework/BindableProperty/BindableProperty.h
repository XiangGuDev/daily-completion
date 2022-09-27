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
				T oldVal = _val;
				_val = val;
				OnValueChanged.Invoke(oldVal, _val);
			}
		}
	public:
		Delegate<void(T, T)> OnValueChanged;
	private:
		T _val;
	};
}
