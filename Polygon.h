#pragma once
#include "Include.h"
#include "Vertex.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Colour.h"

#define GL_VERTEX_SIZE 5
#define MEM_VERTEX_SIZE 4
#define GL_TRIANGLE_SIZE GL_VERTEX_SIZE*3
#define MEM_TRIANGLE_SIZE MEM_VERTEX_SIZE*3

#define CrossProd(v,w) float((v.x * w.y) - (v.y * w.x))

class Triangle
{
public:
	Triangle() {}
	Triangle(Vertex pA, Vertex pB, Vertex pC)
	{
		setVertices(pA, pB, pC);
	}
	Triangle(Vertex2D pA, Vertex2D pB, Vertex2D pC)
	{
		setVertices(pA, pB, pC);
	}
	~Triangle() {}

	Vertex2D* getVertices()
	{
		return vertices;
	}

	void draw();

	void setVertices(Vertex pA, Vertex pB, Vertex pC)
	{
		vertices[0].x = pA.x;
		vertices[0].y = pA.y;
		vertices[0].s0 = pA.s0;
		vertices[0].t0 = pA.t0;

		vertices[1].x = pB.x;
		vertices[1].y = pB.y;
		vertices[1].s0 = pA.s0;
		vertices[1].t0 = pA.t0;

		vertices[2].x = pC.x;
		vertices[2].y = pC.y;
		vertices[2].s0 = pA.s0;
		vertices[2].t0 = pA.t0;
	}
	void setVertices(Vertex2D pA, Vertex2D pB, Vertex2D pC)
	{
		vertices[0].x = pA.x;
		vertices[0].y = pA.y;
		vertices[0].s0 = pA.s0;
		vertices[0].t0 = pA.t0;

		vertices[1].x = pB.x;
		vertices[1].y = pB.y;
		vertices[1].s0 = pB.s0;
		vertices[1].t0 = pB.t0;

		vertices[2].x = pC.x;
		vertices[2].y = pC.y;
		vertices[2].s0 = pC.s0;
		vertices[2].t0 = pC.t0;
	}

	void saveToStream(std::ofstream& ofs)
	{
		//ofs.write((char*)(&vertices[0]), sizeof(Vertex) * 3);
		ofs.write((char*)(&vertices[0]), sizeof(Vertex2D) * 3);
	}

	void loadFromStream(std::ifstream& ifs)
	{
		//ofs.write((char*)(&vertices[0]), sizeof(Vertex) * 3);
		ifs.read((char*)(&vertices[0]), sizeof(Vertex2D) * 3);
	}

	Vertex2D vertices[3];

private:

	//mutable Vertex vertices[3];
	
};

class Mesh
{
public:
	Mesh();
	Mesh(std::ifstream& ifs);
	~Mesh() {
		glDeleteBuffers(1, &vbo); 
	}

	virtual void draw() = 0;

	long totTris;

	float r, g, b, a;
	Shader* shader;

	virtual void transformData(Transform& t) {}

	GLuint hullVBO;
	GLuint hullVBOSize;

	GLuint vbo;
	GLuint vboSize;
	GLuint vao;

	GLint posAttrib;
	GLint colAttrib;
	GLint texAttrib;

	void saveToStream(std::ofstream& ofs);

	Texture* texture;
	vec2f textureScale;
	bool clipTextureToWorldOrigin;

	void setColour(Colour c)
	{
		r = c.r; g = c.g; b = c.b; a = c.a;
	}

	template<class T>
	void setColour(vec4<T> c)
	{
		r = c.x; g = c.y; b = c.z; a = c.w;
	}

private:

	void loadFromStream(std::ifstream& ifs);
};

class TriangleMesh : public Mesh
{
public:
	TriangleMesh();
	TriangleMesh(std::ifstream& ifs) : Mesh(ifs)
	{
		loadFromStream(ifs);
	}
	TriangleMesh(int pNOTriangles, Triangle* pTriangles, Transform& t = Transform());
	~TriangleMesh()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glDeleteBuffers(1, &vbo);

		glDeleteBuffers(1, &hullVBO);
		glDeleteVertexArrays(1, &vao);

