#pragma once

#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "SubTexture.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void InitRenderer();
		static void BeginRender(Camera& camera);
		static void SubmitTile(const glm::vec3& position, std::shared_ptr<Texture> texture, float T1ileSize = 1.0f);
		static void SubmitTile(const glm::vec3& position, std::shared_ptr<SubTexture> SubTexture, float T1ileSize = 1.0f);
		static void SubmitData(float* vertices, uint32_t vertexSize, uint32_t* indices, uint32_t indicesCount, std::shared_ptr<Texture> texture);
		static void EndRender();
	private:
		static void BeginBatch();
		static void EndBatch();
		static void pushData(float* vertices, uint32_t vertexSize, uint32_t* indices, uint32_t indicesCount, std::shared_ptr<Texture> texture);
		static float SubmitTextureForBatch(std::shared_ptr<Texture> texture);
		static void DrawIndexed(std::shared_ptr<VertexArray>& VA);
	};

}