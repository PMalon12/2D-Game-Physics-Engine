#pragma once
#include "Utility.h"
#include <glm\glm.hpp>
//#include "Transform.h"

class Transform;

template<class T>
class vec2
{
public:
	vec2() {}
	vec2(T pX, T pY) : x(pX), y(pY) {}
	template<class T2>
	vec2(vec2<T2> p) : x(p.x), y(p.y) {}
	//vec2(vec2<T> p) : x(p.x), y(p.y) {}
	T x, y;
	
	inline bool operator==(const vec2<T>& rhs)
	{
		return ((x == rhs.x) && (y == rhs.y));
	}

	inline bool operator!=(const vec2<T>& rhs)
	{
		return !(this->operator==(rhs));
	}

	inline vec2<T> operator+(const vec2<T>& rhs)
	{
		return vec2<T>(this->x + rhs.x, this->y + rhs.y);
	}

	inline vec2<T> operator+(const T& rhs)
	{
		return vec2<T>(this->x + rhs, this->y + rhs);
	}

	inline vec2<T> operator-(const vec2<T>& rhs)
	{
		return vec2<T>(this->x - rhs.x, this->y - rhs.y);
	}

	inline vec2<T> operator-(const T rhs)
	{
		return vec2<T>(this->x - rhs, this->y - rhs);
	}

	inline vec2<T> operator*(const vec2<T>& rhs)
	{
		return vec2<T>(this->x * rhs.x, this->y * rhs.y);
	}

	template<class R>
	inline vec2<T> operator*(const vec2<R>& rhs)
	{
		return vec2<T>(this->x * rhs.x, this->y * rhs.y);
	}

	inline vec2<T> operator*(const T rhs)
	{
		return vec2<T>(this->x * rhs, this->y * rhs);
	}

	inline vec2<T> operator/(const vec2<T>& rhs)
	{
		return vec2<T>(this->x / rhs.x, this->y / rhs.y);
	}

	inline vec2<T> operator/(const T rhs)
	{
		return vec2<T>(this->x / rhs, this->y / rhs);
	}

	inline vec2<T>& operator+=(const vec2<T>& rhs)
	{
		(*this) = (*this) + rhs;
		return *this;
	}

	inline vec2<T>& operator-=(const vec2<T>& rhs)
	{
		(*this) = (*this) - rhs;
		return *this;
	}

	inline vec2<T>& operator*=(const vec2<T>& rhs)
	{
		(*this) = (*this) * rhs;
		return *this;
	}

	inline vec2<T>& operator*=(const T rhs)
	{
		(*this) = (*this) * rhs;
		return *this;
	}

	inline vec2<T>& operator/=(const vec2<T>& rhs)
	{
		(*this) = (*this) / rhs;
		return *this;
	}

	inline vec2<T>& operator/=(const T rhs)
	{
		(*this) = (*this) / rhs;
		return *this;
	}

	inline vec2<T>& inverse()
	{
		x = -x;
		y = -y;
		return *this;
	}

	inline vec2<T>& normal()
	{
		T temp = x;
		x = y;
		y = -temp;
		return *this;
	}

	inline vec2<T> getNormal()
	{
		return vec2f(y, -x);
	}

	inline vec2<T> getNormalised()
	{
		auto inverseSquareRoot = invsqrt((x*x) + (y*y));
		return vec2<T>(x*inverseSquareRoot, y*inverseSquareRoot);
	}

	inline vec2<T>& normalise()
	{
		auto inverseSquareRoot = invsqrt((x*x) + (y*y));
		x *= inverseSquareRoot;
		y *= inverseSquareRoot;
		return *this;
	}

	inline float mag() const
	{
		return std::sqrtf(magSquared());
	}

	inline T magSquared() const
	{
		return (x*x) + (y*y);
	}

	inline T dot(const vec2<T>& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y);
	}

	inline T cross(const vec2<T>& rhs) const
	{
		return (x * rhs.y) - (y * rhs.x);
	}

	inline vec2<T> cross(const T rhs) const
	{
		return vec2<T>(rhs * y, -rhs * x);
	}

	inline vec2<T> negative() const
	{
		return vec2<T>(-x, -y);
	}

	inline vec2<T> negate()
	{
		x = -x; y = -y;
		return *this;
	}

	inline vec2<T>& rotate(double rad)
	{
		T oldx = x, oldy = y;
		double cos = std::cos(rad), sin = std::sin(rad);
		x = oldx * cos - oldy * sin;
		y = oldx * sin + oldy * cos;
		return *this;
	}

	inline bool inBetween(vec2<T> left, vec2<T> right)
	{
		return (cross(left) * cross(right) >= 0 && right.cross(left) * right.cross(*this) >= 0);
	}

	inline vec2<T>& transform(glm::fmat3&);
	inline vec2<T> getTransformed(glm::fmat3&);
};

template<class T, class R>
inline vec2<T> operator*(R left, const vec2<T>& right)
{
	return vec2<T>(right.x * left, right.y * left);
}

