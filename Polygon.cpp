#pragma once
#include "Game.h"
#include "Polygon.h"
#include "Utility.cpp"

Mesh::Mesh() : textureScale(1, 1)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader = Game::shaderManager.getShader("minTex");

	posAttrib = glGetAttribLocation(shader->getID(), "position");
	texAttrib = glGetAttribLocation(shader->getID(), "texCoord");

	GLuint colourUniform = glGetUniformLocation(shader->getID(), "colour");

	//float col[] = { 0.820, 0.066, 0.255, 1.000,
	//				0.000, 0.694, 0.349, 1.000,
	//				0.000, 0.682, 0.859, 1.000,
	//				0.953, 0.467, 0.208, 1.000,
	//				1.000, 0.769, 0.145, 1.000 };


	//float col[] = { 0.05, 0.05, 0.05, 1.000,
	//				0.1, 0.1, 0.1, 1.000,
	//				0.15, 0.15, 0.15, 1.000 };

	//void* c = col + ((Game::rng() % 3)  * sizeof(float));

	//std::memcpy(&r, c, 4 * sizeof(float));

	r = 0.05; b = 0.05; g = 0.05; a = 1.0;

	glUniform4f(colourUniform, r, g, b, a);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	registerVAO(VERT_3D_TEX, shader->getID());

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(std::ifstream& ifs) : textureScale(1, 1)
{
	loadFromStream(ifs);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader = Game::shaderManager.getShader("minTex");

	posAttrib = glGetAttribLocation(shader->getID(), "position");
	texAttrib = glGetAttribLocation(shader->getID(), "texCoord");

	GLuint colourUniform = glGetUniformLocation(shader->getID(), "colour");

	//float col[] = { 0.820, 0.066, 0.255, 1.000,
	//	0.000, 0.694, 0.349, 1.000,
	//	0.000, 0.682, 0.859, 1.000,
	//	0.953, 0.467, 0.208, 1.000,
	//	1.000, 0.769, 0.145, 1.000 };

	//float col[] = { 0.8, 0.9, 0.8, 1.000,
	//	0.7, 0.7, 0.7, 1.000,
	//	0.6, 0.6, 0.6, 1.000 };

	//void* c = col + ((Game::rng() % 3)  * sizeof(float));

	//std::memcpy(&r, c, 4 * sizeof(float));

	glUniform4f(colourUniform, r, g, b, a);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	registerVAO(VERT_3D_TEX, shader->getID());

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::loadFromStream(std::ifstream& ifs)
{
	ifs.read((char*)&r, sizeof(float) * 4);
	char texName[16];
	ifs.read(texName, sizeof(texName));

	if (texName[0] == '\0')
	{
		texture = Game::textureManager.getTexture("d");
	}
	else
	{
		texture = Game::textureManager.getTexture(texName);
	}

	//char shaderName[16];
	//ifs.read(shaderName, 16);
	//std::string shdr; shdr.assign(shaderName, shaderName + 16);
	shader = Game::shaderManager.getShader("min");
}

void Mesh::saveToStream(std::ofstream& ofs)
{
	ofs.write((char*)&r, sizeof(float) * 4);
	if (texture == nullptr)
	{
		const char n[16] = { '\0' };
		ofs.write(n, sizeof(n));
	}
	else
	{
		ofs.write(texture->getName(), 16);
	}
	//ofs.write(shader->getName(), 16);
}

void Triangle::draw()
{
}

TriangleMesh::TriangleMesh()
{
}

TriangleMesh::TriangleMesh(int pNOTriangles, Triangle* pTriangles, Transform& t)
{
	edgeVertData = nullptr;
	trianglesRawSize = pNOTriangles * 6;
	trianglesRaw = new float[trianglesRawSize];

	textureCoordsRawSize = pNOTriangles * 6;
	textureCoordsRaw = new float[trianglesRawSize];

	for (int i = 0; i < pNOTriangles; ++i)
	{
		pTriangles[i].vertices[0].pos.transform(t.getTransform());
		pTriangles[i].vertices[1].pos.transform(t.getTransform());
		pTriangles[i].vertices[2].pos.transform(t.getTransform());

		int o = i * 6;
		trianglesRaw[0 + o] = pTriangles[i].vertices[0].x;
		trianglesRaw[1 + o] = pTriangles[i].vertices[0].y;
		textureCoordsRaw[0 + o] = pTriangles[i].vertices[0].s0;
		textureCoordsRaw[1 + o] = pTriangles[i].vertices[0].t0;

		trianglesRaw[2 + o] = pTriangles[i].vertices[1].x;
		trianglesRaw[3 + o] = pTriangles[i].vertices[1].y;
		textureCoordsRaw[2 + o] = pTriangles[i].vertices[1].s0;
		textureCoordsRaw[3 + o] = pTriangles[i].vertices[1].t0;

		trianglesRaw[4 + o] = pTriangles[i].vertices[2].x;
		trianglesRaw[5 + o] = pTriangles[i].vertices[2].y;
		textureCoordsRaw[4 + o] = pTriangles[i].vertices[2].s0;
		textureCoordsRaw[5 + o] = pTriangles[i].vertices[2].t0;
	}

	vboSize = pNOTriangles * 6;
	totTris = pNOTriangles;

	bufferData();

	buildConvexHull();
}

void TriangleMesh::draw()
{
	draw(Transform());
}

void TriangleMesh::draw(Transform& model, vec2f texOffset, GLenum S, GLenum T)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader = Game::shaderManager.getShader("minTex");
	shader->use();

	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		auto texoffs = glGetUniformLocation(shader->getID(), "texCoordOffset");
		glUniform2f(texoffs, texOffset.x, texOffset.y);
	}
	else
		glBindTexture(GL_TEXTURE_2D, Game::textureManager.getTexture("d")->getID());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	auto col = glGetUniformLocation(shader->getID(), "colour");
	glUniform4f(col, r, g, b, a);

	auto modelUniform = glGetUniformLocation(shader->getID(), "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model.getGLTransform()));

	glDrawArrays(GL_TRIANGLES, 0, vboSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void TriangleMesh::drawConvexHull(Transform& t)
{
	if (Game::getOption(DRAW_CONVEX_HULL))
	{
		glBindBuffer(GL_ARRAY_BUFFER, hullVBO);

		shader = Game::shaderManager.getShader("min");
		shader->use();

		auto p = glGetAttribLocation(shader->getID(), "position");
		glVertexAttribPointer(p, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(p);

		auto col = glGetUniformLocation(shader->getID(), "colour");
		glUniform4f(col, 1, 0, 0, 1.0f);

		auto model = glGetUniformLocation(shader->getID(), "model");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(t.getGLTransform()));

		glDrawArrays(GL_LINE_LOOP, 0, hullVBOSize);

		glDisableVertexAttribArray(glGetAttribLocation(shader->getID(), "position"));
	}
}

void TriangleMesh::bufferData()
{
	//int bufferSize = trianglesRawSize * GL_VERTEX_SIZE;
	int bufferSize = totTris * GL_TRIANGLE_SIZE;
	float* bufferData = new float[bufferSize];

	for (int i = 0; i < totTris; ++i)
	{
		bufferData[(GL_TRIANGLE_SIZE * i) + 0] = trianglesRaw[(i * 6) + 0];
		bufferData[(GL_TRIANGLE_SIZE * i) + 1] = trianglesRaw[(i * 6) + 1];
		bufferData[(GL_TRIANGLE_SIZE * i) + 2] = 0.f;

		bufferData[(GL_TRIANGLE_SIZE * i) + 3] = textureCoordsRaw[(i * 6) + 0];
		bufferData[(GL_TRIANGLE_SIZE * i) + 4] = textureCoordsRaw[(i * 6) + 1];

		bufferData[(GL_TRIANGLE_SIZE * i) + 5] = trianglesRaw[(i * 6) + 2];
		bufferData[(GL_TRIANGLE_SIZE * i) + 6] = trianglesRaw[(i * 6) + 3];
		bufferData[(GL_TRIANGLE_SIZE * i) + 7] = 0.f;

		bufferData[(GL_TRIANGLE_SIZE * i) + 8] = textureCoordsRaw[(i * 6) + 2];
		bufferData[(GL_TRIANGLE_SIZE * i) + 9] = textureCoordsRaw[(i * 6) + 3];

		bufferData[(GL_TRIANGLE_SIZE * i) + 10] = trianglesRaw[(i * 6) + 4];
		bufferData[(GL_TRIANGLE_SIZE * i) + 11] = trianglesRaw[(i * 6) + 5];
		bufferData[(GL_TRIANGLE_SIZE * i) + 12] = 0.f;

		bufferData[(GL_TRIANGLE_SIZE * i) + 13] = textureCoordsRaw[(i * 6) + 4];
		bufferData[(GL_TRIANGLE_SIZE * i) + 14] = textureCoordsRaw[(i * 6) + 5];
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), bufferData, GL_STATIC_DRAW);

	delete bufferData;
}

void TriangleMesh::reBufferData()
{
	//int bufferSize = trianglesRawSize * GL_VERTEX_SIZE;
	int bufferSize = totTris * GL_TRIANGLE_SIZE;
	float* bufferData = new float[bufferSize];

	for (int i = 0; i < totTris; ++i)
	{
		bufferData[(GL_TRIANGLE_SIZE * i) + 0] = trianglesRaw[(i * 6) + 0];
		bufferData[(GL_TRIANGLE_SIZE * i) + 1] = trianglesRaw[(i * 6) + 1];
		bufferData[(GL_TRIANGLE_SIZE * i) + 2] = 0.f;

		bufferData[(GL_TRIANGLE_SIZE * i) + 3] = textureCoordsRaw[(i * 6) + 0];
		bufferData[(GL_TRIANGLE_SIZE * i) + 4] = textureCoordsRaw[(i * 6) + 1];

		bufferData[(GL_TRIANGLE_SIZE * i) + 5] = trianglesRaw[(i * 6) + 2];
		bufferData[(GL_TRIANGLE_SIZE * i) + 6] = trianglesRaw[(i * 6) + 3];
		bufferData[(GL_TRIANGLE_SIZE * i) + 7] = 0.f;

		bufferData[(GL_TRIANGLE_SIZE * i) + 8] = textureCoordsRaw[(i * 6) + 2];
		bufferData[(GL_TRIANGLE_SIZE * i) + 9] = textureCoordsRaw[(i * 6) + 3];

		bufferData[(GL_TRIANGLE_SIZE * i) + 10] = trianglesRaw[(i * 6) + 4];
		bufferData[(GL_TRIANGLE_SIZE * i) + 11] = trianglesRaw[(i * 6) + 5];
		bufferData[(GL_TRIANGLE_SIZE * i) + 12] = 0.f;

		bufferData[(GL_TRIANGLE_SIZE * i) + 13] = textureCoordsRaw[(i * 6) + 4];
		bufferData[(GL_TRIANGLE_SIZE * i) + 14] = textureCoordsRaw[(i * 6) + 5];
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(float), bufferData);

	delete bufferData;
}

void TriangleMesh::transformData(Transform & t)
{
	vec2f* rawTriVerts = reinterpret_cast<vec2f*>(trianglesRaw);

	for (int i = 0; i < totTris; ++i)
	{
		int o = i * 3;
		rawTriVerts[0 + o].transform(t.getTransform());
		rawTriVerts[1 + o].transform(t.getTransform());
		rawTriVerts[2 + o].transform(t.getTransform());
	}

	bufferData();

	buildConvexHull();
}
