#pragma once

#include "Test.h"

namespace test
{
	class TestClearColor : Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};
}
