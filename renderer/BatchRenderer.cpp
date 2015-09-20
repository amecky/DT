#include "BatchRenderer.h"
#include "..\renderer\render_types.h"
#include "..\renderer\VertexIndexBuffer.h"
#include "..\renderer\Shader.h"
#include "..\renderer\gfx.h"
#include "..\math\mathutils.h"
#include "..\utils\Log.h"
#include <assert.h>
#include "..\utils\Profiler.h"

namespace batch {

	const int MAX_QUADS = 8192;

	struct BatchContext {

		int maxSprites;
		PCTVertex vertices[MAX_QUADS * 4];
		int size;
		int index;
		int maxVertices;
		VertexIndexBuffer* buffer;
		DefaultShader* shader;
		int texture;
		int blendState;
		D3DXMATRIX world;
		BatchContext() : size(0) {}

		~BatchContext() {}
	};

	static BatchContext* batchCtx = 0;

	

	bool intialize(const char* textureName) {
		batchCtx = new BatchContext;
		batchCtx->size = 0;
		batchCtx->index = 0;
		batchCtx->maxVertices = MAX_QUADS * 4;
		batchCtx->buffer = gfx::createQuadBuffer(MAX_QUADS * 4, sizeof(PCTVertex));
		batchCtx->shader = gfx::getDefaultShader();
		batchCtx->texture = assets::loadTexture(textureName);
		D3DXMatrixIdentity(&batchCtx->world);
		assert(batchCtx->texture != -1);
		batchCtx->blendState = gfx::createBlendState(D3D11_BLEND_SRC_ALPHA,D3D11_BLEND_INV_SRC_ALPHA,D3D11_BLEND_ONE,D3D11_BLEND_ZERO);
		return true;
	}

	void setWorldMatrix(const D3DXMATRIX& world) {
		batchCtx->world = world;
	}

	void shutdown() {
		delete batchCtx->buffer;
		delete batchCtx;
		batchCtx = 0;
	}

	void begin() {
		assert(batchCtx != 0);
		batchCtx->size = 0;
		batchCtx->index = 0;
	}

	void end() {
		assert(batchCtx != 0);
		if (batchCtx->size > 0) {
			PR_START("vertices")
			gfx::fillQuadBuffer(batchCtx->buffer,batchCtx->vertices,batchCtx->index);
			gfx::submitBuffer(batchCtx->buffer);
			gfx::setBlendState(batchCtx->blendState);
			int indexCount = batchCtx->index / 4 * 6;
			batchCtx->shader->setWorldMatrix(batchCtx->world);
			gfx::renderShader(batchCtx->shader,batchCtx->texture,indexCount);
			debug::addSprites(batchCtx->size);
			PR_END("vertices")
		}
	}

	void flush() {
		assert(batchCtx != 0);
		end();
		begin();
	}

	void drawQuad(v3* vertices, const Texture& tex, const Color& color) {
		assert(batchCtx != 0);
		int vertexCount = batchCtx->index;
		if ((vertexCount + 4) >= batchCtx->maxVertices  ) {
			//renderer::setTexture(tex.textureID);
			flush();
		}
		int idx = batchCtx->index;
		batchCtx->vertices[idx].u = tex.uv.x;
		batchCtx->vertices[idx].v = tex.uv.w;
		batchCtx->vertices[idx + 1].u = tex.uv.x;
		batchCtx->vertices[idx + 1].v = tex.uv.y;
		batchCtx->vertices[idx + 2].u = tex.uv.z;
		batchCtx->vertices[idx + 2].v = tex.uv.y;
		batchCtx->vertices[idx + 3].u = tex.uv.z;
		batchCtx->vertices[idx + 3].v = tex.uv.w;
		v2 p(0, 0);
		for (int i = 0; i < 4; ++i) {
			batchCtx->vertices[idx + i].x = vertices[i].x;
			batchCtx->vertices[idx + i].y = vertices[i].y;
			batchCtx->vertices[idx + i].z = vertices[i].z;
			batchCtx->vertices[idx + i].color = color;
		}
		batchCtx->index += 4;
		++batchCtx->size;
	}
}