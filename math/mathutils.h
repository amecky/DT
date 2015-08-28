#pragma once
#include <Vector.h>

namespace math {

	v2 get_radial_velocity(float angle,float velocity);

	float get_angle(const v2& v1,const v2& v2);

	float get_target_angle(const v2& v1,const v2& v2);
}