template<class T, class R>
inline vec2<T> operator%(const vec2<T>& left, const vec2<R>& right)
{
	return vec2<float>(std::fmodf(left.x, right.x), std::fmodf(left.y,right.y));
}

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;
typedef vec2<long> vec2l;

template<class T>
class vec3
{
public:
	vec3() {}
	vec3(T pX, T pY, T pZ) : x(pX), y(pY), z(pZ) {}
	template<class T2>
	vec3(vec3<T2> p) : x(p.x), y(p.y), z(p.z) {}
	//vec3(vec3<T> p) : x(p.x), y(p.y) {}
	T x, y, z;

	bool operator==(const vec3<T>& rhs)
	{
		return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
	}

	bool operator!=(const vec3<T>& rhs)
	{
		return !(this->operator==(rhs));
	}

	vec3<T> operator+(const vec3<T>& rhs)
	{
		return vec3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
	}

	vec3<T> operator+(const T& rhs)
	{
		return vec3<T>(this->x + rhs, this->y + rhs, this-> + rhs);
	}

	vec3<T> operator-(const vec3<T>& rhs)
	{
		return vec3<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	vec3<T> operator-(const T rhs)
	{
		return vec3<T>(this->x - rhs, this->y - rhs, this->z - rhs);
	}

	vec3<T> operator*(const vec3<T>& rhs)
	{
		return vec3<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
	}

	vec3<T> operator*(const T rhs)
	{
		return vec3<T>(this->x * rhs, this->y * rhs, this->z * rhs);
	}

	vec3<T> operator/(const vec3<T>& rhs)
	{
		return vec3<T>(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
	}

	vec3<T> operator/(const T rhs)
	{
		return vec3<T>(this->x / rhs, this->y / rhs, this->z / rhs);
	}

	vec3<T>& operator+=(const vec3<T>& rhs)
	{
		(*this) = (*this) + rhs;
		return *this;
	}

	vec3<T>& operator-=(const vec3<T>& rhs)
	{
		(*this) = (*this) - rhs;
		return *this;
	}

	vec3<T>& operator*=(const vec3<T>& rhs)
	{
		(*this) = (*this) * rhs;
		return *this;
	}

	vec3<T>& operator/=(const vec3<T>& rhs)
	{
		(*this) = (*this) / rhs;
		return *this;
	}

	vec3<T>& inverse()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	//vec3<T>& normal()
	//{
	//	T temp = x;
	//	x = y;
	//	y = -temp;
	//	return *this;
	//}

	//vec3<T> getNormal()
	//{
	//	return vec3f(y, -x);
	//}

	vec3<T>& normalise()
	{
		auto inverseSquareRoot = invsqrt((x*x) + (y*y) + (z*z));
		x *= inverseSquareRoot;
		y *= inverseSquareRoot;
		z *= inverseSquareRoot;
		return *this;
	}

	float mag() const
	{
		return std::sqrtf(magSquared());
	}

	double magSquared() const
	{
		return (x*x) + (y*y) + (z*z);
	}

	double dot(const vec3<T>& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y) + (x * rhs.y);
	}

	vec3<T> cross(const vec3<T>& rhs) 
	{
		return vec3<T>((y * rhs.z - z * rhs.y), (z * rhs.x - x* rhs.z), (x * rhs.y - y * rhs.x));
	}
};

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<int> vec3i;
typedef vec3<long> vec3l;

template<class T>
class vec4
{
public:
	vec4() {}
	vec4(T pX, T pY, T pZ, T pW) : x(pX), y(pY), z(pZ), w(pW) {}
	template<class T2>
	vec4(vec4<T2> p) : x(p.x), y(p.y), z(p.z), w(p.w) {}
	//vec4(vec4<T> p) : x(p.x), y(p.y) {}
	T x, y, z, w;

	bool operator==(const vec4<T>& rhs)
	{
		return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w));
	}

	bool operator!=(const vec4<T>& rhs)
	{
		return !(this->operator==(rhs));
	}

	vec4<T> operator+(const vec4<T>& rhs)
	{
		return vec4<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w);
	}

	vec4<T> operator+(const T& rhs)
	{
		return vec4<T>(this->x + rhs, this->y + rhs, this->z + rhs, this->w + rhs);
	}

	vec4<T> operator-(const vec4<T>& rhs)
	{
		return vec4<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w);
	}

	vec4<T> operator-(const T rhs)
	{
		return vec4<T>(this->x - rhs, this->y - rhs, this->z - rhs, this->w - rhs);
	}

	vec4<T> operator*(const vec4<T>& rhs)
	{
		return vec4<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w);
	}

	vec4<T> operator*(const T rhs)
	{
		return vec4<T>(this->x * rhs, this->y * rhs, this->z * rhs, this->w * rhs);
	}

	vec4<T> operator/(const vec4<T>& rhs)
	{
		return vec4<T>(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w);
	}

	vec4<T> operator/(const T rhs)
	{
		return vec4<T>(this->x / rhs, this->y / rhs, this->z / rhs, this->w / rhs);
	}

	vec4<T>& operator+=(const vec4<T>& rhs)
	{
		(*this) = (*this) + rhs;
		return *this;
	}

	vec4<T>& operator-=(const vec4<T>& rhs)
	{
		(*this) = (*this) - rhs;
		return *this;
	}

	vec4<T>& operator*=(const vec4<T>& rhs)
	{
		(*this) = (*this) * rhs;
		return *this;
	}

	vec4<T>& operator/=(const vec4<T>& rhs)
	{
		(*this) = (*this) / rhs;
		return *this;
	}

	/*vec4<T>& inverse()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
		return *this;
	}

	vec4<T>& normalise()
	{
		auto inverseSquareRoot = invsqrt((x*x) + (y*y) + (z*z) + (w*w));
		x *= inverseSquareRoot;
		y *= inverseSquareRoot;
		z *= inverseSquareRoot;
		w *= inverseSquareRoot;
		return *this;
	}

	float mag() const
	{
		return std::sqrtf(magSquared());
	}

	double magSquared() const
	{
		return (x*x) + (y*y) + (z*z) + (w*w);
	}

	double dot(const vec4<T>& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y) + (x * rhs.y) + (w * rhs.w);
	}*/
};

