#pragma once
#include <Vector.h>
#include "..\renderer\render_types.h"

namespace math {

	v2 get_radial_velocity(float angle,float velocity);

	float get_angle(const v2& v1,const v2& v2);

	float get_target_angle(const v2& v1,const v2& v2);

	Texture buildTexture(const Rect& r, float textureWidth = 1024.0f, float textureHeight = 1024.0f, bool useHalfTexel = true);

	v2 srt(const Vector2f& v,const Vector2f& u,float scaleX,float scaleY,float rotation);
}