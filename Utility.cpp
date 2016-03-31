#pragma once
#include "Utility.h"
#include "Vector.h"
#include "Colour.h"
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Polygon.h"

inline float lerp(float v0, float v1, float t)
{
	return ((1 - t)*v0) + (t*v1);
}

inline vec2f vlerp(vec2f v0, vec2f v1, float t)
{
	return vec2f(lerp(v0.x, v1.x, t), lerp(v0.y, v1.y, t));
}

inline Colour clerp(Colour v0, Colour v1, float t)
{
	return Colour(((1 - t)*v0.r) + (t*v1.r), ((1 - t)*v0.g) + (t*v1.g), ((1 - t)*v0.b) + (t*v1.b), ((1 - t)*v0.a) + (t*v1.a));
}

inline TriangleMesh* createRegularPolygon(int numEdges, float rad, vec2f texSize = vec2f(1,1), Transform& t = Transform())
{
	const int num = numEdges;
	Triangle* trisss = new Triangle[num];

	for (int i = 0; i < num; ++i)
	{
		vec2f nextA(rad*std::sin((-i*PI * 2) / num), rad*std::cos((-i*PI * 2) / num));
		vec2f nextB(rad*std::sin(((-i + 1)*PI * 2) / num), rad*std::cos(((-i + 1)*PI * 2) / num));
		vec2f nAN(nextA + rad); nAN = nAN / (rad * 2);
		vec2f nBN(nextB + rad); nBN = nBN / (rad * 2);
		trisss[i].setVertices(Vertex2D(0, 0, 0.5, 0.5),
			Vertex2D(nextA.x, nextA.y, nAN.x, nAN.y),
			Vertex2D(nextB.x, nextB.y, nBN.x, nBN.y));
	}
	auto mesh = new TriangleMesh(num, trisss, Transform().scale(t.getScale()));
	return mesh;
}

inline void registerVAO(GLenum vertexType, GLuint shaderProgram)
{
	switch (vertexType)
	{
	case(VERT_2D) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);
		break;
	}
	case(VERT_2D_TEX) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		auto texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(texAttrib);

		break;
	}
	case(VERT_2D_TEX_COL) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		auto texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(texAttrib);

		auto colAttrib = glGetAttribLocation(shaderProgram, "colour");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
		glEnableVertexAttribArray(colAttrib);

		break;
	}
	case(VERT_2D_COL) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		auto colAttrib = glGetAttribLocation(shaderProgram, "colour");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(colAttrib);

		break;
	}
	case(VERT_3D) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		break;
	}
	case(VERT_3D_TEX) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		auto texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(texAttrib);

		break;
	}
	case(VERT_3D_TEX_COL) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		auto texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(texAttrib);

		auto colAttrib = glGetAttribLocation(shaderProgram, "colour");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(colAttrib);

		break;
	}
	case(VERT_3D_COL) :
	{
		auto posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		auto colAttrib = glGetAttribLocation(shaderProgram, "colour");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(colAttrib);

		break;
	}
	}
}
