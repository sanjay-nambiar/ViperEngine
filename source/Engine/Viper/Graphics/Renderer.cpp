#include "Viper.h"
#include "Renderer.h"
#include <iterator>

namespace Viper
{
	using namespace Graphics;

	Renderer::Renderer()
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

	void Renderer::Update()
	{
		points[Primitive::Point].clear();
		points[Primitive::Line].clear();
		points[Primitive::Triangle].clear();
	}

	void Renderer::AddPoint(const glm::vec3& pointToAdd, Primitive primitive)
	{
		points[primitive].push_back(pointToAdd);
	}

	void Renderer::AddPoints(const std::vector<glm::vec3> pointsToAdd, Primitive primitive)
	{
		points[primitive].insert(points[primitive].end(), pointsToAdd.begin(), pointsToAdd.end());
	}
}
