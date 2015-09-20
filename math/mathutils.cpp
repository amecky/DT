#include "mathutils.h"
#include "..\stdafx.h"
#include "..\renderer\render_types.h"
#include "..\utils\mtrand.h"

namespace math {

	v2 get_radial_velocity(float angle,float velocity) {
		float vx = (float)cos(angle)*velocity;
		float vy = (float)sin(angle)*velocity;
		return v2(vx,vy);
	}

	v2 get_radial_velocity(float angle, float velocity, float velocityVariance) {
		float var = random(-velocityVariance, velocityVariance);
		return get_radial_velocity(angle, velocity + var);
	}

	float get_angle(const v2& v1,const v2& v2) {	
		if ( v1 != v2 ) {
			Vector2f vn1 = normalize(v1);
			Vector2f vn2 = normalize(v2);
			float dt = dot(vn1,vn2);		
			if ( dt < -1.0f ) {
				dt = -1.0f;
			}
			if ( dt > 1.0f ) {
				dt = 1.0f;
			}
			float tmp = acos(dt);
			float crs = cross(vn1,vn2);
			if ( crs < 0.0f ) {
				tmp = TWO_PI - tmp;
			}
			return tmp;		
		}
		else {
			return 0.0f;
		}
	}

	float get_target_angle(const v2& v1,const v2& v2) {	
		Vector2f diff = v2 - v1;
		float angle = get_angle(V2_RIGHT,diff);
		if ( angle >= TWO_PI ) {
			angle -= TWO_PI;
		}
		return angle;
	}

	v4 getTextureCoordinates(const Rect& textureRect,float textureWidth,float textureHeight,bool useHalfTexel) {
		v4 ret;
		if ( useHalfTexel ) {
			float halfTexel = 0.5f;
			float const width   = textureWidth;
			float const height  = textureHeight;

			float tw = textureRect.width() / textureWidth;
			float th = textureRect.height() / textureHeight;

			float kUOffset = halfTexel/width;
			float kVOffset = halfTexel/height;

			ret.x = textureRect.left/width  + kUOffset;
			ret.y = textureRect.top/height + kVOffset;  

			ret.z = ret.x + tw   - 2.0f*kUOffset;
			ret.w = ret.y + th  - 2.0f*kVOffset;
		}
		else {
			ret.x = textureRect.left/textureWidth;
			ret.z = textureRect.right/textureWidth;
			ret.y = textureRect.top/textureHeight;
			ret.w = textureRect.bottom/textureHeight;
		}
		return ret;
	}

	Texture buildTexture(float top, float left, float width, float height, float textureWidth, float textureHeight, bool useHalfTexel) {
		Rect r(top, left, width, height);
		return buildTexture(r, textureWidth, textureHeight, useHalfTexel);
	}

	Texture buildTexture(const Rect& r, float textureWidth, float textureHeight, bool useHalfTexel) {
		Texture ret;
		ret.uv = getTextureCoordinates(r, textureWidth, textureHeight,true);
		ret.textureID = 0;
		ret.dim = Vector2f(r.width(), r.height());
		return ret;
	}

	v2 srt(const Vector2f& v,const Vector2f& u,float scaleX,float scaleY,float rotation) {
		float sx = u.x * scaleX;
		float sy = u.y * scaleY;

		// rotation clock wise
		//float xt = cosf(rotation) * sx + sinf(rotation) * sy;
		//float yt = -sinf(rotation) * sx + cosf(rotation) * sy;

		// rotation counter clock wise
		//float xt = cosf(rotation) * sx - sinf(rotation) * sy;
		//float yt = sinf(rotation) * sx + cosf(rotation) * sy;

		float xt = cos(rotation) * sx - sin(rotation) * sy;
		float yt =sin(rotation) * sx + cos(rotation) * sy;

		xt += v.x;
		yt += v.y;

		return Vector2f(xt,yt);
	}

	float random(float min,float max) {
		MTRand_open rand;
		//rand.seed(GetTickCount());
		return min + (max - min)* (float)rand();
	}

	v2 random(const v2& min,const v2& max) {
		MTRand_open rand;
		v2 ret;
		ret.x = min.x + (max.x - min.x)* (float)rand();
		ret.y = min.y + (max.y - min.y)* (float)rand();
		return ret;
	}

	bool chanceRoll(int probability) {
		int x = random(0,100);
		return x <= probability;
	}

	float clamp(float value, float min, float max) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}

	float calculateRotation(const v2& v) {
		Vector2f vn = normalize(v);
		if (vn != V2_RIGHT) {
			float dt = clamp(dot(vn, V2_RIGHT), -1.0f, 1.0f);
			float tmp = acos(dt);
			float cross = -1.0f * vn.y;
			if (cross > 0.0f) {
				tmp = 2.0f * PI - tmp;
			}
			return tmp;
		}
		else {
			return 0.0f;
		}
	}

}