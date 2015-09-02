#include "..\StdAfx.h"
#include "Mesh.h"
#include <Matrix.h>
#include "..\utils\JSONReader.h"

namespace data {

	void getTextureCoordinates(const Rect& textureRect,int textureWidth,float textureHeight,float* u1,float* v1,float* u2,float* v2,bool useHalfTexel) {
		if ( useHalfTexel ) {
			float halfTexel = 0.5f;
			float const width   = textureWidth;
			float const height  = textureHeight;

			float kUOffset = halfTexel/width;
			float kVOffset = halfTexel/height;

			*u1 = static_cast<float>(textureRect.left)/width  + kUOffset;
			*v1 = static_cast<float>(textureRect.top)/height + kVOffset;  

			*u2 = *u1 + static_cast<float>(textureRect.width()) /width   - 2.0f*kUOffset;
			*v2 = *v1 + static_cast<float>(textureRect.height())/height  - 2.0f*kVOffset;
		}
		else {
			*u1 = static_cast<float>(textureRect.left)/textureWidth;
			*u2 = static_cast<float>(textureRect.right)/textureWidth;
			*v1 = static_cast<float>(textureRect.top)/textureHeight;
			*v2 = static_cast<float>(textureRect.bottom)/textureHeight;
		}
	}

	void build_cube(PCTMeshData* data,float dx,float dy,float dz,const Rect& textureRect,float textureSize) {
		float hx = dx * 0.5f;
		float hy = dy * 0.5f;
		float hz = dz * 0.5f;
		float u1,v1,u2,v2;

		getTextureCoordinates(textureRect,textureSize,textureSize,&u1,&v1,&u2,&v2,true);

		// front
		Quad<PCTVertex> front;
		front.v[0] = PCTVertex( hx, -hy, hz, Color(255, 255, 255),u1,v2);
		front.v[1] = PCTVertex( hx,  hy, hz, Color(255, 255, 255),u1,v1);
		front.v[2] = PCTVertex(-hx,  hy, hz, Color(255, 255, 255),u2,v1);
		front.v[3] = PCTVertex(-hx, -hy, hz, Color(255, 255, 255),u2,v2);
		data->addQuad(front);
		// back
		Quad<PCTVertex> back;
		back.v[0] = PCTVertex( -hx, -hy, -hz, Color(255, 255, 255),u1,v2);
		back.v[1] = PCTVertex( -hx,  hy, -hz, Color(255, 255, 255),u1,v1);
		back.v[2] = PCTVertex(  hx,  hy, -hz, Color(255, 255, 255),u2,v1);
		back.v[3] = PCTVertex(  hx, -hy, -hz, Color(255, 255, 255),u2,v2);
		data->addQuad(back);
		// left
		Quad<PCTVertex> left;
		left.v[0] = PCTVertex( hx, -hy, -hz, Color(255, 255, 255),u1,v2);
		left.v[1] = PCTVertex( hx,  hy, -hz, Color(255, 255, 255),u1,v1);
		left.v[2] = PCTVertex( hx,  hy,  hz, Color(255, 255, 255),u2,v1);
		left.v[3] = PCTVertex( hx, -hy,  hz, Color(255, 255, 255),u2,v2);
		data->addQuad(left);
		// right
		Quad<PCTVertex> right;
		right.v[0] = PCTVertex( -hx, -hy,  hz, Color(255, 255, 255),u1,v2);
		right.v[1] = PCTVertex( -hx,  hy,  hz, Color(255, 255, 255),u1,v1);
		right.v[2] = PCTVertex( -hx,  hy, -hz, Color(255, 255, 255),u2,v1);
		right.v[3] = PCTVertex( -hx, -hy, -hz, Color(255, 255, 255),u2,v2);
		data->addQuad(right);
		// top
		Quad<PCTVertex> top;
		top.v[0] = PCTVertex(  hx, hy,  hz, Color(255, 255, 255),u1,v2);
		top.v[1] = PCTVertex(  hx, hy, -hz, Color(255, 255, 255),u1,v1);
		top.v[2] = PCTVertex( -hx, hy, -hz, Color(255, 255, 255),u2,v1);
		top.v[3] = PCTVertex( -hx,hy,  hz, Color(255, 255, 2550),u2,v2);
		data->addQuad(top);

		// bottom
		Quad<PCTVertex> bottom;
		bottom.v[0] = PCTVertex( hx, -hy,-hz, Color(255, 255, 255),u1,v2);
		bottom.v[1] = PCTVertex( hx, -hy, hz, Color(255, 255, 255),u1,v1);
		bottom.v[2] = PCTVertex(-hx, -hy, hz, Color(255, 255, 255),u2,v1);
		bottom.v[3] = PCTVertex(-hx, -hy,-hz, Color(255, 255, 255),u2,v2);
		data->addQuad(bottom);
	}
	// -----------------------------------------------------------
	// build cube
	// -----------------------------------------------------------
	void build_cube(PCTMeshData& data,float dx,float dy,float dz) {
		float hx = dx * 0.5f;
		float hy = dy * 0.5f;
		float hz = dz * 0.5f;
		// front
		Quad<PCTVertex> front;
		front.v[0] = PCTVertex( hx, -hy, hz, Color(255, 255, 255),0.0f,1.0f);
		front.v[1] = PCTVertex( hx,  hy, hz, Color(255, 255, 2555),0.0f,0.0f);
		front.v[2] = PCTVertex(-hx,  hy, hz, Color(255, 255, 255),1.0f,0.0f);
		front.v[3] = PCTVertex(-hx, -hy, hz, Color(255, 255, 255),1.0f,1.0f);
		data.addQuad(front);
		// back
		Quad<PCTVertex> back;
		back.v[0] = PCTVertex( -hx, -hy, -hz, Color(255, 255, 255),0.0f,1.0f);
		back.v[1] = PCTVertex( -hx,  hy, -hz, Color(255, 255, 255),0.0f,0.0f);
		back.v[2] = PCTVertex(  hx,  hy, -hz, Color(255, 255, 255),1.0f,0.0f);
		back.v[3] = PCTVertex(  hx, -hy, -hz, Color(255, 255, 255),1.0f,1.0f);
		data.addQuad(back);
		// left
		Quad<PCTVertex> left;
		left.v[0] = PCTVertex( hx, -hy, -hz, Color(255, 255, 255),0.0f,1.0f);
		left.v[1] = PCTVertex( hx,  hy, -hz, Color(255, 255, 255),0.0f,0.0f);
		left.v[2] = PCTVertex( hx,  hy,  hz, Color(255, 255, 255),1.0f,0.0f);
		left.v[3] = PCTVertex( hx, -hy,  hz, Color(255, 255, 255),1.0f,1.0f);
		data.addQuad(left);
		// right
		Quad<PCTVertex> right;
		right.v[0] = PCTVertex( -hx, -hy, hz, Color(255, 255, 255),0.0f,1.0f);
		right.v[1] = PCTVertex( -hx,  hy, hz, Color(255, 255, 255),0.0f,0.0f);
		right.v[2] = PCTVertex( -hx,  hy, -hz, Color(255, 255, 255),1.0f,0.0f);
		right.v[3] = PCTVertex( -hx, -hy, -hz, Color(255, 255, 255),1.0f,1.0f);
		data.addQuad(right);
		// top
		Quad<PCTVertex> top;
		top.v[0] = PCTVertex(  hx, hy,  hz, Color(255, 255, 255),1.0f,1.0f);
		top.v[1] = PCTVertex(  hx, hy, -hz, Color(255, 255, 255),1.0f,0.0f);
		top.v[2] = PCTVertex( -hx, hy, -hz, Color(255, 255, 255),0.0f,0.0f);
		top.v[3] = PCTVertex( -hx,hy,  hz, Color(255, 255, 2550),0.0f,1.0f);
		data.addQuad(top);

		// bottom
		Quad<PCTVertex> bottom;
		bottom.v[0] = PCTVertex( hx, -hy,-hz, Color(255, 255, 255),1.0f,1.0f);
		bottom.v[1] = PCTVertex( hx, -hy, hz, Color(255, 255, 255),1.0f,0.0f);
		bottom.v[2] = PCTVertex(-hx, -hy, hz, Color(255, 255, 255),0.0f,0.0f);
		bottom.v[3] = PCTVertex(-hx, -hy,-hz, Color(255, 255, 255),0.0f,1.0f);
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

	void add_xz_plane(PCTMeshData* data, float width, float height) {
		float hx = width * 0.5f;
		float hy = 0.0f;
		float hz = height * 0.5f;
		float u2 = 1.0f;//width;
		float v2 = 1.0f;//height;
		// top
		Quad<PCTVertex> plane;
		plane.v[0] = PCTVertex(hx, hy, hz, 0.0f, v2);
		plane.v[1] = PCTVertex(hx, hy, -hz, 0.0f, 0.0f);
		plane.v[2] = PCTVertex(-hx, hy, -hz, u2, 0.0f);
		plane.v[3] = PCTVertex(-hx, hy, hz, u2, v2);
		data->addQuad(plane);
	}

	void add_grid(PCTMeshData* data, float cellSize, int countX, int countY,const Rect& textureRect,float textureSize) {
		float x = countX * cellSize * -0.5f + cellSize;
		float y = countY * cellSize * -0.5f + cellSize;
		float u1,v1,u2,v2;

		getTextureCoordinates(textureRect,textureSize,textureSize,&u1,&v1,&u2,&v2,true);
		for (int sx = 0; sx < countX; ++sx) {			
			for (int sy = 0; sy < countY; ++sy) {
				//LOG << "creating at " << x << " " << y;
				float nx = x + sx * cellSize;
				float ny = y + sy * cellSize;
				Quad<PCTVertex> plane;
				plane.v[0] = PCTVertex(nx           , ny - cellSize, 0.0f, u1, v2);
				plane.v[1] = PCTVertex(nx           , ny           , 0.0f, u1, v1);
				plane.v[2] = PCTVertex(nx - cellSize, ny           , 0.0f, u2, v1);
				plane.v[3] = PCTVertex(nx - cellSize, ny - cellSize, 0.0f, u2, v2);
				data->addQuad(plane);
				//x += cellSize;
			}
			//y += cellSize;
		}
	}

	void add_quad(PCTMeshData* data,Vector3f* vertices,const Rect& textureRect,float textureSize) {
		float u1,v1,u2,v2;
		getTextureCoordinates(textureRect,textureSize,textureSize,&u1,&v1,&u2,&v2,true);
		Quad<PCTVertex> plane;
		plane.v[0] = PCTVertex(vertices[0].x, vertices[0].y, vertices[0].z, u1, v2);
		plane.v[1] = PCTVertex(vertices[1].x, vertices[1].y, vertices[1].z, u1, v1);
		plane.v[2] = PCTVertex(vertices[2].x, vertices[2].y, vertices[2].z, u2, v1);
		plane.v[3] = PCTVertex(vertices[3].x, vertices[3].y, vertices[3].z, u2, v2);
		data->addQuad(plane);
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
		plane.v[0] = PCTVertex(  start.x, start.y,  start.z, Color(255, 0, 255), 0.0f,v2);
		plane.v[1] = PCTVertex(  sn.x, sn.y,sn.z, Color(255, 0, 255), 0.0f,0.0f);
		plane.v[2] = PCTVertex(  en.x, en.y, en.z, Color(255, 0, 255), u2,0.0f);
		plane.v[3] = PCTVertex(  end.x, end.y, end.z, Color(255, 0, 255), u2,v2);
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
		plane.v[0] = PCVertex(  hx, hy,  hz, Color(255, 0, 0));
		plane.v[1] = PCVertex(  hx, hy, -hz, Color(255, 0, 0));
		plane.v[2] = PCVertex( -hx, hy, -hz, Color(255, 0, 0));
		plane.v[3] = PCVertex( -hx, hy,  hz, Color(255, 0, 0));
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
		plane.v[0] = PCVertex(  start.x, start.y,  start.z, Color(192, 0, 0));
		plane.v[1] = PCVertex(  sn.x, sn.y,sn.z, Color(192, 0, 0));
		plane.v[2] = PCVertex(  en.x, en.y, en.z, Color(192, 0, 0));
		plane.v[3] = PCVertex(  end.x, end.y, end.z, Color(192, 0, 0));
		data.addQuad(plane);

	}

	void load_mesh(PCTMeshData* data,const char* fileName,float textureSize) {
		JSONReader reader;
		if ( reader.parse(fileName) ) {
			std::vector<Category*> categories = reader.getCategories();
			Vector3f vertices[4];
			for ( size_t i = 0; i < categories.size(); ++i ) {
				Category* c = categories[i];
				vertices[0] = c->getVector3f("v0");
				vertices[1] = c->getVector3f("v1");
				vertices[2] = c->getVector3f("v2");
				vertices[3] = c->getVector3f("v3");
				Rect r;
				c->getRect("texture",&r);
				data::add_quad(data,vertices,r,textureSize);
			}
		}
		else {
			LOGE << "File not found";
		}
	}

	// -----------------------------------------------------------
	// build cube
	// -----------------------------------------------------------
	void build_cube(PCMeshData* data,float dx,float dy,float dz) {
		float hx = dx * 0.5f;
		float hy = dy * 0.5f;
		float hz = dz * 0.5f;
		// front
		Quad<PCVertex> front;
		front.v[0] = PCVertex( hx, -hy, hz, Color(255, 255, 255));
		front.v[1] = PCVertex( hx,  hy, hz, Color(255, 255, 2555));
		front.v[2] = PCVertex(-hx,  hy, hz, Color(255, 255, 255));
		front.v[3] = PCVertex(-hx, -hy, hz, Color(255, 255, 255));
		data->addQuad(front);
		// back
		Quad<PCVertex> back;
		back.v[0] = PCVertex( -hx, -hy, -hz, Color(255, 255, 255));
		back.v[1] = PCVertex( -hx,  hy, -hz, Color(255, 255, 255));
		back.v[2] = PCVertex(  hx,  hy, -hz, Color(255, 255, 255));
		back.v[3] = PCVertex(  hx, -hy, -hz, Color(255, 255, 255));
		data->addQuad(back);
		// left
		Quad<PCVertex> left;
		left.v[0] = PCVertex( hx, -hy, -hz, Color(255, 255, 255));
		left.v[1] = PCVertex( hx,  hy, -hz, Color(255, 255, 255));
		left.v[2] = PCVertex( hx,  hy,  hz, Color(255, 255, 255));
		left.v[3] = PCVertex( hx, -hy,  hz, Color(255, 255, 255));
		data->addQuad(left);
		// right
		Quad<PCVertex> right;
		right.v[0] = PCVertex( -hx, -hy, hz, Color(255, 255, 255));
		right.v[1] = PCVertex( -hx,  hy, hz, Color(255, 255, 255));
		right.v[2] = PCVertex( -hx,  hy, -hz, Color(255, 255, 255));
		right.v[3] = PCVertex( -hx, -hy, -hz, Color(255, 255, 255));
		data->addQuad(right);
		// top
		Quad<PCVertex> top;
		top.v[0] = PCVertex(  hx, hy,  hz, Color(255, 255, 255));
		top.v[1] = PCVertex(  hx, hy, -hz, Color(255, 255, 255));
		top.v[2] = PCVertex( -hx, hy, -hz, Color(255, 255, 255));
		top.v[3] = PCVertex( -hx,hy,  hz, Color(255, 255, 2550));
		data->addQuad(top);

		// bottom
		Quad<PCVertex> bottom;
		bottom.v[0] = PCVertex( hx, -hy,-hz, Color(255, 255, 255));
		bottom.v[1] = PCVertex( hx, -hy, hz, Color(255, 255, 255));
		bottom.v[2] = PCVertex(-hx, -hy, hz, Color(255, 255, 255));
		bottom.v[3] = PCVertex(-hx, -hy,-hz, Color(255, 255, 255));
		data->addQuad(bottom);
	}
}
