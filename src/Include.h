
#pragma once

#include <cstddef>
#include <stdio.h>
#include <tchar.h>
#include <list>
#include <bitset>
#include <random>
#include <functional>

#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <basetsd.h>
#include <set>

#include <SFML/Config.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <GL\glew.h>
#include <GL\GLU.h>
#include <glm\glm.hpp>
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\matrix_transform_2d.hpp"

#include <map>
#include <iostream>
#include <cassert>

#include "Utility.h"
#include "Vector.h"
#include "Shader.h"
#include "Time.h"

#include "CollisionCallbacksAndCriteria.h"

#define PI 3.1415926535897932384626433832795

#define DEGTORAD 0.01745329251994329576923690768489
#define RADTODEG 57.295779513082320876798154814105

#define METTOPIX 100
#define PIXTOMET 1/METTOPIX

#define TOSECONDS 0.000001

#define CLOCKWISE glm::fvec3(0,0,1)
#define CTR_CLOCKWISE glm::fvec3(0,0,1)

#define PHYSICAL_BIT 0

#define IS_O(a,b) ((a & b) == b)
#define IS_NOT_O(a,b) ((a & b) != b)

#define O_ENTITY		0x0e000001
#define O_PLAYER		0x0e100001
#define O_TEMP			0x0e200001
#define O_PROJECTILE	0x0e300000

#define O_STRUCTURE		0x0d000011
#define O_STATIC_STRUCT 0xad100011
#define O_SS_LAVA		0xad110011
#define O_DYNAMIC_STRUCT 0xad200011

#define O_CHECKPOINT	0xbd120000
#define O_END_LEVEL		0xbd130000
#define O_POWERUP		0xbd140000

#define O_FONT			0xff000000
#define O_SEGOEUIL		0xff100000
#define O_SEGUISB		0xff200000
#define O_CONSOLA		0xff300000