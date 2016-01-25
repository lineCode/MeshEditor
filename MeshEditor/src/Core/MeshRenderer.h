#pragma once

#include "Model\Mesh.h"
#include <vector>
#include "Dependencies\glew\glew.h"
#include "Dependencies\glm\glm.hpp"

namespace Core
{
	class MeshRenderer
	{
		private:
			Model::Mesh* mesh; // Mesh in half-edge structure

			// Mesh as vertices + faces + normals arrays for better performances
			// Reloaded when the mesh is modified
			std::vector<GLfloat> vertices;
			std::vector<GLuint> faces;
			std::vector<GLfloat> normals;

			GLuint buffers[3]; // Buffers for each previous arrays

			glm::vec3 cameraEye; // Position of the camera
			glm::vec3 cameraUp; // Camera orientation
			glm::vec3 cameraForward; // Camera direction

			// Viewer parameters
			int viewportWidth;
			int viewportHeight;
			float fovy;
			float zNear;
			float zFar;

			GLuint program; // Shader program
			
			// Shaders variables
			GLuint projection_matrix_loc;
			GLuint view_matrix_loc;			

		public:	
			MeshRenderer(int viewportWidth, int viewportHeight, Model::Mesh* mesh = NULL);
			~MeshRenderer();

			void Init(); // Initialize opengl context

			void SetMesh(Model::Mesh* mesh);
			Model::Mesh* GetMesh();
			
			// Must be called whenever the mesh structure has changed.
			// Update vertices, faces and normals arrays
			void Update(); 

			// Display the model by sending the vertices, faces and normals arrays to the graphic card
			void Display();

			// Rotate the model around the x axis by x degrees and around the y axis by y degrees
			void Rotate(float x, float y);
	};
}
