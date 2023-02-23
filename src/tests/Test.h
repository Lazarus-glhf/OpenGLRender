#pragma once
#include <functional>
#include <string>
#include <vector>
#include <iostream>

namespace test
{
	class Test
	{
	public:
		Test() {}
		~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& CurrentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void Registertest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return dynamic_cast<Test*> (new T()); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}
