#include "SpriteBatch.h"
#include "..\renderer\render_types.h"
#include "..\renderer\VertexIndexBuffer.h"
#include "..\renderer\Shader.h"
#include "..\renderer\gfx.h"
#include "..\renderer\Mesh.h"
#include <assert.h>

namespace sprites {

	const int MAX_SPRITES = 8192;

	const float VP_ARRAY[] = {
		-0.5f,0.5f, 0.5f,0.5f, 
		0.5f,-0.5f, -0.5f,-0.5f
	};

	struct SpriteBatchContext {

		int maxSprites;
		PTVertex sprites[MAX_SPRITES * 4];
		int size;
		int index;
		int maxVertices;
		VertexIndexBuffer* buffer;
		Shader* shader;
		TextureAsset* texture;

		SpriteBatchContext() : size(0) {}

		~SpriteBatchContext() {}
	};

	static SpriteBatchContext* spriteCtx = 0;

	Vector4f getTextureCoordinates(const Rect& textureRect,float textureWidth,float textureHeight,bool useHalfTexel) {
		Vector4f ret;
		if ( useHalfTexel ) {
			float halfTexel = 0.5f;
			float const width   = textureWidth;
			float const height  = textureHeight;

			float tw = textureRect.width() / textureWidth;
			float th = textureRect.height() / textureHeight;

			float kUOffset = halfTexel/width;
			float kVOffset = halfTexel/height;

			ret.x = textureRect.left/width  + kUOffset;
			ret.y = textureRect.top/height + kVOffset;  

			ret.z = ret.x + tw   - 2.0f*kUOffset;
			ret.w = ret.y + th  - 2.0f*kVOffset;
		}
		else {
			ret.x = textureRect.left/textureWidth;
			ret.z = textureRect.right/textureWidth;
			ret.y = textureRect.top/textureHeight;
			ret.w = textureRect.bottom/textureHeight;
		}
		return ret;
	}

	Texture buildTexture(const Rect& r, float textureWidth, float textureHeight, bool useHalfTexel) {
		Texture ret;
		ret.uv = getTextureCoordinates(r, textureWidth, textureHeight,true);
		ret.textureID = 0;
		ret.dim = Vector2f(r.width(), r.height());
		return ret;
	}

	bool intialize(const char* textureName) {
		spriteCtx = new SpriteBatchContext;
		spriteCtx->size = 0;
		spriteCtx->index = 0;
		spriteCtx->maxVertices = MAX_SPRITES * 4;
		spriteCtx->buffer = gfx::createQuadBuffer(MAX_SPRITES * 4,sizeof(PTVertex));
		spriteCtx->shader = gfx::createShader("texture.vs","texture.ps");
		spriteCtx->texture = gfx::loadTexture(textureName);//"content\\ref_256.png");
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

	v2 srt(const Vector2f& v,const Vector2f& u,float scaleX,float scaleY,float rotation) {
		float sx = u.x * scaleX;
		float sy = u.y * scaleY;

		// rotation clock wise
		//float xt = cosf(rotation) * sx + sinf(rotation) * sy;
		//float yt = -sinf(rotation) * sx + cosf(rotation) * sy;

		// rotation counter clock wise
		//float xt = cosf(rotation) * sx - sinf(rotation) * sy;
		//float yt = sinf(rotation) * sx + cosf(rotation) * sy;

		float xt = cos(rotation) * sx - sin(rotation) * sy;
		float yt =sin(rotation) * sx + cos(rotation) * sy;

		xt += v.x;
		yt += v.y;

		return Vector2f(xt,yt);
	}

	void draw(const v2& pos,const Texture& tex, float rotation, float scaleX, float scaleY) {
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
		Vector2f p(0, 0);
		for (int i = 0; i < 4; ++i) {
			p.x = VP_ARRAY[i * 2] * tex.dim.x;
			p.y = VP_ARRAY[i * 2 + 1] * tex.dim.y;
			//p = p - center;
			Vector2f np = srt(cor, p, scaleX, scaleY, rotation);
			spriteCtx->sprites[idx + i].x = np.x;
			spriteCtx->sprites[idx + i].y = np.y;
			spriteCtx->sprites[idx + i].z = 0.0f;
			//spriteCtx->sprites[idx + i].color = color;
		}
		spriteCtx->index += 4;
		++spriteCtx->size;
	}
}