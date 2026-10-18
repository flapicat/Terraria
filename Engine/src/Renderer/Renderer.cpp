#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Engine
{
	struct RenderData
	{
		std::unique_ptr<Shader> shader;
		std::shared_ptr<VertexArray> VA;
		std::shared_ptr<VertexBuffer> VB;
		std::shared_ptr<IndexBuffer> IB;

		// Position(3) | UV(2) | TexIndex(1)
		static const uint32_t FloatsPerVertex = 6;
		static const uint32_t TexIndexOffset = 5;

		static const uint32_t MaxQuads = 10000;

		static const uint32_t MaxVerticesCount = MaxQuads * 4 * FloatsPerVertex; // in floats
		float BatchedVertices[MaxVerticesCount] = {};
		uint32_t VerticesCount = 0; // in floats

		static const uint32_t MaxIndicesCount = MaxQuads * 6;
		uint32_t BatchedIndices[MaxIndicesCount] = {};
		uint32_t IndicesCount = 0;

		uint32_t maxTexturesBoundAtTheTime = 0;
		std::shared_ptr<Texture> WhiteTexture;
		uint32_t WhiteTextureID = 0;
		std::vector<std::shared_ptr<Texture>> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
	};

	RenderData rData;

	static void APIENTRY OnOpenGLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:   LOG_ERROR("OpenGL: {}", message); break;
		case GL_DEBUG_SEVERITY_MEDIUM: LOG_WARN("OpenGL: {}", message); break;
		default:                       LOG_TRACE("OpenGL: {}", message); break;
		}
	}

	void Renderer::InitRenderer()
	{
		LOG_INFO("OpenGL {} on {} ({})",
			(const char*)glGetString(GL_VERSION),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VENDOR));

		if (GLAD_GL_VERSION_4_3)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OnOpenGLMessage, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		}
		else
		{
			LOG_WARN("OpenGL debug output needs OpenGL 4.3+, OpenGL errors will not be logged");
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		rData.shader.reset(new Shader("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"));
		rData.VA.reset(new VertexArray());

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float , "a_TexIndex" }
		};

		// Allocate GPU storage for a full batch once, EndBatch only uploads what's used
		rData.VB.reset(new VertexBuffer(RenderData::MaxVerticesCount));
		rData.IB.reset(new IndexBuffer(RenderData::MaxIndicesCount));
		rData.VA->AddVertexBuffer(rData.VB, layout);
		rData.VA->SetIndexBuffer(rData.IB);
		rData.VA->Unbind();

		rData.maxTexturesBoundAtTheTime = 32;
		rData.TextureSlots.resize(rData.maxTexturesBoundAtTheTime);

		int gpuTextureUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &gpuTextureUnits);
		if ((uint32_t)gpuTextureUnits < rData.maxTexturesBoundAtTheTime)
			LOG_WARN("GPU supports only {} textures per draw call but the renderer batches up to {}, textures in higher slots will not render",
				gpuTextureUnits, rData.maxTexturesBoundAtTheTime);

		unsigned char WhiteTextureData[4] = { 255,255,255,255 };
		rData.WhiteTexture.reset(new Texture(1, 1, WhiteTextureData));
		rData.WhiteTextureID = 0;
		rData.TextureSlots[0] = rData.WhiteTexture;

		std::vector<int> samplers;
		samplers.resize(rData.maxTexturesBoundAtTheTime);
		for (uint32_t i = 0; i < rData.maxTexturesBoundAtTheTime; i++)
			samplers[i] = i;

		rData.shader->Bind();
		rData.shader->SetIntArray("u_Textures", samplers.data(), rData.maxTexturesBoundAtTheTime);

		LOG_INFO("Renderer initialized: up to {} quads and {} textures per draw call", RenderData::MaxQuads, rData.maxTexturesBoundAtTheTime);
	}

	void Renderer::BeginRender(Camera& camera)
	{
		rData.shader->Bind();
		rData.shader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		BeginBatch();
	}

	void Renderer::SubmitTile(const glm::vec3& position, std::shared_ptr<SubTexture> SubTexture, float T1ileSize) {
		if (!SubTexture)
		{
			LOG_ERROR("Renderer: SubmitTile got a null SubTexture, tile at ({}, {}) skipped", position.x, position.y);
			return;
		}

		// Texture index is filled in by pushData
		//	Position	|	UV   |	 Index
		float vertices[] = {
			position.x + -1 * T1ileSize, position.y + -1 * T1ileSize, position.z,		SubTexture->GetUVMin().x, SubTexture->GetUVMin().y,		0.0f,
			position.x + -1 * T1ileSize, position.y +  1 * T1ileSize, position.z,		SubTexture->GetUVMin().x, SubTexture->GetUVMax().y,		0.0f,
			position.x +  1 * T1ileSize, position.y +  1 * T1ileSize, position.z,		SubTexture->GetUVMax().x, SubTexture->GetUVMax().y,		0.0f,
			position.x +  1 * T1ileSize, position.y + -1 * T1ileSize, position.z,		SubTexture->GetUVMax().x, SubTexture->GetUVMin().y,		0.0f,
		};
		uint32_t verticesCount = 6 * 4;

		// Local indices, pushData offsets them by the vertices already in the batch
		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
		uint32_t IndicesCount = 6;

		pushData(vertices, verticesCount, indices, IndicesCount, SubTexture->GetTexAtlas());
	}

	void Renderer::SubmitTile(const glm::vec3& position, std::shared_ptr<Texture> texture, float T1ileSize)
	{
		// Texture index is filled in by pushData
		//	Position	|	UV   |	 Index
		float vertices[] = {
			position.x + -1 * T1ileSize, position.y + -1 * T1ileSize, position.z,		0.0f, 0.0f,		0.0f,
			position.x + -1 * T1ileSize, position.y +  1 * T1ileSize, position.z,		0.0f, 1.0f,		0.0f,
			position.x +  1 * T1ileSize, position.y +  1 * T1ileSize, position.z,		1.0f, 1.0f,		0.0f,
			position.x +  1 * T1ileSize, position.y + -1 * T1ileSize, position.z,		1.0f, 0.0f,		0.0f,
		};
		uint32_t verticesCount = 6 * 4;

		// Local indices, pushData offsets them by the vertices already in the batch
		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
		uint32_t IndicesCount = 6;

		pushData(vertices, verticesCount, indices, IndicesCount, texture);
	}

	void Renderer::SubmitData(float* vertices, uint32_t vertexSize, uint32_t* indices, uint32_t indicesCount, std::shared_ptr<Texture> texture)
	{
		pushData(vertices, vertexSize, indices, indicesCount, texture);
	}

	void Renderer::EndRender()
	{
		EndBatch();
	}

	void Renderer::pushData(float* vertices, uint32_t vertexSize, uint32_t* indices, uint32_t indicesCount, std::shared_ptr<Texture> texture)
	{
		if (vertexSize % RenderData::FloatsPerVertex != 0)
		{
			LOG_ERROR("Renderer: vertex data size {} is not a multiple of {} floats", vertexSize, RenderData::FloatsPerVertex);
			return;
		}
		if (vertexSize > RenderData::MaxVerticesCount || indicesCount > RenderData::MaxIndicesCount)
		{
			LOG_ERROR("Renderer: submitted mesh is larger than the batch ({} floats, {} indices)", vertexSize, indicesCount);
			return;
		}

		// Flush before resolving the texture slot, so the slot stays valid for this data
		if (rData.VerticesCount + vertexSize > RenderData::MaxVerticesCount ||
			rData.IndicesCount + indicesCount > RenderData::MaxIndicesCount)
		{
			EndBatch();
			BeginBatch();
		}

		float textureIndex = SubmitTextureForBatch(texture);
		uint32_t baseVertex = rData.VerticesCount / RenderData::FloatsPerVertex;

		float* dst = rData.BatchedVertices + rData.VerticesCount;
		memcpy(dst, vertices, vertexSize * sizeof(float));
		for (uint32_t i = 0; i < vertexSize; i += RenderData::FloatsPerVertex)
			dst[i + RenderData::TexIndexOffset] = textureIndex;
		rData.VerticesCount += vertexSize;

		for (uint32_t i = 0; i < indicesCount; i++)
			rData.BatchedIndices[rData.IndicesCount + i] = indices[i] + baseVertex;
		rData.IndicesCount += indicesCount;
	}

	float Renderer::SubmitTextureForBatch(std::shared_ptr<Texture> texture)
	{
		if (!texture || texture == rData.WhiteTexture)
			return 0.0f;

		for (uint32_t i = 1; i < rData.TextureSlotIndex; i++)
		{
			if (rData.TextureSlots[i] &&
				rData.TextureSlots[i]->GetTexture() == texture->GetTexture())
				return (float)i;
		}

		if (rData.TextureSlotIndex >= rData.maxTexturesBoundAtTheTime)
		{
			EndBatch();
			BeginBatch();
		}

		float textureIndex = (float)rData.TextureSlotIndex;
		rData.TextureSlots[rData.TextureSlotIndex] = texture;
		rData.TextureSlotIndex++;

		return textureIndex;
	}

	void Renderer::BeginBatch()
	{
		rData.VerticesCount = 0;
		rData.IndicesCount = 0;

		rData.TextureSlotIndex = 1;
		rData.TextureSlots[0] = rData.WhiteTexture;
	}

	void Renderer::EndBatch()
	{
		if (rData.IndicesCount == 0)
			return;

		// Bind the VAO first so the element buffer upload hits our VAO's binding
		rData.VA->Bind();
		rData.VB->SetData(rData.BatchedVertices, rData.VerticesCount);
		rData.IB->SetData(rData.BatchedIndices, rData.IndicesCount);

		for (uint32_t i = 0; i < rData.TextureSlotIndex; i++) {

			rData.TextureSlots[i]->Bind(i);
		}
		rData.shader->Bind();
		DrawIndexed(rData.VA);
	}


	void Renderer::DrawIndexed(std::shared_ptr<VertexArray>& VA)
	{
		VA->Bind();
		glDrawElements(GL_TRIANGLES, VA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}
}