		delete[] trianglesRaw;
		delete[] edgeVertData;
		delete[] textureCoordsRaw;
	}

	vec2f getSupportPoint(vec2f direction)
	{
		float* set = trianglesRaw;
		int setSize = trianglesRawSize;

		vec2f next(set[0], set[1]);

		float max = next.dot(direction);
		int maxI = 0;
		for (size_t i = 2; i < (setSize); i += 2)
		{
			next = vec2f(set[i], set[i + 1]);
			float dot = next.dot(direction);
			if (dot > max)
			{
				max = dot;
				maxI = i;
			}
		}

		vec2f ret(set[maxI], set[maxI + 1]);
		return ret;
	}

	void draw();
	//void draw(Transform& t, GLenum S = GL_CLAMP_TO_EDGE, GLenum T = GL_CLAMP_TO_EDGE);
	void draw(Transform& model, vec2f texOffset = vec2f(0,0), GLenum S = GL_CLAMP_TO_EDGE, GLenum T = GL_CLAMP_TO_EDGE);
	void drawConvexHull(Transform&);

	void bufferData();
	void reBufferData();
	void updateTextureCoords(vec2f pos)
	{
		vec2f* tex = reinterpret_cast<vec2f*>(textureCoordsRaw);
		vec2f* vert = reinterpret_cast<vec2f*>(trianglesRaw);
		for (uint32_t i = 0; i < totTris; ++i)
		{
			static const auto round = [](vec2f num, vec2f gran) -> vec2f {
				vec2f signs(num.x > 0 ? 1 : -1, num.y > 0 ? 1 : -1);
				num *= signs;
				num /= gran;
				vec2f fixed(int(ceil(num.x)), int(ceil(num.y)));
				num = fixed * gran;
				num *= signs;
				return num;
			};

			int o = i * 3;

			vec2f texRealSize = texture->getSize() / textureScale;

			tex[0 + o] = (vert[0 + o] + pos * clipTextureToWorldOrigin) / texRealSize;
			tex[1 + o] = (vert[1 + o] + pos * clipTextureToWorldOrigin) / texRealSize;
			tex[2 + o] = (vert[2 + o] + pos * clipTextureToWorldOrigin) / texRealSize;

		}
		reBufferData();
	}
	inline void updateTextureCoords(float scale, vec2f pos)
	{
		updateTextureCoords(vec2f(scale, scale), pos);
	}
	inline void updateTextureCoords(vec2f scale, vec2f pos)
	{
		textureScale = scale;
		if (textureScale.x == 0)
			textureScale.x = 1;
		if (textureScale.y == 0)
			textureScale.y = 1;
		updateTextureCoords(pos);
	}

	virtual void transformData(Transform& t);

	void saveToStream(std::ofstream& ofs)
	{
		Mesh::saveToStream(ofs);
		ofs.write((char*)&vboSize, sizeof(vboSize));
		ofs.write((char*)&totTris, sizeof(totTris));

		ofs.write((char*)&trianglesRawSize, sizeof(trianglesRawSize));
		ofs.write((char*)trianglesRaw, trianglesRawSize * sizeof(float));

		ofs.write((char*)&textureCoordsRawSize, sizeof(textureCoordsRawSize));
		ofs.write((char*)textureCoordsRaw, textureCoordsRawSize * sizeof(float));
	}

	void loadFromStream(std::ifstream& ifs)
	{
		ifs.read((char*)&vboSize, sizeof(vboSize));
		ifs.read((char*)&totTris, sizeof(totTris));

		ifs.read((char*)&trianglesRawSize, sizeof(trianglesRawSize));

		trianglesRaw = new float[trianglesRawSize];

		ifs.read((char*)trianglesRaw, trianglesRawSize * sizeof(float));

		ifs.read((char*)&textureCoordsRawSize, sizeof(textureCoordsRawSize));

		textureCoordsRaw = new float[textureCoordsRawSize];

		ifs.read((char*)textureCoordsRaw, textureCoordsRawSize * sizeof(float));

		buildConvexHull();

		bufferData();
	}

	void buildConvexHull()
	{
		if (edgeVertData != nullptr) return;

		glGenBuffers(1, &hullVBO);

		std::vector<vec2f> cvx; cvx.reserve(30);

		vec2f maxX, minX;

		maxX = getSupportPoint(vec2f(1, 0));
		minX = getSupportPoint(vec2f(-1, 0));

		std::function<vec2f(vec2f,vec2f)> getLeft = [&](vec2f left, vec2f right) -> vec2f
		{
			vec2f supPoint = getSupportPoint((right-left).getNormal());
			if (supPoint == left || supPoint == right) { cvx.push_back(left); return right; }

			vec2f newRight = supPoint;
			vec2f newLeft = getLeft(left, newRight);
			return getLeft(newLeft, right);
		};

		//BUILD HULL
		getLeft(minX, maxX);
		getLeft(maxX, minX);

		//COPY TO FLOAT ARRAY
		edgeVertDataSize = cvx.size() * 2;
		edgeVertData = new float[edgeVertDataSize];
		std::memcpy(edgeVertData, &cvx.front(), 8 * cvx.size());

		hullVBOSize = cvx.size();
		
		glBindBuffer(GL_ARRAY_BUFFER, hullVBO);
		glBufferData(GL_ARRAY_BUFFER, edgeVertDataSize * sizeof(float), edgeVertData, GL_STATIC_DRAW);
	}

	void setTextureCoord(int vertex, vec2f coord)
	{
		textureCoordsRaw[vertex * 2] = coord.x;
		textureCoordsRaw[(vertex * 2) + 1] = coord.y;
	}

	float* edgeVertData;
	unsigned int edgeVertDataSize;

	float* trianglesRaw;
	unsigned int trianglesRawSize;

	float* textureCoordsRaw;
	unsigned int textureCoordsRawSize;
};