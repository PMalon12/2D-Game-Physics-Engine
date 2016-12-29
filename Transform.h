#pragma once
#include <iostream>
#include <GL\glew.h>
#include <GL\GLU.h>
#include <glm\glm.hpp>
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\matrix_transform_2d.hpp"
#include "Vector.h"
#include <fstream>

class Transform
{
public:
	Transform();
	~Transform() {}

	inline glm::fmat3 getTransform()
	{
		if (needUpdate)
		{
			updateMatrix();
		}

		return transform;
	}

	inline glm::fmat4 getGLTransform()
	{
		if (needUpdate)
		{
			updateMatrix();
		}

		return glTransform;
	}

	inline glm::fmat3 getInverseTransform()
	{
		if (needUpdate)
		{
			updateMatrix();
		}

		return glm::inverse(transform);
	}

	inline glm::fmat4 getInverseGLTransform()
	{
		if (needUpdate)
		{
			updateMatrix();
		}

		return glm::inverse(glTransform);
	}

	inline Transform& setTranslation(vec2f p) { translation = p; needUpdate = true; return *this; }
	inline Transform& translate(vec2f p) { translation += p; needUpdate = true; return *this; }
	inline vec2f getTranslation() { return translation; }

	inline Transform& setRotation(float p) { rotation = p; needUpdate = true; return *this; }
	inline Transform& rotate(float p) { rotation += p; needUpdate = true; return *this; }
	inline float getRotation() { return rotation; }

	inline Transform& setScale(vec2f p) { scalem = p; needUpdate = true; return *this;}
	inline Transform& scale(vec2f p) { scalem *= p; needUpdate = true; return *this;}
	inline Transform& scale(float p) { scalem = scalem * p; needUpdate = true; return *this;}
	inline vec2f getScale() { return scalem; }

	inline Transform& setOrigin(vec2f p) { origin = p; return *this;}
	inline vec2f getOrigin() { return origin; }

	inline void saveToStream(std::ofstream& ofs)
	{
		ofs.write((char*)&translation, sizeof(translation));
		ofs.write((char*)&origin, sizeof(origin));
		ofs.write((char*)&rotation, sizeof(rotation));
		ofs.write((char*)&scalem, sizeof(scalem));
	}

	inline void loadFromStream(std::ifstream& ifs)
	{
		needUpdate = true;

		ifs.read((char*)&translation, sizeof(translation));
		ifs.read((char*)&origin, sizeof(origin));
		ifs.read((char*)&rotation, sizeof(rotation));
		ifs.read((char*)&scalem, sizeof(scalem));
	}

private:

	inline void updateMatrix()
	{
		needUpdate = false;

		glm::fmat3 ttranslate = glm::translate(glm::fmat3(), glm::fvec2(translation.x - origin.x, translation.y - origin.y));
		glm::fmat3 sscale = glm::scale(glm::fmat3(), glm::fvec2(scalem.x, scalem.y));
		glm::fmat3 rrotate = glm::rotate(glm::fmat3(), rotation);
		glm::fmat3 tttranslate = glm::translate(glm::fmat3(), glm::fvec2(origin.x, origin.y));

		transform = ttranslate * rrotate * sscale * tttranslate;

		glTransform[0].x = transform[0].x;
		glTransform[0].y = transform[0].y;
		glTransform[1].x = transform[1].x;
		glTransform[1].y = transform[1].y;

		glTransform[3].x = transform[2].x;
		glTransform[3].y = transform[2].y;
	}

	vec2f translation;
	vec2f origin;
	float rotation;
	vec2f scalem;
	
	bool needUpdate;
	glm::fmat3 transform;
	glm::fmat4 glTransform;
};

class Transformable
{
public:
	Transformable() {}
	~Transformable() {}

	virtual inline void setPosition(vec2f pPos) { transform.setTranslation(pPos); }
	inline void move(vec2f pMove) { transform.translate(pMove); }
	inline vec2f getPosition() { return transform.getTranslation(); }

	virtual inline void setRotation(float pRot) { transform.setRotation(pRot); }
	inline void rotate(float pRot) { transform.rotate(pRot); }
	inline float getRotation() { return transform.getRotation(); }

	inline void setScale(vec2f pScale) { transform.setScale(pScale); }
	inline void scale(vec2f pScale) { transform.scale(pScale); }
	inline void scale(float pScale) { transform.scale(pScale); }
	inline vec2f getScale() { return transform.getScale(); }

	inline void setOrigin(vec2f p) { transform.setOrigin(p); }
	inline vec2f getOrigin() { return transform.getOrigin(); }

	inline glm::fmat3 getTransformMat() { return transform.getTransform(); }
	inline glm::fmat4 getGLTransformMat() { return transform.getGLTransform(); }

	inline glm::fmat3 getInverseTransformMat() { return transform.getInverseTransform(); }
	inline glm::fmat4 getInverseGLTransformMat() { return transform.getInverseGLTransform(); }

	inline Transform& getTransform() { return transform; }

	inline void saveTransformToStream(std::ofstream& ofs) { transform.saveToStream(ofs); }
	inline void loadTransformFromStream(std::ifstream& ifs) { transform.loadFromStream(ifs); }

	inline void setTransfom(Transform p) { transform = p; }

private:

	Transform transform;

};