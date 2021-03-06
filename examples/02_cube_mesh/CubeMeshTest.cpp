#include "..\..\StdAfx.h"
#include "CubeMeshTest.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\gfx.h"
#include "..\..\World.h"
#include "..\..\utils\JSONReader.h"

//BaseApp *app = new CubeMeshTest();

CubeMeshTest::CubeMeshTest() {
	_settings.tickCamera = true;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	//_settings.clearColor = Color(0,0,0);
}


CubeMeshTest::~CubeMeshTest() {
	delete _floorData;
	delete _data;
	delete _buffer;
}


void CubeMeshTest::loadContent() {
	_textureID = assets::loadTexture("ref_256");
	_floorTexID = assets::loadTexture("UVCheckerMap06-512");
	_buffer = gfx::createQuadBuffer(1024,sizeof(PCTVertex));
	_shader = gfx::getDefaultShader();

	_data = new PCTMeshData(0, 0, 64);
	_data->setTextureID(_textureID);

	_floorData = new PCTMeshData(0, 0, 64);
	_floorData->setTextureID(_textureID);

	_rotating = false;
	_moving = false;
	_scaling = false;
	_timer = 0.0f;

	LOG << "Key mappings:";
	LOG << "'1' - toggle rotation";
	LOG << "'2' - toggle scale";
	LOG << "'3' - toggle movement";
	LOG << "'4' - reset";
	_position = v3(0,0,0);
	_scale = v3(1,1,1);
	_rotation = v3(0,0,0);

	data::load_mesh(_data,"test.mesh",256.0f);	

	_floorData->addQuad(PCTVertex(v3(-4.0f,-4.0f,0.0f),0.0f,1.0f),PCTVertex(v3(-4.0f,4.0f,0.0f),0.0f,0.0f),PCTVertex(v3(-4.0f,4.0f,8.0f),1.0f,0.0f),PCTVertex(v3(-4.0f,-4.0f,8.0f),1.0f,1.0f));
	_floorData->addQuad(PCTVertex(v3(4.0f,-4.0f,8.0f),0.0f,1.0f),PCTVertex(v3(4.0f,4.0f,8.0f),0.0f,0.0f),PCTVertex(v3(4.0f,4.0f,0.0f),1.0f,0.0f),PCTVertex(v3(4.0f,-4.0f,0.0f),1.0f,1.0f));
	_floorData->addQuad(PCTVertex(v3(-4.0f,-4.0f,8.0f),0.0f,1.0f),PCTVertex(v3(-4.0f,4.0f,8.0f),0.0f,0.0f),PCTVertex(v3(4.0f,4.0f,8.0f),1.0f,0.0f),PCTVertex(v3(4.0f,-4.0f,8.0f),1.0f,1.0f));
	_floorData->addQuad(PCTVertex(v3(-4.0f,-4.0f,0.0f),0.0f,1.0f),PCTVertex(v3(-4.0f,-4.0f,8.0f),0.0f,0.0f),PCTVertex(v3(4.0f,-4.0f,8.0f),1.0f,0.0f),PCTVertex(v3(4.0f,-4.0f,0.0f),1.0f,1.0f));
	_floorData->addQuad(PCTVertex(v3(-4.0f,4.0f,8.0f),0.0f,1.0f),PCTVertex(v3(-4.0f,4.0f,0.0f),0.0f,0.0f),PCTVertex(v3(4.0f,4.0f,0.0f),1.0f,0.0f),PCTVertex(v3(4.0f,4.0f,8.0f),1.0f,1.0f));

	D3DXMatrixIdentity(&_worldMatrix);

}

void CubeMeshTest::tick(float dt) {
	_timer += dt;
	D3DXMATRIX t;
	D3DXMATRIX s;
	D3DXMATRIX rx;
	D3DXMATRIX ry;
	D3DXMATRIX rz;
	D3DXMatrixTranslation(&t, _position.x,_position.y,_position.z);	
	D3DXMatrixScaling(&s,_scale.x,_scale.y,_scale.z);
	D3DXMatrixRotationX(&rx,_rotation.x);
	D3DXMatrixRotationY(&ry,_rotation.y);
	D3DXMatrixRotationZ(&rz,_rotation.z);

	_worldMatrix = s * rz * ry * rx * t;
	if ( _rotating ) {
		_rotation.z = _timer;
	}
	if ( _scaling ) {
		_scale.x = 1.0f + sin(_timer) * 0.2f;
		_scale.y = _scale.x;
		_scale.z = _scale.x;
	}
	if ( _moving ) {
		_position.x = 2.0f * cos(_timer);
		_position.y = 2.0f * sin(_timer);
	}
}

void CubeMeshTest::render() {
	_floorData->fillBuffer(_buffer);
	gfx::submitBuffer(_buffer);
	D3DXMATRIX w;
	D3DXMatrixIdentity(&w);
	_shader->setWorldMatrix(w);
	gfx::renderShader(_shader,_floorTexID,_floorData->getIndexCount());

	_data->fillBuffer(_buffer);
	gfx::submitBuffer(_buffer);
	_shader->setWorldMatrix(_worldMatrix);
	gfx::renderShader(_shader,_textureID,_data->getIndexCount());
}

void CubeMeshTest::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
		_rotating = !_rotating;
	}
	if ( ascii == '2' ) {
		_scaling = !_scaling;
	}
	if ( ascii == '3' ) {
		_moving = !_moving;
	}
	if ( ascii == '4' ) {
		_rotating = false;
		_scaling = false;
		_moving = false;
		_position = v3(0,0,0);
		_scale = v3(1,1,1);
		_rotation = v3(0,0,0);
	}
}