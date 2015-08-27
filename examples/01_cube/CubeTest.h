#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"

class VertexIndexBuffer;
class Shader;
struct TextureAsset;

class CubeTest : public BaseApp {

public:
	CubeTest();
	~CubeTest();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	World* _world;
	PTMeshData* _data;
	MID _id;
	bool _rotating;
	bool _moving;
	bool _scaling;
	float _timer;

	VertexIndexBuffer* buffer;
	Shader* shader;
	TextureAsset* texture;
};

