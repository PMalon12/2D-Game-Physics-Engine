#pragma once
#include "Include.h"
#include "Followable.h"
#include "Transform.h"

#define ZFar 100.0
#define ZNear -0.9

class Camera
{
	friend class CameraManager;
public:
	Camera(unsigned int pId, std::string pName);
	~Camera() 
	{
		glDeleteBuffers(1, &ubo);
	}

	void setCenter(GLfloat x, GLfloat y);
	void setCenter(vec2f c);
	void setSize(GLdouble width, GLdouble height);
	void setSize(vec2d pSize);
	void zoom(GLdouble pFactor);
	void rotate(float pRot);
	void setRotation(float pRot);
	void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	void setActive();
	void update(Time& dt);

	std::string getName() { return name; }

	void setFollowSpeed(float pV) { followSpeed = pV; }
	float getFollowSpeed() { return followSpeed; }

	void setSmoothFollow(bool pV) { smoothCamFollow = pV; }
	bool getSmoothFollow() { return smoothCamFollow; }

	void setContraintRect(frect pConstraint)
	{
		constrainCam = true;
		constraint = pConstraint;
	}

	void toggleConstrain()
	{
		constrainCam = !constrainCam;
	}

	void setConstrainCam(bool pVal)
	{
		constrainCam = pVal;
	}

	void setMinZoom(float min) { minZoom = min; }
	void setMaxZoom(float max) { maxZoom = max; }
	void removeZoomConstraint() { minZoom = FLT_MAX; maxZoom = 0; }

	vec2d getCenter()
	{ 
		return vec2d(view.center.x, view.center.y); 
	}

	vec2d getSize()
	{ 
		return vec2d(sizeX, sizeY);
	}

	void setFollowTarget(Transformable* pFollowTarget) { followTarget = pFollowTarget; setCenter(followTarget->getPosition()); }
	Transformable* getFollowTarget() { return followTarget; }

	glm::fmat4 getProjView() { return uniformData.projViewMat; }
	float* getProjViewArray() { return glm::value_ptr(uniformData.projViewMat); }

	glm::fmat4 getViewMat() { return viewMat; }
	float* getViewMatArray() { return glm::value_ptr(viewMat); }

	glm::fmat4 getProjMat() { return projMat; }
	float* getProjMatArray() { return glm::value_ptr(projMat); }

	glm::fvec4 getViewport() { return uniformData.viewport; }

	frect getWorldSpaceViewport()
	{
		frect vp;
		vp.bot = getCenter().y - (getSize().y / 2);
		vp.left = getCenter().x - (getSize().x / 2);
		vp.height = getSize().y;
		vp.width = getSize().x;
		return vp;
	}

	struct View
	{
		View() {}
		View(glm::fvec3 pEye, glm::fvec3 pCenter, glm::fvec3 pUp) : eye(pEye), center(pCenter), up(pUp) {}
		View& operator=(const View pNewView)
		{
			eye = pNewView.eye;
			center = pNewView.center;
			up = pNewView.up;
			return *this;
		}
		glm::fvec3 eye;
		glm::fvec3 center;
		glm::fvec3 up;
	};

	const View* getView() { return &view; }
	void setView(View& pView) 
	{ 
		view = pView; 
	}

	void setDefault()
	{
		followTarget = nullptr;
		followSpeed = 100.f;
		view = DefaultView;
		smoothCamFollow = false;
		constrainCam = false;
		zoomFactor = 1.f;
		targetZoomFactor = 1.f;
	}

	static View DefaultView;

private:

	View view;
	GLdouble sizeX, sizeY;

	//Viewport data
	GLint x, y;
	GLsizei width, height;
	float rotation;
	float targetRotation;

	//Other data
	std::string name;
	unsigned int id;
	Transformable* followTarget;
	bool smoothCamFollow;
	float followSpeed;

	frect constraint;
	bool constrainCam;

	float zoomFactor;
	float targetZoomFactor;
	float maxZoom, minZoom;

	struct UniformData
	{
		glm::fmat4 projViewMat;
		glm::ivec4 viewport;
	} uniformData;

	glm::fmat4 projMat;
	glm::fmat4 viewMat;

	GLuint ubo;
	GLuint uboBlockIndex;
};

class CameraManager
{
public:
	CameraManager() {}
	~CameraManager() {}

	void init()
	{
		DefaultCamera = new Camera(0, "DefaultCamera");
		cameras.insert(std::pair<unsigned int, Camera*>(0, DefaultCamera));
		camNames.insert(std::pair<std::string, unsigned int>("DefaultCamera", 0));
		DefaultCamera->setMinZoom(1); DefaultCamera->setMaxZoom(1);
		//DefaultCamera->update();
	}

	Camera* getCamera(unsigned int pId);
	Camera* getCamera(std::string pName);
	void setCurrCamera(unsigned int pId);
	void setCurrCamera(std::string pName);
	void setCurrCamera(Camera* cam);
	Camera* getCurrCamera() { return currCamera; }

	Camera* addCamera(std::string pName = std::string(""))
	{
		if (pName.length() == 0)
		{
			pName = std::to_string(cameras.size());
		}
		auto newCamera = new Camera(cameras.size(), pName);
		cameras.insert(std::pair <unsigned int, Camera*>(cameras.size(), newCamera));
		camNames.insert(std::pair <std::string, unsigned int>(pName, cameras.size()-1));
		return newCamera;
	}

	void removeCamera(unsigned int pId)
	{
		for (auto itr = camNames.begin(); itr != camNames.end(); ++itr)
		{
			if (itr->second == pId)
			{
				camNames.erase(itr);
			}
		}

		delete cameras.at(pId);
		cameras.erase(pId);
	}

	void removeCamera(std::string pCamName)
	{
		delete cameras.find(camNames.find(pCamName)->second)->second;

		cameras.erase(camNames.find(pCamName)->second);
		
		camNames.erase(pCamName);
	}

	void removeCamera(Camera* pCameraPtr)
	{
		for (auto itr = cameras.begin(); itr != cameras.end(); ++itr)
		{
			if (itr->second == pCameraPtr)
			{
				cameras.erase(itr);
				delete pCameraPtr;
			}
		}
	}

	static Camera* DefaultCamera;

private:

	std::map<std::string, unsigned int> camNames;
	std::map<unsigned int, Camera*> cameras;
	Camera* currCamera;
};