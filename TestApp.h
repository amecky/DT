#pragma once
#include "base\BaseApp.h"
#include "particle\ParticleSystem.h"

class TestApp : public BaseApp {

public:
	TestApp(void);
	virtual ~TestApp(void);

	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii,unsigned int state);
private:
	ParticleSystem* _particles;
	ParticleEmitter emitter;
	int _addBlendState;
};

