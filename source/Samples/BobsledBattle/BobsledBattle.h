#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Gameplay/Game.h"
#include "Core/Types.h"

namespace Viper
{
	struct WindowContext;
	class ServiceLocator;
	namespace Gameplay
	{
		class Actor;
		class GameTime;
	}
}

namespace Bobsled
{
	class BobsledBattle : public Viper::Gameplay::Game
	{
	public:
		BobsledBattle(Viper::ServiceLocator& serviceLocator, std::uint32_t width, std::uint32_t height, const std::string& title);
		void Initialize() override;
		bool Update(const Viper::Gameplay::GameTime& gameTime) override;
		void Shutdown() override;

	private:
		void DefineCubeActors();
		void TestFrameGraph() const;

		Viper::ServiceLocator& serviceLocator;
		glm::vec3 audioPosition;
		std::vector<Viper::Gameplay::Actor*> actors;
	};
}
