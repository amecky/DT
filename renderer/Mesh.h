#pragma once
#include <d3dx9math.h>
#include "render_types.h"
#include "gfx.h"
#include "VertexIndexBuffer.h"

class MeshData {

public:
	explicit MeshData(int buffer_id,int vertex_declaration,int _max) : _buffer_id(buffer_id) , _vertex_declaration(vertex_declaration) , _size(0) , _maxVertices(_max) , _textureID(-1) {}
	virtual ~MeshData() {}
	void clear() {
		_size = 0;
	}
	void setTextureID(int tid) {
		_textureID = tid;
	}
	int getTextureID() const {
		return _textureID;
	}
	virtual void fillBuffer(VertexIndexBuffer* buffer) = 0;
	int getBufferID() const {
		return _buffer_id;
	}
	int getVertexDeclaration() const {
		return _vertex_declaration;
	}
protected:
	int _vertex_declaration;
	int _buffer_id;
	int _size;
	int _maxVertices;
	int _textureID;	
};

template<class T>
class AbstractMeshData : public MeshData {

public:
	AbstractMeshData(int buffer_id,int vertex_declaration,int max) : MeshData(buffer_id,vertex_declaration,max) {
		_data = new T[max];
		_indexCount = 0;
	}
	virtual ~AbstractMeshData() {
		delete[] _data;
	}
	void addQuad(Quad<T> quad) {
		if ( _size + 4 < _maxVertices ) {
			_data[_size++] = quad.v[0];
			_data[_size++] = quad.v[1];
			_data[_size++] = quad.v[2];
			_data[_size++] = quad.v[3];
			_indexCount += 6;
		}
	}
	void addQuad(const T& v0,const T& v1,const T& v2,const T& v3) {
		if ( _size + 4 < _maxVertices ) {
			_data[_size++] = v0;
			_data[_size++] = v1;
			_data[_size++] = v2;
			_data[_size++] = v3;
			_indexCount += 6;
		}
	}
	void fillBuffer(VertexIndexBuffer* buffer) {
		gfx::fillQuadBuffer(buffer,_data,_size);
	}
	T* getData() const {
		return _data;
	}
	int getIndexCount() const {
		return _indexCount;
	}
private:
	T* _data;
	int _indexCount;
};

typedef AbstractMeshData<PCTVertex> PCTMeshData;
typedef AbstractMeshData<PNTCVertex> PNTCMeshData;
typedef AbstractMeshData<PCVertex> PCMeshData;
typedef AbstractMeshData<PTVertex> PTMeshData;

namespace data {

	void load_mesh(PCTMeshData* data,const char* fileName,float textureSize);

	void load_mesh(PNTCMeshData* data,const char* fileName,float textureSize);

	void add_quad(PNTCMeshData* data,v3* vertices,const v3& n,const Rect& textureRect,float textureSize);
	
	void add_quad(PCTMeshData* data,Vector3f* vertices,const Rect& textureRect,float textureSize);

	void build_cube(PCTMeshData* data,float dx,float dy,float dz,const Rect& textureRect,float textureSize);

	void build_cube(PCTMeshData& data,float dx,float dy,float dz);

	void build_cube(PCMeshData* data,float dx,float dy,float dz);

	void build_unit_cube(PCTMeshData& data);

	void add_xz_plane(PCTMeshData& data,float width,float height);

	void add_xz_plane(PCTMeshData* data, float width, float height);

	void add_line(PCTMeshData& data,const Vector3f& start,const Vector3f& end,float thickness);

	void add_xz_plane(PCMeshData& data,float width,float height);

	void add_grid(PCTMeshData* data, float cellSize, int countX, int countY,const Rect& textureRect,float textureSize);

	void add_line(PCMeshData& data,const Vector3f& start,const Vector3f& end,float thickness);

	void getTextureCoordinates(const Rect& textureRect,int textureWidth,float textureHeight,float* u1,float* v1,float* u2,float* v2,bool useHalfTexel);
}

struct Mesh {

	MeshData* data;
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;
	D3DXMATRIX world;

	Mesh(MeshData* _data) : data(_data) {
		D3DXMatrixIdentity(&world);
	}
};

