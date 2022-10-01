#pragma once

namespace YFramework 
{
	template<typename T>
	class BindableProperty 
	{
	public:
		BindableProperty(const T &val)
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
		inline void Set(const T &val)
		{
			if (val != _val)
			{
				T oldVal = _val;
				_val = val;
				_OnValueChanged.Invoke(oldVal, _val);
			}
		}

		void RegisterChangedEvent(std::function<void(const T&, const T&)> onEvent)
		{
			_OnValueChanged += onEvent;
		}

		void UnRegisterChangedEvent(std::function<void(const T&, const T&)> onEvent)
		{
			_OnValueChanged -= onEvent;
		}

	private:
		Delegate<void(const T&, const T&)> _OnValueChanged;
		T _val;
	};
}

// 使用方法
//std::function<void(const CString&, const CString&)> _OnSearchKeyChanged = std::bind(&CTaskListCtrl::OnSearchKeyChanged, this, std::placeholders::_1, std::placeholders::_2);
//GetModel<CTaskModel>()->SearchKey.RegisterChangedEvent(_OnSearchKeyChanged);
