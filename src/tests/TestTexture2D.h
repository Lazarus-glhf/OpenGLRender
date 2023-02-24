#pragma once

#include <glm/glm.hpp>

#include "Test.h"

#include "Texture.h"
#include "VertexBufferLayout.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Texture> m_Texture;

		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
	};
}
