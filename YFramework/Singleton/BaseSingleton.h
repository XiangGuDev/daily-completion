#pragma once

namespace YFramework
{
	template <typename T>
	class BaseSingleton {
	public:
		BaseSingleton() = default;
		virtual ~BaseSingleton() = default;
		BaseSingleton(T &&) = delete;
		BaseSingleton(const T &) = delete;
		void operator = (const T &) = delete;

		static T* Instance()
		{
			static T object;
			return &object;
		}
	};
}

//----------------使用方法---------------------------------------------------------
//class Test : public BaseSingleton<Test>
//{
//	friend class BaseSingleton<Test>;
//	Test() = default;
//public:
//	void setId(int id)
//	{
//		this->id = id;
//	}
//	int getId() const
//	{
//		return this->id;
//	}
//	int id;
//};
//int main()
//{
//	//Test t;  //错误，禁止创建对象
//	Test::Instance()->setId(5);
//	std::cout << Test::Instance()->getId();
//}