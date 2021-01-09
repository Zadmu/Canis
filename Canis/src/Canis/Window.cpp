#include "Window.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct Vertex
{
	float Position[3];
	float Color[4];
};

Window::Window(const std::string& title, uint32_t width, uint32_t height)
	: m_Title(title), m_Width(width), m_Height(height)
{
	if (!glfwInit())
	{
		std::cout << "glfwInit failed!\n";
		__debugbreak();
	}

	m_GLFWWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_GLFWWindow);

	if (!gladLoadGL())
	{
		std::cout << "Could not load glad!\n";
		__debugbreak();
	}

	m_LastFrameTime = GetTime();

	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);

	// Index Buffers
	// Vertex Colors
	// Transformations

	Vertex vertices[4];
	vertices[0] = { { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } };
	vertices[1] = { {  0.25f, -0.25f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	vertices[2] = { {  0.25f,  0.25f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	vertices[3] = { { -0.25f,  0.25f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f} };

	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glCreateBuffers(1, &m_VertexBuffer);
	glNamedBufferData(m_VertexBuffer, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &m_IndexBuffer);
	glNamedBufferData(m_IndexBuffer, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)12);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	std::string vertexSrc = R"(
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform vec3 u_Position;

void main()
{
	v_Color = a_Color;
	gl_Position = vec4(a_Position + u_Position, 1.0);
}
	)";

	std::string fragmentSrc = R"(
#version 430 core

layout(location = 0) out vec4 a_Color;

in vec4 v_Color;

uniform vec4 u_Color;

void main()
{
	a_Color = u_Color;
}
	)";

	m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

	// Bind
	// Operation
}

Window::~Window()
{
	glfwDestroyWindow(m_GLFWWindow);
}

void Window::DrawQuad(float x, float y, float r, float g, float b, float a)
{
	m_Shader->Bind();
	{
		GLint location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Position");
		if (location == -1)
			std::cout << "Uniform not found!" << std::endl;

		glUniform3f(location, x, y, 0.0f);
	}
	{
		GLint location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Color");
		if (location == -1)
			std::cout << "Uniform not found!" << std::endl;

		glUniform4f(location, r, g, b, a);
	}
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Window::DrawTriangle(float x, float y)
{
	m_Shader->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

#if 0
	glBegin(GL_TRIANGLES);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex2f(-0.5f + x, -0.5f + y);

	glColor3f(0.0f, 0.8f, 0.0f);
	glVertex2f(0.5f + x, -0.5f + y);

	glColor3f(0.0f, 0.0f, 0.8f);
	glVertex2f(0.0f + x, 0.5f + y);
	glEnd();
#endif
}

void Window::DrawQuad2(float x, float y, float width, float height)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	Vertex vertices[6];
	vertices[0] = { -halfWidth + x, -halfHeight + y, 0.0f };
	vertices[1] = { halfWidth + x, -halfHeight + y, 0.0f };
	vertices[2] = { halfWidth + x, halfHeight + y, 0.0f };
	vertices[3] = { halfWidth + x, halfHeight + y, 0.0f };
	vertices[4] = { -halfWidth + x, halfHeight + y, 0.0f };
	vertices[5] = { -halfWidth + x, -halfHeight + y, 0.0f };

#if 0
	glBegin(GL_TRIANGLES);
	glVertex2f(-halfWidth + x, -halfHeight + y);
	glVertex2f(halfWidth + x, -halfHeight + y);
	glVertex2f(halfWidth + x, halfHeight + y);

	glVertex2f(halfWidth + x, halfHeight + y);
	glVertex2f(-halfWidth + x, halfHeight + y);
	glVertex2f(-halfWidth + x, -halfHeight + y);
	glEnd();
#endif
}

void Window::Update()
{
	glfwSwapBuffers(m_GLFWWindow);
	glfwPollEvents();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float time = GetTime();
	float dt = time - m_LastFrameTime;
	m_DeltaTime = dt;
	m_LastFrameTime = time;
}

float Window::GetTime() const
{
	return (float)glfwGetTime();
}

bool Window::IsClosed() const
{
	return glfwWindowShouldClose(m_GLFWWindow);
}