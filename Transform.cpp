#pragma once
#include "Transform.h"
#include "Vector.h"

Transform::Transform() : translation(vec2f(0, 0)), rotation(0), origin(vec2f(0.f, 0.f)), scalem(vec2f(1.f, 1.f)), needUpdate(true) 
{

}
