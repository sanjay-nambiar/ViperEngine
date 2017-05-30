#include "Pch.h"
#include <chrono>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Core/Platform.h"
#include "Core/ServiceLocator.h"
#include "Core/ModuleLoader.h"
#include "Gameplay/Actor.h"
#include "Graphics/Mesh.h"
#include "Memory/MemoryManager.h"
#include "Service/AudioManager.h"
#include "Service/RendererSystem.h"


#ifndef UNICODE
#define UNICODE
#endif


#define WIDTH		1920
#define HEIGHT		1080
#define TITLE		"Bobsled Battle"
#define CONFIG_FILE "Config.ini"

using namespace Viper;


void Initialize()
{
	MemoryManager* allocator = new MemoryManager();
	assert(allocator != nullptr);
	Core::ModuleLoader::CreateInstance(*allocator);
	Core::ModuleLoader::GetInstance().LoadModules(CONFIG_FILE);
}

void ShutDown()
{
	MemoryAllocator& allocator = ServiceLocator::GetInstance().GetMemoryAllocator();
	Core::ModuleLoader::GetInstance().UnloadModules();
	Core::ModuleLoader::Destroy();
	delete &allocator;
}

void DefineCubeActors(std::vector<Gameplay::Actor*>& actors)
{
	TextureLoader& loader = ServiceLocator::GetInstance().GetTextureLoader();
	Graphics::Texture textureObj1 = loader.LoadTexture("Content/Textures/wall.jpg");
	Graphics::Texture textureObj2 = loader.LoadTexture("Content/Textures/decal.jpg");

	RendererSystem& rendererSystem = ServiceLocator::GetInstance().GetRendererSystem();

	// vertices and indices to vertices for the cube
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f
	};
	std::uint32_t elements[36];
	for (std::uint32_t index = 0; index < 36; ++index)
	{
		elements[index] = index;
	}

	// load mesh
	Graphics::Mesh mesh(36, vertices, 12, elements);
	rendererSystem.LoadMesh(mesh);

	// Create cube actors
	glm::vec3 cubePositions[10] = {
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(3.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(3.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(2.3f, -2.0f, -2.5f),
		glm::vec3(2.5f,  2.0f, -2.5f),
		glm::vec3(3.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	for (unsigned int i = 0; i < 10; i++)
	{
		Gameplay::Actor& actor = *(new Gameplay::Actor(&mesh, {&textureObj1, &textureObj2}));
		actor.Model() = glm::translate(actor.Model(), cubePositions[i]);
		float angle = 20.0f * i;
		actor.Model() = glm::rotate(actor.Model(), glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		actor.View() = glm::translate(actor.View(), glm::vec3(-1.0f, 0.0f, -3.0f));
		actor.Projection() = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 100.0f);
		rendererSystem.AddActorToScene(actor);
		actors.push_back(&actor);
	}
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Initialize();

	WindowManager& windowManager = ServiceLocator::GetInstance().GetWindowManager();
	windowManager.Initialize();
	const WindowContext& windowContext = windowManager.CreateGameWindow(WIDTH, HEIGHT, TITLE);

	InputManager& inputManager = ServiceLocator::GetInstance().GetInputManager();
	inputManager.Initialize(windowContext);

	RendererSystem& rendererSystem = ServiceLocator::GetInstance().GetRendererSystem();
	rendererSystem.Initialize();
	rendererSystem.SetViewport(windowContext);

	Graphics::Shader vertexShader = rendererSystem.LoadShaderFile("Content/Shaders/default.vert", Graphics::ShaderType::VERTEX_SHADER);
	Graphics::Shader fragmentShader = rendererSystem.LoadShaderFile("Content/Shaders/default.frag", Graphics::ShaderType::FRAGMENT_SHADER);
	rendererSystem.UseShaders({vertexShader, fragmentShader});

	std::vector<Gameplay::Actor*> actors;
	DefineCubeActors(actors);

	// Test Audio
	AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
	audioManager.SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.bank");
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.strings.bank");
	audioManager.LoadSoundBankEvents("Content/Sounds/ZombieWars.bank");

	Viper::Vector3 position(0, 0, 0);
	audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
	audioManager.PlayEvent("event:/GattlingGun-Fire");

	glm::mat4 model;
	while (windowManager.BeginUpdate(windowContext))
	{
		inputManager.Update();
		if (inputManager.GetButtonState(Button::Key_Escape) == ButtonState::Pressed)
		{
			break;
		}
		if (inputManager.GetButtonState(Button::Key_Up) == ButtonState::Pressed)
		{
			position.z += 0.01f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		}
		if (inputManager.GetButtonState(Button::Key_Down) == ButtonState::Pressed)
		{
			position.z -= 0.01f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		}
		if (inputManager.GetButtonState(Button::Key_Space) == ButtonState::Pressed && inputManager.IsModifierActive(ModifierKey::Control))
		{
			position.z = 0.0f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		}

		for (auto actor : actors)
		{
			actor->Model() = glm::rotate(actor->Model(), glm::radians(0.05f), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		rendererSystem.Update();
		audioManager.Update();

		windowManager.EndUpdate(windowContext);
	}

	rendererSystem.Shutdown();
	windowManager.Shutdown();
	ShutDown();
	return 0;
}
