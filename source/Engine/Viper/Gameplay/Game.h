#pragma once

#include <string>
#include "Core/Service.h"
#include "Core/Types.h"

namespace Viper
{
	namespace Gameplay
	{
		class GameTime;

		class Game : public Service
		{
		public:
			Game(std::uint32_t width, std::uint32_t height, const std::string& title);
			virtual void Initialize() = 0;
			virtual bool Update(const GameTime& gameTime) = 0;
			virtual void Shutdown() = 0;

			std::uint32_t Width() const;
			std::uint32_t Height() const;
			const std::string& Title() const;

			static const ServiceType Type;
		protected:
			std::uint32_t width;
			std::uint32_t height;
			std::string title;
		};
	}
}
