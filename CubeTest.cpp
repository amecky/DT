#include "StdAfx.h"
#include "CubeTest.h"
#include "renderer\DX.h"
#include "renderer\render_types.h"
#include "renderer\Mesh.h"
#include "renderer\VIBuffer.h"
#include "World.h"

BaseApp *app = new CubeTest();

CubeTest::CubeTest() {
}


CubeTest::~CubeTest() {
	delete _data;
	delete _floor;
	delete _world;
}


void CubeTest::loadContent() {
	// PCT buffer
	BufferDescriptor desc;
	desc.declarationID = 0;
	desc.vertexSize = sizeof(PCTVertex);
	desc.type = 0;
	int id = _dx.createBuffer(desc);

	int tid = _dx.loadTexture("case");
	int blid = _dx.loadTexture("ref_256");

	_data = new PCTMeshData(id, 0, 64);
	data::build_cube(*_data, 1.0f, 1.0f, 1.0f);
	_data->setTextureID(tid);

	_floor = new PCTMeshData(id, 0, 256);
	_floor->setTextureID(blid);
	//data::build_cube(floor,12.0f,0.1f,12.0f);
	//data::add_xz_plane(_floor, 1.0f, 1.0f);
	//data::add_line(*_floor, Vector3f(4.0f, 0.0f, 0.0f), Vector3f(-4.0f, 2.0f, 0.0f), 0.4f);
	data::add_grid(_floor, 2.0f, 6, 6);

	_world = new World(&_dx);
	// create floor
	_world->create(Vector3f(0.0f, 0.0f, 0.0f), _floor);
	// create three cubes
	//w.create(Vector3f(0.0f,0.0f,0.0f),&data);
	//w.create(Vector3f(2.0f,0.0f,0.0f),&data);
	//w.create(Vector3f(-2.0f,0.0f,0.0f),&data);

	// animated cube
	//MID mi = _world->create(Vector3f(0.0f, -2.0f, 0.0f), _data);
	//w.rotate(mi,Vector3f(0.0f,0.0f,D3DX_PI/4.0f));
	//w.scale(mi,Vector3f(2.0f,2.0f,2.0f));

	/*
	BezierCurve curve;
	curve.p0 = Vector3f(8.0f, 2.0f, 0.0f);
	curve.p1 = Vector3f(2.0f, 6.0f, 0.0f);
	curve.p2 = Vector3f(-2.0f, -4.0f, 0.0f);
	curve.p3 = Vector3f(-8.0f, 2.0f, 0.0f);
	curve.build();
	*/

}

void CubeTest::tick(float dt) {
	_world->tick(dt);
}

void CubeTest::render() {
	_world->render();
}

void CubeTest::onChar(char ascii, unsigned int state) {

}