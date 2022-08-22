#ifndef MATH_3D_H
#define	MATH_3D_H
#include <math.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

#include "const.h"
using namespace glm;

//#define M_PI 3.1415926535
//#define ToRadian(x) ((x) * M_PI / 180.0f)
//#define ToDegree(x) ((x) * 180.0f / M_PI)

#define INVALID_OGL_VALUE 0xFFFFFFFF

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#endif	/* MATH_3D_H */