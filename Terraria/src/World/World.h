#pragma once
#include <Engine.h>

#include "TileData.h"

namespace Terraria
{
	class World
	{
	public:
		World();
		~World();

		void Init();

		void Update(Engine::TimeStep ts);
		void Render(const Engine::Camera& camera);
	
		void Set(uint32_t x, uint32_t y, TileType type);
		TileType Get(uint32_t x, uint32_t y);

		uint32_t GetWidth() const { return m_MapWidth; }
		uint32_t GetHeight() const { return m_MapHeight; }

		glm::vec2 GetSpawnPoint() { return m_SpawnPoint; }
	public:
		static constexpr float TileSize = 0.05f;
		static constexpr float TileSpacing = TileSize * 2.0f;
	private:
		void GenerateWorld();
	private:
		enum class WorldSize {
			None,
			Test,
			Small,
			Medium,
			Big
		};

		WorldSize m_MapSize = WorldSize::None;
		TileType* m_Map = nullptr;
		uint32_t m_MapWidth = 0;
		uint32_t m_MapHeight = 0;
		TileTexManager m_TileTexManager;

		uint32_t m_WorldSeed = 0;
		glm::vec2 m_SpawnPoint = glm::vec2(0.0f);
	};
}
