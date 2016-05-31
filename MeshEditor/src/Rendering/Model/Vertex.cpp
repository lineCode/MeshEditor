#include "Rendering\Model\Vertex.h"
#include "Rendering\Model\HalfEdge.h"
#include "Rendering\Model\Face.h"
#include <vector>

using namespace Rendering::Model;
using namespace glm;

Vertex::Vertex()
{
	originOf = NULL;
}

Vertex::Vertex(glm::vec3 pos) : position(pos)
{
	Vertex();
}

Vertex::~Vertex()
{
}

void Vertex::ListFaces(std::vector<Face*> &faces)
{
	faces.clear();
	if (originOf != NULL)
	{
		HalfEdge* e = originOf;
		vec3 normal;

		do
		{
			faces.push_back(e->adjacentFace);
			e = e->prev->twin;
		} while (e != originOf);
	}
}

int Vertex::CountFaces()
{
	std::vector<Face*> faces;
	ListFaces(faces);
	return faces.size();
}

void Vertex::ComputeNormal()
{	
	std::vector<Face*> faces;
	ListFaces(faces);
	vec3 sum;

	for (unsigned int i = 0; i < faces.size(); i++)
		sum += faces[i]->normal;

	sum /= faces.size();
	normal = normalize(sum);
}