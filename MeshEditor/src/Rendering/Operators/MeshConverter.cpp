#include "Rendering\Operators\MeshConverter.h"
#include "Rendering\Model\Vertex.h"
#include "Rendering\Model\HalfEdge.h"
#include "Rendering\Model\Face.h"
#include <map>

using namespace Rendering;
using namespace Rendering::Operators;
using namespace Rendering::Model;
using namespace std;

void MeshConverter::ArrayToHalfEdgeStructure(Model::Mesh *mesh, vector<glm::vec3> &vertices, vector<vector<unsigned int>> &faces)
{
	map<pair<unsigned int, unsigned int>, HalfEdge*> hmap; // Store if we already created the edge between two vertices
	mesh->Clean();
	
	// Create the vertices, set the position
	for (unsigned int i = 0; i < vertices.size(); i++)
	{ 
		mesh->vertices.push_back(new Vertex(vertices[i]));
	}

	// Create faces
	Face *face;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		// New face
		face = new Face();
		mesh->faces.push_back(face);

		vector<unsigned int> vertexIndices = faces[i]; // Get de vertex indices for the new face

		// Create the first half edge
		HalfEdge* firstEdge = new HalfEdge();
		firstEdge->adjacentFace = face;
		firstEdge->source = mesh->vertices[vertexIndices[0]];		

		face->adjacentHalfEdge = firstEdge; // Link the face to the first half edge
		mesh->vertices[vertexIndices[0]]->originOf = firstEdge; // Link the first vertex to the first half edge

		// Consolidate faces. Check whether the half edge's twin has already been created, e.g. If there is an edge beetween the vertices 3 and 5, 
		// and two faces sharing this edge, we will try to create first the half edge 3 to 5, and later the half edge 5 to 3 (or vice versa)
		// The second time we have to link the two half edges by settings their twins
		if (hmap.count(make_pair(vertexIndices[1], vertexIndices[0])) != 0)
		{
			firstEdge->twin = hmap[make_pair(vertexIndices[1], vertexIndices[0])];
			hmap[make_pair(vertexIndices[1], vertexIndices[0])]->twin = firstEdge;
		}

		mesh->halfEdges.push_back(firstEdge);
		hmap[make_pair(vertexIndices[0], vertexIndices[1])] = firstEdge;

		// Create intermediare edges
		HalfEdge* prev = firstEdge;
		HalfEdge* curr = NULL;
		unsigned int currIndex = 0;
		unsigned int nextIndex;
		unsigned int lastIndex = vertexIndices[vertexIndices.size() - 1];
		for (unsigned int j = 1; j < vertexIndices.size() - 1; j++)
		{
			currIndex = vertexIndices[j];
			nextIndex = vertexIndices[j + 1];

			// New halfedge
			curr = new HalfEdge();
			curr->prev = prev;
			curr->adjacentFace = face;
			curr->source = mesh->vertices[currIndex];

			prev->next = curr; // Set the next of previous half edge
			mesh->vertices[currIndex]->originOf = curr; // Set current vertex has origin of the new half edge

			// Consolidate faces
			if (hmap.count(make_pair(nextIndex, currIndex)) != 0)
			{
				curr->twin = hmap[make_pair(nextIndex, currIndex)];
				hmap[make_pair(nextIndex, currIndex)]->twin = curr;
			}

			mesh->halfEdges.push_back(curr);
			hmap[make_pair(currIndex, nextIndex)] = curr;
			prev = curr;
		}

		// Create the last edge
		HalfEdge* lastEdge = new HalfEdge();
		lastEdge->prev = curr;
		lastEdge->next = firstEdge;
		lastEdge->adjacentFace = face;
		lastEdge->source = mesh->vertices[lastIndex];

		// Consolidate faces
		if (hmap.count(make_pair(vertexIndices[0], lastIndex)) != 0)
		{
			lastEdge->twin = hmap[make_pair(vertexIndices[0], lastIndex)];
			hmap[make_pair(vertexIndices[0], lastIndex)]->twin = lastEdge;
		}

		mesh->halfEdges.push_back(lastEdge);
		hmap[make_pair(lastIndex, vertexIndices[0])] = lastEdge;

		firstEdge->prev = lastEdge;
		curr->next = lastEdge;
		mesh->vertices[lastIndex]->originOf = lastEdge;
	}

	mesh->Normalize();
}

void MeshConverter::HalfEdgeStructureToArray(Model::Mesh *mesh, vector<GLfloat> &vertices, vector<GLuint> &faces, std::vector<GLfloat> &normals)
{
	vertices.clear();
	faces.clear();
	normals.clear();

	// Add vertices
	int i = 0;
	for (vector<Vertex*>::iterator vertexIt = mesh->vertices.begin(); vertexIt != mesh->vertices.end(); vertexIt++)
	{
		vertices.push_back((*vertexIt)->position.x);
		vertices.push_back((*vertexIt)->position.y);
		vertices.push_back((*vertexIt)->position.z);
		normals.push_back((*vertexIt)->normal.x);
		normals.push_back((*vertexIt)->normal.y);
		normals.push_back((*vertexIt)->normal.z);
		(*vertexIt)->index = i++;
	}

	// Add faces
	vector<Vertex*> faceVertices;
	for (vector<Face*>::iterator faceIt = mesh->faces.begin(); faceIt != mesh->faces.end(); faceIt++)
	{
		(*faceIt)->ListVertices(faceVertices);
		for (vector<Vertex*>::iterator vertexIt = faceVertices.begin(); vertexIt != faceVertices.end(); vertexIt++)
		{
			faces.push_back((*vertexIt)->index);
		}
		/*normals.push_back((*faceIt)->normal.x);
		normals.push_back((*faceIt)->normal.y);
		normals.push_back((*faceIt)->normal.z);*/
	}
}