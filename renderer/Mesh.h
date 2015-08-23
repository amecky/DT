#pragma once
#include "DX.h"
#include <d3dx9math.h>

class VIBuffer;

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
	virtual void fillBuffer(VIBuffer* buffer) = 0;
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
	}
	~AbstractMeshData() {
		delete[] _data;
	}
	void addQuad(Quad<T> quad) {
		if ( _size + 4 < _maxVertices ) {
			_data[_size++] = quad.v[0];
			_data[_size++] = quad.v[1];
			_data[_size++] = quad.v[2];
			_data[_size++] = quad.v[3];
		}
	}
	void fillBuffer(VIBuffer* buffer) {
		buffer->fillBuffer(_data,_size);
	}
private:
	T* _data;
};

typedef AbstractMeshData<PCTVertex> PCTMeshData;
typedef AbstractMeshData<PCVertex> PCMeshData;

namespace data {

	void build_cube(PCTMeshData& data,float dx,float dy,float dz);

	void build_unit_cube(PCTMeshData& data);

	void add_xz_plane(PCTMeshData& data,float width,float height);

	void add_xz_plane(PCTMeshData* data, float width, float height);

	void add_line(PCTMeshData& data,const Vector3f& start,const Vector3f& end,float thickness);

	void add_xz_plane(PCMeshData& data,float width,float height);

	void add_grid(PCTMeshData* data, float cellSize, int countX, int countY);

	void add_line(PCMeshData& data,const Vector3f& start,const Vector3f& end,float thickness);
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