typedef vec4<float> vec4f;
typedef vec4<double> vec4d;
typedef vec4<int> vec4i;
typedef vec4<long> vec4l;

template<class T>
class vec5
{
public:
	vec5() {}
	vec5(T pX, T pY, T pZ, T pW, T pV) : x(pX), y(pY), z(pZ), w(pW), v(pV) {}
	T x, y, z, w, v;
};

typedef vec5<float> vec5f;
typedef vec5<double> vec5d;
typedef vec5<int> vec5i;
typedef vec5<long> vec5l;

template<class T>
class rect
{
public:
	rect() {}
	rect(T pLeft, T pBot, T pWidth, T pHeight) : left(pLeft), bot(pBot), width(pWidth), height(pHeight) {}

	union
	{
		struct { T left ,bot, width, height; };
		struct { vec2<T> botLeft, size; };
	};

	T top() { return bot + height; }
	T right() { return left + width; }
	vec2<T> mid() { return vec2<T>(left + width*0.5, bot + height*0.5); }

	bool operator==(const rect<T>& rhs)
	{
		return ((bot == rhs.bot) && (left == rhs.left) && (width == rhs.width) && (height == rhs.height));
	}

	bool operator!=(const rect<T>& rhs)
	{
		return !(this == rhs);
	}

	rect<T> operator+(const rect<T>& rhs)
	{
		T minX1 = std::min(left, left + width);
		T maxX1 = std::max(left, left + width);
		T minY1 = std::min(bot, bot + height);
		T maxY1 = std::max(bot, bot + height);

		T minX2 = std::min(rhs.left, rhs.left + rhs.width);
		T maxX2 = std::max(rhs.left, rhs.left + rhs.width);
		T minY2 = std::min(rhs.bot, rhs.bot + rhs.height);
		T maxY2 = std::max(rhs.bot, rhs.bot + rhs.height);

		T minBot = std::min(minY1, minY2);
		T minLeft = std::min(minX1, minX2);
		T maxRight = std::max(maxX1, maxX2);
		T maxTop = std::max(maxY1, maxY2);

		return frect(minLeft, minBot, maxRight - minLeft, maxTop - minBot);
	}

	template<class R>
	bool contains(const vec2<R>& pPoint)
	{
		return pPoint.x > left && pPoint.x < left + width && pPoint.y > bot && pPoint.y < bot + height;
	}

	bool intersects(const rect<T>& pTarget, rect<T>& pIntersection)
	{
		T minX1 = std::min(left, left + width);
		T maxX1 = std::max(left, left + width);
		T minY1 = std::min(bot, bot + height);
		T maxY1 = std::max(bot, bot + height);

		T minX2 = std::min(pTarget.left, pTarget.left + pTarget.width);
		T maxX2 = std::max(pTarget.left, pTarget.left + pTarget.width);
		T minY2 = std::min(pTarget.bot, pTarget.bot + pTarget.height);
		T maxY2 = std::max(pTarget.bot, pTarget.bot + pTarget.height);

		T botIntersect = std::max(minY1, minY2);
		T leftIntersect = std::max(minX1, minX2);
		T rightIntersect = std::min(maxX1, maxX2);
		T topIntersect = std::min(maxY1, maxY2);

		if ((leftIntersect < rightIntersect) && (botIntersect < topIntersect))
		{
			pIntersection = rect<T>(leftIntersect, botIntersect, rightIntersect - leftIntersect, botIntersect - botIntersect);
			return true;
		}
		else
		{
			pIntersection = rect<T>(0, 0, 0, 0);
			return false;
		}
	}

	bool intersects(const rect<T>& pTarget)
	{
		rect<T> intersection;
		return this->intersects(pTarget, intersection);
	}

};

typedef rect<float> frect;
typedef rect<double> drect;
typedef rect<int> irect;