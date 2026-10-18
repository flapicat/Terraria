#pragma once
#include "Texture.h"	

#include <glm/glm.hpp>

namespace Engine
{
	class SubTexture
	{
	public:
		SubTexture(std::shared_ptr<Texture> texAtlas, uint32_t col, uint32_t row);
		~SubTexture();

		glm::vec2 GetUVMin() { return m_uvMin; }
		glm::vec2 GetUVMax() { return m_uvMax; }

		std::shared_ptr<Texture> GetTexAtlas() { return m_TexAtlas; }
	private:
		std::shared_ptr<Texture> m_TexAtlas;
		uint32_t m_Col = 0;
		uint32_t m_Row = 5;
		glm::vec2 m_uvMin = glm::vec2(0.0f);
		glm::vec2 m_uvMax = glm::vec2(0.0f);
	};

}