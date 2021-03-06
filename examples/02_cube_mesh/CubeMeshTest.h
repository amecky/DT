#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include "..\..\renderer\VertexIndexBuffer.h"
#include "..\..\renderer\Shader.h"

class CubeMeshTest : public BaseApp {

public:
	CubeMeshTest();
	~CubeMeshTest();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	PCTMeshData* _data;
	PCTMeshData* _floorData;
	int _textureID;
	int _floorTexID;
	bool _rotating;
	bool _moving;
	bool _scaling;
	float _timer;
	v3 _position;
	v3 _scale;
	v3 _rotation;
	VertexIndexBuffer* _buffer;
	DefaultShader* _shader;
	D3DXMATRIX _worldMatrix;
};

