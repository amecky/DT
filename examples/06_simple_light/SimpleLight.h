#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include "..\..\renderer\VertexIndexBuffer.h"
#include "..\..\renderer\Shader.h"

struct LightBuffer	{
	v3 dir;
	Color ambient;
	Color diffuse;
	float value;
};


class SimpleLight : public BaseApp {

public:
	SimpleLight();
	~SimpleLight();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	PNTCMeshData* _data;
	//PCTMeshData* _floorData;
	int _textureID;
	//int _floorTexID;
	bool _rotating;
	bool _moving;
	bool _scaling;
	float _timer;
	v3 _position;
	v3 _scale;
	v3 _rotation;
	VertexIndexBuffer* _buffer;
	SimpleLightShader* _shader;
	D3DXMATRIX _worldMatrix;
	LightBuffer _lightBuffer;
	int _lightBufferIndex;
};

