#pragma once

#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

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

		void OnImGuiRender() override;

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Test, T>, void>>
		void Registertest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back({name, [] { return new T(); }});
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}
