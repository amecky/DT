#pragma once
#include "..\renderer\render_types.h"
#include <vector>

class GUIObject {

public:

	virtual void render() = 0;
protected:
	v2 _position;
};

class GUIDialog {

public:
	GUIDialog();
	~GUIDialog();
private:

};