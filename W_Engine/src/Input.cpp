#include <W_Engine/Input.h>

#include <W_Engine/Application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* getWindowPointer()
{
    return W_Engine::Application::Get().GetWindow()->GetGLFWwindow();
}

namespace W_Engine
{
    bool Input::GetKeyDown(int keycode)
    {
        auto state = glfwGetKey(
            getWindowPointer(),
            keycode
        );
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::GetMouseButtonDown(int button)
    {
        auto state = glfwGetMouseButton(
            getWindowPointer(),
            button
        );
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        double x{};
        double y{};

        glfwGetCursorPos(
            getWindowPointer(),
            &x, &y
        );
        return glm::vec2(static_cast<float>(x), static_cast<float>(y));
    }
}

