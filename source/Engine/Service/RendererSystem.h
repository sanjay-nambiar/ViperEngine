#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "Core/Vector3.h"

namespace Viper
{
	class RendererSystem
	{
	public:
		enum class Primitive : std::uint32_t
		{
			Point,
			Line,
			Triangle
		};

		RendererSystem();
		virtual ~RendererSystem() = default;

		virtual void Initialize() = 0;
		virtual void Update();
		virtual void Shutdown() = 0;

		void AddPoint(const Vector3& point, Primitive primitive);
		void AddPoints(const std::vector<Vector3> points, Primitive primitive);

	private:
		std::unordered_map<Primitive, std::vector<Vector3>> points;
	};
}
