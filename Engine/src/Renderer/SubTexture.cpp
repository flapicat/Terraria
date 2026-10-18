#include "pch.h"
#include "SubTexture.h"

namespace Engine {
	SubTexture::SubTexture(std::shared_ptr<Texture> texAtlas, uint32_t col, uint32_t row)
		:m_TexAtlas(texAtlas), m_Col(col), m_Row(row)
	{
		constexpr uint32_t TexWidth = 32;
		constexpr uint32_t TexHeight = 32;

		if (!texAtlas || texAtlas->GetTexture() == 0)
		{
			LOG_ERROR("SubTexture ({}, {}) has no atlas to cut from, the atlas texture is null or failed to load", col, row);
			return;
		}

		const uint32_t numOfColumns = texAtlas->GetWidth() / TexWidth;
		const uint32_t numOfRows = texAtlas->GetHeight() / TexHeight;

		if (texAtlas->GetWidth() % TexWidth != 0 || texAtlas->GetHeight() % TexHeight != 0)
			LOG_WARN("Atlas size {}x{} is not a multiple of the {}x{} cell size, tiles will be cut at the wrong place",
				texAtlas->GetWidth(), texAtlas->GetHeight(), TexWidth, TexHeight);

		if (col >= numOfColumns || row >= numOfRows)
		{
			LOG_ERROR("SubTexture ({}, {}) is outside the atlas grid of {}x{} cells", col, row, numOfColumns, numOfRows);
			return;
		}

		const float cellW = 1.0f / numOfColumns;
		const float cellH = 1.0f / numOfRows;

		// Texture is flipped on load (UV y=0 is the bottom), so count rows from the top
		const uint32_t flippedRow = numOfRows - 1 - row;

		// Inset by half a texel so neighbouring tiles don't bleed in at the edges
		const float halfTexelX = 0.5f / texAtlas->GetWidth();
		const float halfTexelY = 0.5f / texAtlas->GetHeight();

		m_uvMin.x = col * cellW + halfTexelX;
		m_uvMin.y = flippedRow * cellH + halfTexelY;
		m_uvMax.x = (col + 1) * cellW - halfTexelX;
		m_uvMax.y = (flippedRow + 1) * cellH - halfTexelY;
	}

	SubTexture::~SubTexture()
	{
	}
}