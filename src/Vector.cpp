#pragma once
#include "Vector.h"
#include "Transform.h"

template<class T>
inline vec2<T> transformPoint(vec2<T> point, const Transform& t)
{
	glm::vec3<T> a(x, y, 0);
	a = a * t.getTransform();
	x = a.x; y = a.y;
	return *this;
}

template<class T>
inline vec2<T>& vec2<T>::transform(glm::fmat3& t)
{
	glm::fvec3 a(x, y, 1);
	a = t * a;
	x = a.x; y = a.y;
	return *this;
}

template<class T>
inline vec2<T> vec2<T>::getTransformed(glm::fmat3& t)
{
	glm::fvec3 a(x, y, 1);
	a = t * a;
	return vec2f(a.x, a.y);
}

template class vec2<float>;
template class vec2<double>;

//{
//	//glm::vec3<T> a(x, y, 0);
//	//a = a * t.getTransform();
//	//x = a.x; y = a.y;
//	//return *this;
//}