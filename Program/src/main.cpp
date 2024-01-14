#include <W_Engine/Log.h>
#include <glm/mat2x2.hpp>

#include <W_Engine/ResourceManager.h>
#include <W_Engine/Renderer.h>
#include <W_Engine/Application.h>
#include <W_Engine/Time.h>

constexpr float CAMERA_VIEW_SPEED = 5.0f;
constexpr float CAMERA_MOVE_SPEED = 3.0f;


//horrible, hacky way of doing this 
//just for the sake of demonstration
//definitely should work on architecture
void moveCamera(W_Engine::Transform& cameraTransform)
{
    using namespace W_Engine;

    Application& app = Application::Get();
    if (app.GetInput().GetKeyDown(KEY_W))
    {
        cameraTransform.TranslateLocalSpace(
            glm::vec3(0, 0, -1) * CAMERA_MOVE_SPEED * Time::DeltaTime()
        );
    }
    if (app.GetInput().GetKeyDown(KEY_S))
    {
        cameraTransform.TranslateLocalSpace(
            glm::vec3(0, 0, 1) * CAMERA_MOVE_SPEED * Time::DeltaTime()
        );
    }
    if (app.GetInput().GetKeyDown(KEY_A))
    {
        cameraTransform.TranslateLocalSpace(
            glm::vec3(-1, 0, 0) * CAMERA_MOVE_SPEED * Time::DeltaTime()
        );
    }
    if (app.GetInput().GetKeyDown(KEY_D))
    {
        cameraTransform.TranslateLocalSpace(
            glm::vec3(1, 0, 0) * CAMERA_MOVE_SPEED * Time::DeltaTime()
        );
    }

    static glm::vec2 lastMousePosition = glm::vec2{};
    static glm::vec2 yawAndPitch = glm::vec2{};

    glm::vec2 currentMousePos = app.GetInput().GetMousePosition();
    if (app.GetInput().GetMouseButtonDown(MOUSE_BUTTON_1))
    {
        glm::vec2 diff = currentMousePos - lastMousePosition;
        diff *= CAMERA_VIEW_SPEED * Time::DeltaTime();
        yawAndPitch -= diff;
        if (yawAndPitch.y > 89.0f)
        {
            yawAndPitch.y = 89.0f;
        }
        if (yawAndPitch.y < -89.0f)
        {
            yawAndPitch.y = -89.0f;
        }
        cameraTransform.SetRotation(glm::vec3(yawAndPitch.y, yawAndPitch.x, 0.0f));
        glm::vec3 heading = cameraTransform.GetForward();
        heading = glm::normalize(heading);

        // Calculate pitch (angle with the XZ plane)
        float pitchRadians = std::asin(heading.y);
        float pitchDegrees = glm::degrees(pitchRadians);

        float yaw = std::atan2(heading.z, heading.x);
        float yawDegrees = glm::degrees(yaw);

    }
    lastMousePosition = currentMousePos;
}

int main()
{
    W_Engine::Application app{};
    W_Engine::Renderer& renderer = app.GetRenderer();
    W_Engine::ResourceManager& resourceManager = app.GetResourceManager();

    W_Engine::Shader shader = resourceManager.LoadShader("default.shader");
    W_Engine::Model model = resourceManager.LoadModel("cube.obj");
    
    renderer.SetBackgroundColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
    renderer.EnableDepthTest(true);

    W_Engine::Camera camera(45.0f);
    camera.GetTransform().SetPosition(glm::vec3(0.0f,0.0f,20.0f));

    W_Engine::Transform transform{};
    transform.SetPosition(glm::vec3(0,0,10));

    while (W_Engine::Application::IsRunning())
    {
        app.InitializeAndPoll();

        moveCamera(camera.GetTransform());

        app.PreRender(camera);

        model.Draw(transform, shader);

        app.PostRender();
    }
	return 0;
}

