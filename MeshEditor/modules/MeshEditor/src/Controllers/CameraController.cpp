#include "MeshEditor/CameraController.h"
#include "GUI/Window.h"
#include "Rendering/Camera.h"

#include <GLFW/glfw3.h>

using namespace MeshEditor;

CameraController::CameraController(Rendering::Camera* camera) : translate(false), rotate(false), camera(camera)
{

}

CameraController::~CameraController()
{

}

void CameraController::OnMouseButtonEvent(GUI::Window* window, double x, double y, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1)
		rotate = action == GLFW_PRESS;
	else if(button == GLFW_MOUSE_BUTTON_2)
		translate = action == GLFW_PRESS;
}

void CameraController::OnMouseMoveEvent(GUI::Window* window, double x, double y, double dx, double dy)
{
	float rdx = static_cast<float>(dx) / static_cast<float>(window->GetWidth());
	float rdy = static_cast<float>(dy) / static_cast<float>(window->GetHeight());

	if (rotate)
		camera->Rotate(rdx, rdy);
	else if (translate)
		camera->Translate(-rdx * 5.0f, rdy * 5.0f);
}

void CameraController::OnMouseScrollEvent(GUI::Window* window, double dx, double dy)
{
	camera->Zoom(static_cast<float>(dy) * 0.1f);
}

void CameraController::OnWindowResizedEvent(GUI::Window* window, int width, int height)
{
	camera->SetViewPort(width, height);
}
