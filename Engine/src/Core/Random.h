#pragma once
#include <random>

namespace Engine {
	inline std::mt19937 rng{ std::random_device{}() };

	inline int GetRandomInt(int min, int max) {
		return std::uniform_int_distribution<int>{min, max}(rng);
	}
	inline double GetRandomDouble(double min, double max) {
		return std::uniform_real_distribution<double>{min, max}(rng);
	}
}