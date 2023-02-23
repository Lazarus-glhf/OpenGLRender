#pragma once
#include <functional>
#include <string>
#include <vector>

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
		TestMenu() {}
		~TestMenu() {}

		void OnImGuiRender() override;

	private:
		Test* m_CurrentTest;
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}
