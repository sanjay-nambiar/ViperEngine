#include "Pch.h"
#include "RendererSystem.h"
#include <iterator>

namespace Viper
{
	TextureDescription::TextureDescription()
		: data(nullptr)
	{
	}

	RendererSystem::RendererSystem()
	{
		points = {
			{Primitive::Point, {}},
			{Primitive::Line, {}},
			{Primitive::Triangle, {}},
		};

		points[Primitive::Point].reserve(100);
		points[Primitive::Line].reserve(200);
		points[Primitive::Triangle].reserve(1000);
	}

	void RendererSystem::Update()
	{
		points[Primitive::Point].clear();
		points[Primitive::Line].clear();
		points[Primitive::Triangle].clear();
	}

	void RendererSystem::AddPoint(const Vector3& pointToAdd, Primitive primitive)
	{
		points[primitive].push_back(pointToAdd);
	}

	void RendererSystem::AddPoints(const std::vector<Vector3> pointsToAdd, Primitive primitive)
	{
		points[primitive].insert(points[primitive].end(), pointsToAdd.begin(), pointsToAdd.end());
	}
}
