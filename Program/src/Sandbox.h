#pragma once
#include <W_Engine/Application.h>
#include <W_Engine/ResourceManager.h>


class Sandbox : public W_Engine::Application
{
public:
	Sandbox()
	{
        W_Engine::Shader shader = W_Engine::ResourceManager::LoadShader("default.shader");
        W_Engine::Model model = W_Engine::ResourceManager::LoadModel("cube.obj");
		Run();
	}

	~Sandbox()
	{

	}
};