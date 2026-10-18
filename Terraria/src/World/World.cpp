#include "World.h"

#include <FastNoiseLite/FastNoiseLite.h>

namespace Terraria
{
	World::World() {

	}
	World::~World(){
		if (m_Map)
			delete[] m_Map;
	}

	void World::Init(){
		m_TileTexManager.InitTiles();

		m_MapSize = WorldSize::Small;
		switch (m_MapSize)
		{
		case World::WorldSize::None:
			m_MapWidth = 0;
			m_MapHeight = 0;
			break;
		case World::WorldSize::Test:
			m_MapWidth = 20;
			m_MapHeight = 40;
			break;
		case World::WorldSize::Small:
			m_MapWidth = 4200;
			m_MapHeight = 1200;
			break;
		case World::WorldSize::Medium:
			m_MapWidth = 6400;
			m_MapHeight = 1800;
			break;
		case World::WorldSize::Big:
			m_MapWidth = 8400;
			m_MapHeight = 2400;
			break;
		}

		if (m_MapWidth == 0 || m_MapHeight == 0)
			LOG_WARN("World size is {}x{}, the world will be empty", m_MapWidth, m_MapHeight);

		m_Map = new TileType[m_MapWidth * m_MapHeight];

		Engine::Timer timer;
		GenerateWorld();
		LOG_INFO("World generated: {}x{} tiles, seed {}, spawn at ({:.2f}, {:.2f}), took {:.0f} ms",
			m_MapWidth, m_MapHeight, m_WorldSeed, m_SpawnPoint.x, m_SpawnPoint.y, timer.GetTimePassedMS());
	}


	void World::GenerateWorld()
	{
		m_WorldSeed = Engine::GetRandomInt(1, 99999);
		// Wide rolling hills
		FastNoiseLite hills(m_WorldSeed);
		hills.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		hills.SetFrequency(0.0025f);
		hills.SetFractalType(FastNoiseLite::FractalType_FBm);
		hills.SetFractalOctaves(4);

		// Rare tall mountains: ridged noise, only the highest ridges are used
		FastNoiseLite mountains(m_WorldSeed);
		mountains.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		mountains.SetFrequency(0.0012f);
		mountains.SetFractalType(FastNoiseLite::FractalType_Ridged);
		mountains.SetFractalOctaves(3);

		// Small bumps along the surface
		FastNoiseLite detail(m_WorldSeed);
		detail.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		detail.SetFrequency(0.06f);

		// Varies how deep the dirt layer goes before turning to stone
		FastNoiseLite dirtDepth(m_WorldSeed);
		dirtDepth.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		dirtDepth.SetFrequency(0.01f);

		// Stone patches inside dirt and dirt patches inside stone
		FastNoiseLite blobs(m_WorldSeed);
		blobs.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		blobs.SetFrequency(0.05f);
		blobs.SetFractalType(FastNoiseLite::FractalType_FBm);
		blobs.SetFractalOctaves(2);

		// Big open caverns deep underground
		FastNoiseLite caverns(m_WorldSeed);
		caverns.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		caverns.SetFrequency(0.012f);
		caverns.SetFractalType(FastNoiseLite::FractalType_FBm);
		caverns.SetFractalOctaves(4);

		// Small round air pockets
		FastNoiseLite pockets(m_WorldSeed);
		pockets.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		pockets.SetFrequency(0.07f);

		const float surfaceBase = m_MapHeight * 0.25f;

		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			float mountainValue = std::max(0.0f, mountains.GetNoise((float)x, 0.0f) - 0.4f) / 0.6f;

			float height = surfaceBase
				- hills.GetNoise((float)x, 0.0f) * 25.0f
				- mountainValue * mountainValue * 90.0f
				+ detail.GetNoise((float)x, 0.0f) * 2.0f;

			int surfaceLevelY = (int)height;
			int stoneLevelY = surfaceLevelY + 40 + (int)(dirtDepth.GetNoise((float)x, 0.0f) * 15.0f);

			if (x == m_MapWidth / 2) {
				m_SpawnPoint = { x * TileSpacing, -(surfaceLevelY - 5) * TileSpacing };
			}

			for (uint32_t y = 0; y < m_MapHeight; y++)
			{
				TileType tileType = TileType::Air;
				int depth = (int)y - surfaceLevelY;

				if (depth == 0)
					tileType = TileType::Grass1;
				else if (depth > 0 && (int)y < stoneLevelY)
					tileType = TileType::Dirt1;
				else if (depth > 0)
					tileType = blobs.GetNoise((float)x, (float)y) > 0.6f ? TileType::Dirt1 : TileType::Stone1;

				if (depth > 6)
				{
					// Caves get bigger the deeper you go
					float depthFactor = std::min(1.0f, depth / 400.0f);
					bool cavern = depth > 120 && caverns.GetNoise((float)x, (float)y * 1.3f) > 0.45f - depthFactor * 0.2f;
					bool pocket = depth > 20 && pockets.GetNoise((float)x, (float)y) > 0.75f;

					if (cavern || pocket)
						tileType = TileType::Air;
				}

				// Pick a random texture variant (Block1-Block4).
				if (tileType != TileType::Air)
				{
					uint32_t hash = x * 374761393u + y * 668265263u;
					hash = (hash ^ (hash >> 13)) * 1274126177u;
					hash ^= hash >> 16;
					tileType = (TileType)((int)tileType + (int)(hash & 3u));
				}

				Set(x, y, tileType);
			}
		}
	}


	void World::Set(uint32_t x, uint32_t y, TileType type) { 
		if (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight) {
			m_Map[y * m_MapWidth + x] = type;
		}
		else {
			LOG_ERROR("Trying to set tile ({}, {}) outside the {}x{} world", x, y, m_MapWidth, m_MapHeight);
		}
	}

	TileType World::Get(uint32_t x, uint32_t y) { 
		if (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight) {
			return m_Map[y * m_MapWidth + x];
		}
		else {
			LOG_ERROR("Trying to get tile ({}, {}) outside the {}x{} world, Air returned instead", x, y, m_MapWidth, m_MapHeight);
		}
		return TileType::Air;
	}

	void World::Update(Engine::TimeStep ts){

	}

	void World::Render(const Engine::Camera& camera){
		const float tileSpacing = TileSpacing;

		auto camBounds = camera.GetViewBounds();
		int32_t firstX = static_cast<int32_t>(std::floor(camBounds.Left / tileSpacing));
		int32_t lastX = static_cast<int32_t>(std::ceil(camBounds.Right / tileSpacing));

		int32_t firstY = static_cast<int32_t>(std::floor(-camBounds.Top / tileSpacing));
		int32_t lastY = static_cast<int32_t>(std::ceil(-camBounds.Bottom / tileSpacing));

		firstX = std::max(firstX, 0);
		lastX = std::max(lastX, 0);

		firstY = std::max(firstY, 0);
		lastY = std::max(lastY, 0);

		for (uint32_t y = firstY; y < lastY+1; y++){
			for (uint32_t x = firstX; x < lastX+1; x++){
				if (x < 0 || y < 0 || x >= m_MapWidth || y >= m_MapHeight) continue;
				auto tile = this->Get(x, y);
				if (tile == TileType::Air) continue;
				Engine::Renderer::SubmitTile({ x * tileSpacing, -(int32_t)y * tileSpacing, 0.0f }, m_TileTexManager.GetTexture(tile), TileSize);
			}
		}
	}
}
