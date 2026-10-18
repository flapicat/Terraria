#pragma once
#include <Engine.h>
#include <unordered_map>

namespace Terraria
{
	enum class TileType {
		Air,
		Dirt1,
		Dirt2,
		Dirt3,
		Dirt4, 
		Grass1,
		Grass2,
		Grass3,
		Grass4,
		Stone1,
		Stone2,
		Stone3,
		Stone4
	};

	struct TileTexManager
	{
	public:
		void InitTiles() {
			std::shared_ptr<Engine::Texture> atlas = std::make_shared<Engine::Texture>("Assets/Textures/textures.png");
			m_TexturesAtlas = atlas;
			m_Tiles[TileType::Air] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 0, 4);
			m_Tiles[TileType::Dirt1] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 1, 0);
			m_Tiles[TileType::Dirt2] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 1, 1);
			m_Tiles[TileType::Dirt3] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 1, 2);
			m_Tiles[TileType::Dirt4] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 1, 3);
			m_Tiles[TileType::Grass1] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 2, 0);
			m_Tiles[TileType::Grass2] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 2, 1);
			m_Tiles[TileType::Grass3] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 2, 2);
			m_Tiles[TileType::Grass4] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 2, 3);
			m_Tiles[TileType::Stone1] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 3, 0);
			m_Tiles[TileType::Stone2] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 3, 1);
			m_Tiles[TileType::Stone3] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 3, 2);
			m_Tiles[TileType::Stone4] = std::make_shared<Engine::SubTexture>(m_TexturesAtlas, 3, 3);
		}

		std::shared_ptr<Engine::SubTexture> GetTexture(TileType type) { return m_Tiles[type]; }
	private:
		std::shared_ptr<Engine::Texture> m_TexturesAtlas;
		std::unordered_map<TileType, std::shared_ptr<Engine::SubTexture>> m_Tiles;
	};
}
