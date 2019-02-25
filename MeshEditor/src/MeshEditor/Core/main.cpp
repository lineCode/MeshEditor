#include "GUI/Interfaces/Window.h"
#include "GUI/Interfaces/WindowManager.h"
#include "GUI/Core/ImGuiDialog.h"
#include "GUI/Core/GlfwWindowManager.h"

#include "MeshEditor/Views/MeshDialog.h"
#include "MeshEditor/Views/MeshEditorPanel.h"
#include "MeshEditor/Views/FilePanel.h"
#include "MeshEditor/Controllers/MeshEditorController.h"
#include "MeshEditor/Controllers/CameraController.h"
#include "MeshEditor/Controllers/FileController.h"

#include "Rendering/Core/MeshRenderer.h"

using namespace MeshEditor::Views;
using namespace MeshEditor::Controllers;

using namespace GUI::Core;
using namespace GUI::Interfaces;

using namespace Rendering::Core;

constexpr unsigned int WINDOW_WIDTH = 1200;
constexpr unsigned int WINDOW_HEIGHT = 700;

int main(int, char**)
{
	// Create window manager
	WindowManager* manager = GlfwWindowManager::GetInstance();
	Window* window = manager->NewWindow(42, WINDOW_WIDTH, WINDOW_HEIGHT, 400, 400, "Test Window");

	// Create Mesh renderer
	MeshRenderer* meshRenderer = new MeshRenderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	meshRenderer->Init();
	meshRenderer->SetLightType(LightType::POINT_LIGHT);
	meshRenderer->SetLightColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshRenderer->SetRenderMode(RenderMode::WIREFRAME);

	// Mesh dialog
	MeshDialog* meshDialog = new MeshDialog(42);
	meshDialog->SetMeshRenderer(meshRenderer);

	// Mesh editor dialog
	ImGuiDialog* meshEditorDialog = new ImGuiDialog(12, "Mesh editor", 50, 220, 200, 400);
	MeshEditorController* meshEditorController = new MeshEditorController(meshRenderer);
	MeshEditorPanel* meshEditorPanel = new MeshEditorPanel(meshEditorController);
	meshEditorDialog->SetPanel(meshEditorPanel);

	// File dialog
	ImGuiDialog* fileDialog = new ImGuiDialog(1, "File dialog", 50, 50, 200, 150);
	FileController* fileController = new FileController(meshRenderer);
	FilePanel* filePanel = new FilePanel(fileController);
	fileDialog->SetPanel(filePanel);

	// Add all dialogs
	window->AddDialog(meshDialog);
	window->AddDialog(meshEditorDialog);
	window->AddDialog(fileDialog);
	
	// Camera controller
	CameraController* cameraController = new CameraController(meshRenderer);
	window->AddMouseListener(cameraController);
	window->AddWindowListener(cameraController);

	// Start application
	manager->Start();   

    return 0;
}
