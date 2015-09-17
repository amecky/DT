#pragma once
#include <Vector.h>

struct Texture;
struct Rect;

namespace math {

	v2 get_radial_velocity(float angle,float velocity);

	float get_angle(const v2& v1,const v2& v2);

	float get_target_angle(const v2& v1,const v2& v2);

	Texture buildTexture(const Rect& r, float textureWidth = 1024.0f, float textureHeight = 1024.0f, bool useHalfTexel = false);

	Texture buildTexture(float top, float left, float width, float height, float textureWidth = 1024.0f, float textureHeight = 1024.0f, bool useHalfTexel = false);

	v2 srt(const Vector2f& v,const Vector2f& u,float scaleX,float scaleY,float rotation);

	float random(float min,float max);

	v2 random(const v2& min,const v2& max);

	bool chanceRoll(int probability);

	float calculateRotation(const v2& v);
}