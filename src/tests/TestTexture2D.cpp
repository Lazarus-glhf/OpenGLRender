#include "TestTexture2D.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "vender/imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

test::TestTexture2D::TestTexture2D()
	:m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
{
	float positions[] = {
	-50.0f, -50.0f, 0.0f, 0.0f, // 0
	 50.0f, -50.0f, 1.0f, 0.0f, // 1
	 50.0f,  50.0f, 1.0f, 1.0f, // 2
	-50.0f,  50.0f, 0.0f, 1.0f  // 3
	};
	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	m_VAO = std::make_unique<VertexArray>();

	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6);

	m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");
	m_shader->Bind();
	m_Texture = std::make_unique<Texture >("res/texture/Avatar.jpg");
	m_shader->SetUniform1i("u_Texture", 0);	
}

test::TestTexture2D::~TestTexture2D()
{
}

void test::TestTexture2D::OnRender()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	Renderer renderer;

	m_Texture->Bind();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_shader->Bind();
		m_shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
	}

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_shader->Bind();
		m_shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
	}
}

void test::TestTexture2D::OnUpdate(float deltaTime)
{
	Test::OnUpdate(deltaTime);
}

void test::TestTexture2D::OnImGuiRender()
{
	ImGui::SliderFloat3("translationA", &m_TranslationA.x, 0.0f, 960.0f);
	ImGui::SliderFloat3("translationB", &m_TranslationB.x, 0.0f, 960.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
