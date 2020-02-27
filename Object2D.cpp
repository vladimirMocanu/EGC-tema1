#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		//indices.push_back(0);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateCircle(std::string name, float length, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = { 0, 1, 2 };

	int x = length, y = length;
	const float pi = 3.1415926f;
	const float dimensiune = pow(10, 2) * pi;
	int nrTriunghiuri = 5000;
	glVertex2f(x, y);
	float delta = dimensiune / nrTriunghiuri;

	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0)));

	for (int i = 0; i <= nrTriunghiuri; i++)
	{
		vertices.push_back(VertexFormat(glm::vec3(((x * cos(i * delta)) - (y * sin(i * delta))), ((x * sin(i * delta)) + (y * cos(i * delta))), 0), color));
	}


	for (int i = 0; i <= nrTriunghiuri; i++)
	{
		indices.push_back(i);
	}

	Mesh* circle = new Mesh(name);

	if (!fill) {
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		//indices.push_back(0);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}