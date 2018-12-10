#pragma once
#include "Include.h"
#include "Camera Manager.h"
#include "Game.h"
#include "Utility.cpp"
#include "Shader.h"

Camera::Camera(unsigned int pId, std::string pName) : followTarget(nullptr), smoothCamFollow(false), constrainCam(false), zoomFactor(1.f), targetZoomFactor(1.f), followSpeed(100.f), sizeX(Game::ScreenSizeX), sizeY(Game::ScreenSizeY)
{
	DefaultView = View(glm::fvec3(Game::ScreenSizeX/2, Game::ScreenSizeY/2, -10), glm::fvec3(Game::ScreenSizeX/2, Game::ScreenSizeY/2, 0), glm::fvec3(0, -1, 0));
	id = pId;
	name = pName;

	view = DefaultView;

	width = Game::ScreenSizeX;
	height = Game::ScreenSizeY;
	uniformData.viewport = glm::fvec4(0, 0, width, height);

	minZoom = 1;
	maxZoom = 1;
	rotation = 0;
	targetRotation = 0;

	glViewport(uniformData.viewport.x, uniformData.viewport.y, uniformData.viewport.z, uniformData.viewport.w);

	auto s = Game::shaderManager.getShader("min");

	uboBlockIndex = glGetUniformBlockIndex(s->getID(), "Mat");
	GLint blockSize;
	glGetActiveUniformBlockiv(s->getID(), uboBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	GLubyte* blockBuffer = (GLubyte*)malloc(blockSize);
	const GLchar* names[] = { "proj" };
	GLuint indices[1];
	glGetUniformIndices(s->getID(), 1, names, indices);
	GLint offset[1];
	glGetActiveUniformsiv(s->getID(), 1, indices, GL_UNIFORM_OFFSET, offset);

	memcpy(blockBuffer + offset[0], getProjViewArray(), 16 * sizeof(GLfloat));

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_STREAM_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, uboBlockIndex, ubo);
}

void Camera::setCenter(GLfloat pX, GLfloat pY)
{
	view.eye.x = pX; view.eye.y = pY;
	view.center.x = view.eye.x; view.center.y = view.eye.y;
}

void Camera::setCenter(vec2f c)
{
	view.eye.x = c.x; view.eye.y = c.y;
	view.center.x = c.x; view.center.y = c.y;
}

void Camera::setSize(GLdouble pSizeX, GLdouble pSizeY)
{
	sizeX = pSizeX;
	sizeY = pSizeY;

	projMat = glm::ortho(-(sizeX / 2), (sizeX / 2), (sizeY / 2), -(sizeY / 2), -10.0, 10.0);
	uniformData.projViewMat = projMat * viewMat;
}

void Camera::setSize(vec2d pSize)
{
	setSize(pSize.x, pSize.y);
}

void Camera::zoom(GLdouble pFactor)
{
	//setSize(sizeX * pFactor, sizeY * pFactor);
	targetZoomFactor *= pFactor;
}

void Camera::rotate(float pRot)
{
	targetRotation += pRot;
}

void Camera::setRotation(float pRot)
{
	targetRotation = pRot;
}

void Camera::setViewport(GLint pX, GLint pY, GLsizei pWidth, GLsizei pHeight)
{
	x = pX; 
	y = pY; 
	width = pWidth; 
	height = pHeight;

	glViewport(x, y, width, height);
}

void Camera::setActive()
{
	viewMat = glm::lookAt(view.eye, view.center, view.up);
	projMat = glm::ortho(-sizeX / 2, sizeX / 2, sizeY / 2, -sizeY / 2, -10.0, 10.0);
	uniformData.projViewMat = viewMat * projMat;
}

void Camera::update(Time& dt)
{
	if (followTarget != nullptr)
	{
		if (smoothCamFollow)
		{
			vec2f pos(view.eye.x, view.eye.y);

			double blend = followSpeed * dt.getSeconds();

			pos = vlerp(pos, followTarget->getPosition(), blend);
			zoomFactor = lerp(zoomFactor, targetZoomFactor, blend);

			setSize(uniformData.viewport.z * zoomFactor, uniformData.viewport.w * zoomFactor);
			setCenter(pos);
		}
		else
		{
			setSize(uniformData.viewport.z * targetZoomFactor, uniformData.viewport.w * targetZoomFactor);
			setCenter(followTarget->getPosition());
		}
	}
	else
	{
		setSize(uniformData.viewport.z * targetZoomFactor, uniformData.viewport.w * targetZoomFactor);
		setCenter(view.center.x,view.center.y);
	}

	if (uniformData.viewport.z * targetZoomFactor > uniformData.viewport.z * minZoom)
	{
		targetZoomFactor = minZoom;
		setSize(uniformData.viewport.z * minZoom, uniformData.viewport.w * minZoom);
	}
	else if (uniformData.viewport.z * targetZoomFactor < uniformData.viewport.z * maxZoom)
	{
		targetZoomFactor = maxZoom;
		setSize(uniformData.viewport.z * maxZoom, uniformData.viewport.w * maxZoom);
	}

	if (constrainCam)
	{
		bool l = false;
		bool r = false;

		if ((view.eye.x - (sizeX/2)) < constraint.left)
		{
			l = true;
			view.eye.x = constraint.left + (sizeX/2);
		}
		
		if ((view.eye.x + (sizeX/2)) > (constraint.left + constraint.width))
		{
			r = true;
			view.eye.x = constraint.left + constraint.width - (sizeX/2);
		}

		if (l && r)
		{
			view.eye.x = constraint.left + (constraint.width / 2.f);
		}

		bool t = false;
		bool b = false;

		if ((view.eye.y - (sizeY/2)) < constraint.bot)
		{
			b = true;
			view.eye.y = constraint.bot + (sizeY/2);
		}
		
		if ((view.eye.y + (sizeY/2)) > (constraint.bot + constraint.height))
		{
			t = true;
			view.eye.y = constraint.bot + constraint.height - (sizeY/2);
		}

		if (t && b)
		{
			view.eye.y = constraint.bot + (constraint.height / 2.f);
		}

		view.center.x = view.eye.x;
		view.center.y = view.eye.y;
	}

	double blend = followSpeed * dt.getSeconds();
	rotation = lerp(rotation, targetRotation, blend);
	viewMat = glm::rotate(glm::fmat4(), rotation, glm::fvec3(0, 0, -1)) * glm::lookAt(view.eye, view.center, view.up);
	uniformData.projViewMat = projMat * viewMat;

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 64, getProjViewArray(), GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera* CameraManager::getCamera(unsigned int pId)
{
	return cameras.at(pId);
}

Camera* CameraManager::getCamera(std::string pName)
{
	return cameras.at(camNames.at(pName));
}

void CameraManager::setCurrCamera(unsigned int pId)
{
	currCamera = cameras.at(pId);
	glBindBufferBase(GL_UNIFORM_BUFFER, currCamera->uboBlockIndex, currCamera->ubo);
}

void CameraManager::setCurrCamera(std::string pName)
{
	currCamera = cameras.at(camNames.at(pName));
	glBindBufferBase(GL_UNIFORM_BUFFER, currCamera->uboBlockIndex, currCamera->ubo);
}

void CameraManager::setCurrCamera(Camera* cam)
{
	currCamera = cam;
	glBindBufferBase(GL_UNIFORM_BUFFER, currCamera->uboBlockIndex, currCamera->ubo);
}


Camera::View Camera::DefaultView;
Camera* CameraManager::DefaultCamera;