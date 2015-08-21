#include "..\StdAfx.h"
#include "Mesh.h"
#include <Matrix.h>
#include "VIBuffer.h"


namespace data {

	// -----------------------------------------------------------
	// build cube
	// -----------------------------------------------------------
	void build_cube(PCTMeshData& data,float dx,float dy,float dz) {
		float hx = dx * 0.5f;
		float hy = dy * 0.5f;
		float hz = dz * 0.5f;
		// front
		Quad<PCTVertex> front;
		front.v[0] = PCTVertex( hx, -hy, hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,1.0f);
		front.v[1] = PCTVertex( hx,  hy, hz, D3DCOLOR_XRGB(255, 255, 2555),0.0f,0.0f);
		front.v[2] = PCTVertex(-hx,  hy, hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,0.0f);
		front.v[3] = PCTVertex(-hx, -hy, hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,1.0f);
		data.addQuad(front);
		// back
		Quad<PCTVertex> back;
		back.v[0] = PCTVertex( -hx, -hy, -hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,1.0f);
		back.v[1] = PCTVertex( -hx,  hy, -hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,0.0f);
		back.v[2] = PCTVertex(  hx,  hy, -hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,0.0f);
		back.v[3] = PCTVertex(  hx, -hy, -hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,1.0f);
		data.addQuad(back);
		// left
		Quad<PCTVertex> left;
		left.v[0] = PCTVertex( hx, -hy, -hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,1.0f);
		left.v[1] = PCTVertex( hx,  hy, -hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,0.0f);
		left.v[2] = PCTVertex( hx,  hy,  hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,0.0f);
		left.v[3] = PCTVertex( hx, -hy,  hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,1.0f);
		data.addQuad(left);
		// right
		Quad<PCTVertex> right;
		right.v[0] = PCTVertex( -hx, -hy, hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,1.0f);
		right.v[1] = PCTVertex( -hx,  hy, hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,0.0f);
		right.v[2] = PCTVertex( -hx,  hy, -hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,0.0f);
		right.v[3] = PCTVertex( -hx, -hy, -hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,1.0f);
		data.addQuad(right);
		// top
		Quad<PCTVertex> top;
		top.v[0] = PCTVertex(  hx, hy,  hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,1.0f);
		top.v[1] = PCTVertex(  hx, hy, -hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,0.0f);
		top.v[2] = PCTVertex( -hx, hy, -hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,0.0f);
		top.v[3] = PCTVertex( -hx,hy,  hz, D3DCOLOR_XRGB(255, 255, 2550),0.0f,1.0f);
		data.addQuad(top);

		// bottom
		Quad<PCTVertex> bottom;
		bottom.v[0] = PCTVertex( hx, -hy,-hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,1.0f);
		bottom.v[1] = PCTVertex( hx, -hy, hz, D3DCOLOR_XRGB(255, 255, 255),1.0f,0.0f);
		bottom.v[2] = PCTVertex(-hx, -hy, hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,0.0f);
		bottom.v[3] = PCTVertex(-hx, -hy,-hz, D3DCOLOR_XRGB(255, 255, 255),0.0f,1.0f);
		data.addQuad(bottom);
	}

	// -----------------------------------------------------------
	// build unit cube
	// -----------------------------------------------------------
	void build_unit_cube(PCTMeshData& data) {
		build_cube(data,1.0f,1.0f,1.0f);		
	}

	void add_xz_plane(PCTMeshData& data,float width,float height) {
		float hx = width * 0.5f;
		float hy = 0.0f;
		float hz = height * 0.5f;
		float u2 = 1.0f;//width;
		float v2 = 1.0f;//height;
		// top
		Quad<PCTVertex> plane;
		plane.v[0] = PCTVertex(  hx, hy,  hz, 0.0f,v2);
		plane.v[1] = PCTVertex(  hx, hy, -hz, 0.0f,0.0f);
		plane.v[2] = PCTVertex( -hx, hy, -hz, u2,0.0f);
		plane.v[3] = PCTVertex( -hx, hy,  hz, u2,v2);
		data.addQuad(plane);
	}

	void add_line(PCTMeshData& data,const Vector3f& start,const Vector3f& end,float thickness) {
		Vector3f d = end - start;
		float u2 = 1.0f;//width;
		float v2 = 1.0f;//height;
		Vector3f dn = rotationZ<float>(-D3DX_PI * 0.5f) * d;
		dn = normalize(dn);
		dn *= thickness;
		Quad<PCTVertex> plane;
		Vector3f sn = start + dn;
		Vector3f en = end + dn;
		plane.v[0] = PCTVertex(  start.x, start.y,  start.z, D3DCOLOR_XRGB(255, 0, 255), 0.0f,v2);
		plane.v[1] = PCTVertex(  sn.x, sn.y,sn.z, D3DCOLOR_XRGB(255, 0, 255), 0.0f,0.0f);
		plane.v[2] = PCTVertex(  en.x, en.y, en.z, D3DCOLOR_XRGB(255, 0, 255), u2,0.0f);
		plane.v[3] = PCTVertex(  end.x, end.y, end.z, D3DCOLOR_XRGB(255, 0, 255), u2,v2);
		data.addQuad(plane);

	}

	void add_xz_plane(PCMeshData& data,float width,float height) {
		float hx = width * 0.5f;
		float hy = 0.0f;
		float hz = height * 0.5f;
		float u2 = 1.0f;//width;
		float v2 = 1.0f;//height;
		// top
		Quad<PCVertex> plane;
		plane.v[0] = PCVertex(  hx, hy,  hz, D3DCOLOR_XRGB(255, 0, 0));
		plane.v[1] = PCVertex(  hx, hy, -hz, D3DCOLOR_XRGB(255, 0, 0));
		plane.v[2] = PCVertex( -hx, hy, -hz, D3DCOLOR_XRGB(255, 0, 0));
		plane.v[3] = PCVertex( -hx, hy,  hz, D3DCOLOR_XRGB(255, 0, 0));
		data.addQuad(plane);
	}

	void add_line(PCMeshData& data,const Vector3f& start,const Vector3f& end,float thickness) {
		Vector3f d = end - start;
		float u2 = 1.0f;//width;
		float v2 = 1.0f;//height;
		Vector3f dn = rotationZ<float>(-D3DX_PI * 0.5f) * d;
		dn = normalize(dn);
		dn *= thickness;
		Quad<PCVertex> plane;
		Vector3f sn = start + dn;
		Vector3f en = end + dn;
		plane.v[0] = PCVertex(  start.x, start.y,  start.z, D3DCOLOR_XRGB(192, 0, 0));
		plane.v[1] = PCVertex(  sn.x, sn.y,sn.z, D3DCOLOR_XRGB(192, 0, 0));
		plane.v[2] = PCVertex(  en.x, en.y, en.z, D3DCOLOR_XRGB(192, 0, 0));
		plane.v[3] = PCVertex(  end.x, end.y, end.z, D3DCOLOR_XRGB(192, 0, 0));
		data.addQuad(plane);

	}
}
