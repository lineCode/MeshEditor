#pragma once

#include <vector>
#include "GLM/glm.hpp"
#include "GL/gl3w.h"

namespace Rendering
{
	class Mesh;

	class MeshConverter
	{
	public:
		// Generate a mesh in half edge structure from an array of vertices and an array of indices
		// @param mesh : The mesh to construct
		// @param vertices : The list of vertices positions
		// @param faces : The list of faces. Each face is represented by an array of n integer (n the number of vertex per face). 
		//				  Each integer is an index refering to the vertices array
		static void ArrayToHalfEdgeStructure(Mesh& mesh, const std::vector<glm::vec3>& vertices, const std::vector<std::vector<unsigned int>>& faces);

		// Generate arrays for vertices, faces and normals from a mesh in half edge structure
		// @param mesh : The mesh to convert
		// @param vertices : The list of vertices positions
		// @param faces : The list of faces, reprensented by a continus set of vertices indices
		// @param normals : faces' normals
		static void HalfEdgeStructureToArray(const Mesh& mesh, std::vector<GLfloat>& vertices, std::vector<GLuint>& faces, std::vector<GLfloat>& normals);
	};
}
