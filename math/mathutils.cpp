#include "mathutils.h"
#include "..\stdafx.h"

namespace math {

	v2 get_radial_velocity(float angle,float velocity) {
		float vx = (float)cos(angle)*velocity;
		float vy = (float)sin(angle)*velocity;
		return v2(vx,vy);
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
}