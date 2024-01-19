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
    glm::vec3 movement{};
    if (app.GetInput().GetKeyDown(KEY_W))
    {
        movement += glm::vec3(0, 0, -1);
    }
    if (app.GetInput().GetKeyDown(KEY_S))
    {
        movement += glm::vec3(0, 0, 1);
    }
    if (app.GetInput().GetKeyDown(KEY_A))
    {
        movement += glm::vec3(-1, 0, 0);
    }
    if (app.GetInput().GetKeyDown(KEY_D))
    {
        movement += glm::vec3(1, 0, 0);
    }
    if (app.GetInput().GetKeyDown(KEY_SPACE))
    {
        movement += glm::vec3(0, 1, 0);
    }
    if (app.GetInput().GetKeyDown(KEY_LEFT_CONTROL))
    {
        movement += glm::vec3(0, -1, 0);
    }
    cameraTransform.TranslateLocalSpace(movement * CAMERA_MOVE_SPEED * Time::DeltaTime());

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
        //float pitchRadians = std::asin(heading.y);
        //float pitchDegrees = glm::degrees(pitchRadians);

        //float yaw = std::atan2(heading.z, heading.x);
        //float yawDegrees = glm::degrees(yaw);

    }
    lastMousePosition = currentMousePos;
}

int main()
{
    using namespace W_Engine;

    Application app{};
    Renderer& renderer = app.GetRenderer();
    ResourceManager& resourceManager = app.GetResourceManager();

    Shader shader = resourceManager.LoadShader("default.shader");
    Model cube = resourceManager.LoadModel("Minecraft_Grass_Block_OBJ\\Grass_Block.obj");
    
    renderer.SetBackgroundColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
    renderer.EnableDepthTest(true);

    Camera camera(45.0f);
    camera.GetTransform().SetPosition(glm::vec3(0.0f,0.0f,20.0f));

    Transform cubeTransform{};
    cubeTransform.SetPosition(glm::vec3(0,0,10));

    while (Application::IsRunning())
    {
        app.InitializeAndPoll();

        moveCamera(camera.GetTransform());

        app.PreRender(camera);

        cube.Draw(cubeTransform, shader);

        app.PostRender();
    }
	return 0;
}

