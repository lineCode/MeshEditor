#pragma once

#include <vector>
#include "GL/gl3w.h"
#include "GLM/glm.hpp"

#define NB_BUFFER 7

namespace Rendering
{
	class Mesh;
	class Vertex;
	
	enum RenderMode {
		MESH = 1,
		WIREFRAME = 1 << 2,
		VERTICES = 1 << 3,
		FACES_NORMALS = 1 << 4,
		VERTICES_NORMALS = 1 << 5
	};

	enum BufferId {
		BUF_VERTICES = 0,
		BUF_FACES = 1,
		BUF_NORMALS = 2,
		BUF_FACES_NORMALS = 3,
		BUF_VERTICES_NORMALS = 4,
		BUF_VERTICES_SELECTION = 5,
		BUF_EDGES_SELECTION = 6
	};

	enum LightType {
		AMBIANT = 0,
		POINT_LIGHT = 1,
		DIRECTIONNAL = 2,
		SPOT_LIGHT = 3,
		SILOUHETTE = 4
	};

	class MeshRenderer
	{
	private:
		Mesh* mesh; // Mesh in half-edge structure

		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;

		// Mesh as vertices + faces + normals arrays for better performances
		// Reloaded when the mesh is modified
		std::vector<GLfloat> vertices;
		std::vector<GLuint> faces;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> verticesNormals;
		std::vector<GLfloat> facesNormals;
		std::vector<GLint> verticesSelection;
		std::vector<GLint> edgesSelection;

		GLuint vao;
		GLuint buffers[NB_BUFFER]; // Buffers for each previous arrays
		bool verticesNormalsUpdated;
		bool facesNormalsUpdated;
						

		GLuint program; // Shader program

		// Shaders variables
		GLuint projectionMatrixLoc;
		GLuint viewMatrixLoc;
		GLuint modelMatrixLoc;
		GLuint meshColorLoc;
			
		glm::vec3 cameraEye; // Position of the camera
		glm::vec3 cameraUp; // Camera orientation
		glm::vec3 cameraForward; // Camera direction

		// Viewer parameters
		int viewportWidth;
		int viewportHeight;
		float fovy;
		float zNear;
		float zFar;
		RenderMode renderMode;

		// Light parameters
		LightType lightType;
		glm::vec4 lightColor;
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		float lightAngle;
			
		GLuint lightTypeLoc;
		GLuint lightColorLoc;
		GLuint lightPositionLoc;
		GLuint lightDirectionLoc;
		GLuint lightAngleLoc;

		void Clean();

		// Display functions
		void DisplayMesh();
		void DisplayWireframe();
		void DisplayVertices();
		void DisplayFacesNormals();
		void DisplayVerticesNormals();

		void DrawMesh(unsigned int drawMode, GLuint program, glm::vec4 color, LightType lightType);
		void DrawVertices(BufferId buffer, GLuint program, glm::vec4 color, LightType lightType);
		void DrawNormals(BufferId buffer, unsigned int size, GLuint program, glm::vec4 color, LightType lightType);

		void UpdateFacesNormals();
		void UpdateVerticesNormals();
		void GenerateFacesNormals();
		void GenerateVerticesNormals();

	public:
		MeshRenderer(int viewportWidth, int viewportHeight, Mesh* mesh = NULL);
		~MeshRenderer();

		void Init(); // Initialize opengl context			

		void SetMesh(Mesh* mesh);
		Mesh* GetMesh();

		void SetTranslation(glm::vec3 translation);
		glm::vec3 GetTranslation();
		void SetRotation(glm::vec3 rotation);
		glm::vec3 GetRotation();
		void SetScale(glm::vec3 scale);
		glm::vec3 GetScale();

		// Must be called whenever the mesh connectivity has changed (new or removes vertices/edge/face)
		// Update vertices, faces and normals arrays
		void UpdateMeshConnectivity();

		// UpdateMeshGeomtry() methods must be called whenever the mesh geometry has changed (i.e. some vertices' positions have changed)
		// Update all vertices
		void UpdateMeshGeometry();
		// Update a subset of all vertices
		void UpdateMeshGeometry(std::vector<Vertex*> vertices);
		// Update a single vertex
		void UpdateMeshGeometry(Vertex* vertex);

		// Display the model by sending the vertices, faces and normals arrays to the graphic card
		void Display();

		void SetRenderMode(RenderMode renderMode);

		void SetLightType(LightType lightType);
		void SetLightColor(glm::vec4 color);
		void SetLightPosition(glm::vec3 position);
		void SetLightDirection(glm::vec3 direction);
		void SetLightAngle(float angle);

		void SetVertexSelected(int index, bool selected);
		void ClearVertexSelection();
		void SetEdgeSelected(int index, bool selected);
		void ClearEdgeSelection();
			
		int GetViewportWidth();
		int GetViewportHeight();
		float GetFovy();
		float GetZNear();
		float GetZFar();
		glm::vec3 GetCameraEye();
		glm::vec3 GetCameraUp();
		glm::vec3 GetCameraForward();

		glm::vec3 cameraRotation;
		glm::vec3 cameraPosition;

		void SetViewPort(int viewportWidth, int viewportHeight);
		void Rotate(float x, float y); // Rotate the camera around the x axis by x degrees and around the y axis by y degrees
		void Translate(float x, float y);
		void Zoom(float value);
	};
}