#include "shader.h"
#include "..\utils\StringUtils.h"
#include "..\utils\Log.h"

#ifdef DEBUG
DWORD SHADER_FLAGS = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG;
#else
DWORD SHADER_FLAGS = D3DXFX_NOT_CLONEABLE;
#endif

Shader::Shader(DX* dx) : _dx(dx) , _hashName(0), _FX(0), _constants(0), _constantCount(0) {
	
}

bool Shader::setFloat(const char* name, float value) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetFloat(handle, value);
		return true;
	}
	return false;
}

bool Shader::setVector3f(const char* name, const Vector3f& v) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetValue(handle, &v, sizeof(Vector3f));
		return true;
	}
	return false;
}

bool Shader::setVector2f(const char* name, const Vector2f& v) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetValue(handle, &v, sizeof(Vector2f));
		return true;
	}
	return false;
}

bool Shader::setValue(const char* name, void* data, UINT size) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetValue(handle, &data, size);
		return true;
	}
	return false;
}

bool Shader::setTexture(const char* name, int textureID) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetTexture(handle, _dx->getTexture(textureID));
		return true;
	}
	return false;
}
/*
bool Shader::setMatrix(const char* name, const mat4& m) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetValue(handle, m,sizeof(mat4));
		return true;
	}
	return false;
}

bool Shader::setColor(const char* name, const Color& color) {
	D3DXHANDLE handle = findHandle(name);
	if (handle != 0) {
		_FX->SetValue(handle, &color, sizeof(Color));
		return true;
	}
	return false;
}
*/
bool Shader::contains(const char* name) {
	return findHandle(name) != 0;
}

D3DXHANDLE Shader::findHandle(const char* name) {
	IdString hashName = string::murmur_hash(name);
	ShaderConstant* sh = _constants;
	for (uint32 i = 0; i < _constantCount; ++i) {
		if (hashName == sh->name) {
			return sh->handle;
		}
		++sh;
	}
	return 0;
}

void Shader::initialize(const char* techName) {
	LOGC("renderer") << "initializing shader using tech: " << techName;
	_hTech = _FX->GetTechniqueByName(techName);
	D3DXEFFECT_DESC effectDesc;
	_FX->GetDesc(&effectDesc);
	UINT nc = effectDesc.Parameters;
	_constants = new ShaderConstant[nc];
	_constantCount = nc;
	LOGC("renderer") << "Got Description - number of parameters: " << nc;
	for (UINT i = 0; i < effectDesc.Parameters; ++i) {
		D3DXHANDLE hParam = _FX->GetParameter(NULL, i);
		D3DXPARAMETER_DESC pDesc;
		// get parameter description
		_FX->GetParameterDesc(hParam, &pDesc);
		LOGC("renderer") << "Parameter : " << pDesc.Name << " Type: " << pDesc.Type;
		_constants[i].handle = hParam;
		_constants[i].name = string::murmur_hash(pDesc.Name);
	}
	LOGC("renderer") << "Shader finally loaded";

}

uint32 Shader::start() {
	HR(_FX->SetTechnique(_hTech));
	UINT numPasses = 0;
	HR(_FX->Begin(&numPasses, 0));
	return numPasses;
}

void Shader::end() {
	HR(_FX->End());
}

void Shader::release() {
	delete[] _constants;
	//SAFE_RELEASE(_FX);
}

bool Shader::loadShader(const char* fxName, const char* techName) {
	char fileName[256];
	sprintf(fileName, "content\\%s.fx", fxName);
	ID3DXBuffer* errors = 0;
	//HR(D3DXCreateEffectFromFileA(_dx->getDevice(), fileName, 0, 0, SHADER_FLAGS, 0, &_FX, &errors));
	if (errors != 0) {
		LOGEC("Renderer") << "Error while loading shader: " << (char*)errors->GetBufferPointer();
		return false;
	}

	LOGC("Renderer") << "Shader created";
	initialize(techName);
	return true;
}

void Shader::beginPass(UINT p) {
	HR(_FX->BeginPass(p));
}

void Shader::endPass() {
	HR(_FX->EndPass());
}

void Shader::commitChanges() {
	HR(_FX->CommitChanges());
}
