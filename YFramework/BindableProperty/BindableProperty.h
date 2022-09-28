#pragma once

namespace YFramework 
{
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
				_OnValueChanged.Invoke(oldVal, _val);
			}
		}

		void RegisterChangedEvent(std::function<void(T, T)> onEvent)
		{
			_OnValueChanged += onEvent;
		}

		void UnRegisterChangedEvent(std::function<void(T, T)> onEvent)
		{
			_OnValueChanged -= onEvent;
		}

	private:
		Delegate<void(T, T)> _OnValueChanged;
		T _val;
	};
}
