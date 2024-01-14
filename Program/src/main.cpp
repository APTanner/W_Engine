#include <W_Engine/Log.h>
#include <glm/mat2x2.hpp>

#include <W_Engine/ResourceManager.h>
#include <W_Engine/Renderer.h>
#include <W_Engine/Application.h>

int main()
{
    W_Engine::Application app{};
    W_Engine::Renderer& renderer = app.GetRenderer();
    W_Engine::ResourceManager& resourceManager = app.GetResourceManager();

    W_Engine::Shader shader = resourceManager.LoadShader("default.shader");
    W_Engine::Model model = resourceManager.LoadModel("cube.obj");
    /*W_Engine::Model model{};

    std::vector<W_Engine::Vertex> verticies;

    W_Engine::Vertex v1, v2, v3;
    v1.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
    v1.Color = glm::vec3(1.0f, 0.0f, 0.0f);
    v2.Position = glm::vec3(0.5f, -0.5f, 0.0f);
    v2.Color = glm::vec3(0.0f, 1.0f, 0.0f);
    v3.Position = glm::vec3(0.0f, 0.5f, 0.0f);
    v3.Color = glm::vec3(0.0f, 0.0f, 1.0f);
    verticies.push_back(v1);
    verticies.push_back(v2);
    verticies.push_back(v3);

    std::vector<uint32_t> indicies;
    indicies.push_back(0);
    indicies.push_back(1);
    indicies.push_back(2);
    W_Engine::Material material{};

    model.Meshes.push_back(std::make_unique<W_Engine::Mesh>(verticies, indicies, std::vector<W_Engine::Texture>(), material));*/

    renderer.SetBackgroundColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
    renderer.EnableDepthTest(true);

    W_Engine::Camera camera(45.0f);
    camera.GetTransform().SetPosition(glm::vec3(0.0f,0.0f,20.0f));

    W_Engine::Transform transform{};
    transform.SetPosition(glm::vec3(0,0,10));

    while (W_Engine::Application::IsRunning())
    {
        app.PreRender(camera);

        model.Draw(transform, shader);

        app.PostRender();
    }
	return 0;
}

