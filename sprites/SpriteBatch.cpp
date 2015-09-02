#include "SpriteBatch.h"
#include "..\renderer\render_types.h"
#include "..\renderer\VertexIndexBuffer.h"
#include "..\renderer\Shader.h"
#include "..\renderer\gfx.h"
#include "..\renderer\Mesh.h"
#include "..\math\mathutils.h"
#include "..\utils\Log.h"
#include <assert.h>

namespace sprites {

	const int MAX_SPRITES = 8192;

	const float VP_ARRAY[] = {
		-0.5f,0.5f, 0.5f,0.5f, 
		0.5f,-0.5f, -0.5f,-0.5f
	};

	struct SpriteBatchContext {

		int maxSprites;
		PCTVertex sprites[MAX_SPRITES * 4];
		int size;
		int index;
		int maxVertices;
		VertexIndexBuffer* buffer;
		Shader* shader;
		TextureAsset* texture;
		FontDefinition fontDefinition;

		SpriteBatchContext() : size(0) {}

		~SpriteBatchContext() {}
	};

	static SpriteBatchContext* spriteCtx = 0;

	

	bool intialize(const char* textureName) {
		spriteCtx = new SpriteBatchContext;
		spriteCtx->size = 0;
		spriteCtx->index = 0;
		spriteCtx->maxVertices = MAX_SPRITES * 4;
		spriteCtx->buffer = gfx::createQuadBuffer(MAX_SPRITES * 4,sizeof(PCTVertex));
		spriteCtx->shader = gfx::createShader("texture.vs","texture.ps");
		spriteCtx->texture = gfx::loadTexture(textureName);//"content\\ref_256.png");
		

		spriteCtx->fontDefinition.startChar = 32;// " : "32",
		spriteCtx->fontDefinition.endChar = 128;// " : "128",
		spriteCtx->fontDefinition.charHeight = 14;// " : "14",
		spriteCtx->fontDefinition.gridHeight = 21;// " : "21",
		spriteCtx->fontDefinition.startX = 0;// " : "0",
		spriteCtx->fontDefinition.startY = 100;// " : "300",
		spriteCtx->fontDefinition.width = 405;// " : "405",
		spriteCtx->fontDefinition.height = 168;// " : "168",
		spriteCtx->fontDefinition.padding = 6;// " : "6",
		spriteCtx->fontDefinition.textureSize = 512;// " : "1024"

		gfx::initializeBitmapFont(spriteCtx->fontDefinition, spriteCtx->texture, Color(255, 0, 255, 255));
		return true;
	}

	void shutdown() {
		delete spriteCtx->texture;
		delete spriteCtx->shader;
		delete spriteCtx->buffer;
		delete spriteCtx;
		spriteCtx = 0;
	}

	void begin() {
		assert(spriteCtx != 0);
		spriteCtx->size = 0;
		spriteCtx->index = 0;
	}

	void end() {
		assert(spriteCtx != 0);
		if (spriteCtx->size > 0) {
			//PR_START("sprites")
			gfx::fillQuadBuffer(spriteCtx->buffer,spriteCtx->sprites,spriteCtx->index);
			gfx::turnZBufferOff();
			gfx::submitBuffer(spriteCtx->buffer);
			int indexCount = spriteCtx->index / 4 * 6;
			gfx::renderShader(spriteCtx->shader,spriteCtx->texture,indexCount);
			//PR_END("sprites")
		}
	}

	void flush() {
		assert(spriteCtx != 0);
		end();
		begin();
	}

	v2 calculateTextSize(const char* text,int padding) {
		int len = strlen(text);
		int xp = 0;
		for (int i = 0; i < len; ++i ) {
			if (text[i] != '\0') {
				CharDef cd = spriteCtx->fontDefinition.definitions[text[i]];
				if (cd.ascii != -1) {
					xp += cd.width;
					xp += padding;
				}
			}
		}
		return v2(xp,spriteCtx->fontDefinition.charHeight);
	}

	void drawText(const char* text, int x, int y,const Color& color,int padding) {
		int len = strlen(text);
		int xp = x;
		for (int i = 0; i < len; ++i ) {
			if (text[i] != '\0') {
				CharDef cd = spriteCtx->fontDefinition.definitions[text[i]];
				if (cd.ascii != -1) {
					draw(v2(xp, y), cd.texture,0.0f,1.0f,1.0f,color);
					xp += cd.width;
					xp += padding;
				}
			}
		}
	}

	void draw(const v2& pos,const Texture& tex, float rotation, float scaleX, float scaleY,const Color& color) {
		assert(spriteCtx != 0);
		int vertexCount = spriteCtx->index;
		if ((vertexCount + 4) >= spriteCtx->maxVertices  ) {
			//renderer::setTexture(tex.textureID);
			flush();
		}
		int idx = spriteCtx->index;
		spriteCtx->sprites[idx].u = tex.uv.x;
		spriteCtx->sprites[idx].v = tex.uv.y;
		spriteCtx->sprites[idx + 1].u = tex.uv.z;
		spriteCtx->sprites[idx + 1].v = tex.uv.y;
		spriteCtx->sprites[idx + 2].u = tex.uv.z;
		spriteCtx->sprites[idx + 2].v = tex.uv.w;
		spriteCtx->sprites[idx + 3].u = tex.uv.x;
		spriteCtx->sprites[idx + 3].v = tex.uv.w;
		Vector2f cor = pos;
		cor = cor - v2(400,300);//ds::renderer::getSelectedViewport().getPosition();
		v2 p(0, 0);
		for (int i = 0; i < 4; ++i) {
			p.x = VP_ARRAY[i * 2] * tex.dim.x;
			p.y = VP_ARRAY[i * 2 + 1] * tex.dim.y;
			//p = p - center;
			Vector2f np = math::srt(cor, p, scaleX, scaleY, rotation);
			spriteCtx->sprites[idx + i].x = np.x;
			spriteCtx->sprites[idx + i].y = np.y;
			spriteCtx->sprites[idx + i].z = 0.0f;
			spriteCtx->sprites[idx + i].color = color;
		}
		spriteCtx->index += 4;
		++spriteCtx->size;
	}
}