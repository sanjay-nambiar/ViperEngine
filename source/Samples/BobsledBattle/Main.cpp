#include "Pch.h"
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Platform.h"
#include "Core/ServiceLocator.h"
#include "Core/ModuleLoader.h"
#include "Gameplay/Actor.h"
#include "Graphics/Mesh.h"
#include "Memory/MemoryManager.h"
#include "Service/AudioManager.h"
#include "Service/RendererSystem.h"

#include "Graphics/FrameGraph.h"

#ifndef UNICODE
#define UNICODE
#endif


#define WIDTH		800
#define HEIGHT		600
#define TITLE		"Bobsled Battle"
#define CONFIG_FILE "Config.json"

using namespace std;
using namespace Viper;
using namespace Viper::Graphics;

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

void TestFrameGraph()
{
	FrameGraph graph(ServiceLocator::GetInstance());

	// Z Pass
	struct ZPassData
	{
		FrameGraphResourceNode* Depth;
	};

	auto& zPrePass = graph.AddRenderPass<ZPassData>(string("ZPass"),
		[&](RenderPassBuilder& builder, ZPassData& data)
		{
			TextureDescription desc;
			desc.type = TextureType::TEXTURE_2D;
			desc.format = TextureFormat::D32F;
			desc.width = 1920;
			desc.height = 1080;
			desc.data = nullptr;
			desc.wrapS = TextureWrapping::CLAMP_TO_EDGE;
			desc.wrapT = TextureWrapping::CLAMP_TO_EDGE;
			desc.minFilter = TextureFiltering::NEAREST;
			desc.magFilter = TextureFiltering::NEAREST;
			data.Depth = &builder.CreateWrite("Depth", desc);
		},
		[=](const ZPassData&) {}
	);

	// GBuffer Pass
	struct GBufferPassData
	{
		// Input
		FrameGraphResourceNode* Depth;
		// Output
		FrameGraphResourceNode* Position;
		FrameGraphResourceNode* Normal;
		FrameGraphResourceNode* AlbedoSpec;
	};

	TextureDescription commonTextureDesc;
	commonTextureDesc.type = TextureType::TEXTURE_2D;
	commonTextureDesc.format = TextureFormat::RGB32F;
	commonTextureDesc.width = 1920;
	commonTextureDesc.height = 1080;
	commonTextureDesc.data = nullptr;
	commonTextureDesc.wrapS = TextureWrapping::CLAMP_TO_EDGE;
	commonTextureDesc.wrapT = TextureWrapping::CLAMP_TO_EDGE;
	commonTextureDesc.minFilter = TextureFiltering::NEAREST;
	commonTextureDesc.magFilter = TextureFiltering::NEAREST;

	auto& gBufferPass = graph.AddRenderPass<GBufferPassData>(string("GBufferPass"),
		[&](RenderPassBuilder& builder, GBufferPassData& data)
		{
			data.Depth = &builder.Read(*reinterpret_cast<FrameGraphResourceNode*>(zPrePass.next.front()));
			data.Position = &builder.CreateWrite("Position", commonTextureDesc);
			data.Normal = &builder.CreateWrite("Normal", commonTextureDesc);
			TextureDescription gbufferColor = commonTextureDesc;
			gbufferColor.format = TextureFormat::RGBA32F;
			data.AlbedoSpec = &builder.CreateWrite("AlbedoSpec", gbufferColor);
		},
		[=](const GBufferPassData&) {}
	);

	// Lighting Pass
	struct LightingPassData
	{
		// Input
		FrameGraphResourceNode* Position;
		FrameGraphResourceNode* Normal;
		FrameGraphResourceNode* AlbedoSpec;
		// Output
		FrameGraphResourceNode* LitScene;
	};

	auto& lightingPass = graph.AddRenderPass<LightingPassData>(string("LightingPass"),
		[&](RenderPassBuilder& builder, LightingPassData& data)
		{
			data.Position = &builder.Read(*reinterpret_cast<FrameGraphResourceNode*>(gBufferPass.next[0]));
			data.Normal = &builder.Read(*reinterpret_cast<FrameGraphResourceNode*>(gBufferPass.next[1]));
			data.AlbedoSpec = &builder.Read(*reinterpret_cast<FrameGraphResourceNode*>(gBufferPass.next[2]));
			data.LitScene = &builder.CreateWrite("LitScene", commonTextureDesc);
		},
		[=](const LightingPassData&) {}
	);

	// PostProcess Pass
	struct PostProcessPassData
	{
		// Input
		FrameGraphResourceNode* LitScene;
		// Output
		FrameGraphResourceNode* PostProcessScene;
	};

	auto& postProcessPass = graph.AddRenderPass<PostProcessPassData>(string("PostProcessPass"),
		[&](RenderPassBuilder& builder, PostProcessPassData& data)
		{
			data.LitScene = &builder.Read(*reinterpret_cast<FrameGraphResourceNode*>(lightingPass.next.front()));
			data.PostProcessScene = &builder.CreateWrite("PostProcessScene", commonTextureDesc);
		},
		[=](const PostProcessPassData&) {}
	);

	// Debug Pass
	struct DebugPassData
	{
		// Input
		FrameGraphResourceNode* Position;
		// Output
		FrameGraphResourceNode* DebugView;
	};

	auto& debugPass = graph.AddRenderPass<DebugPassData>(string("DebugPass"),
		[&](RenderPassBuilder& builder, DebugPassData& data)
		{
			data.Position = &builder.Read(*reinterpret_cast<FrameGraphResourceNode*>(gBufferPass.next[0]));
			data.DebugView = &builder.CreateWrite("DebugView", commonTextureDesc);
		},
		[=](const DebugPassData&) {}
	);

#ifdef _DEBUG
	postProcessPass;
	graph.SetDisplayTarget(*reinterpret_cast<FrameGraphResourceNode*>(debugPass.next.front()));
#else
	debugPass;
	graph.SetDisplayTarget(*reinterpret_cast<FrameGraphResourceNode*>(postProcessPass.next.front()));
#endif

	graph.Compile();
	graph.AllocateGpuResources();
	graph.DebugOutput("Deferred Renderer", "deferred.dot");
	graph.FreeGpuResources();
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

	TestFrameGraph();
	rendererSystem.Shutdown();
	windowManager.Shutdown();
	ShutDown();

	return 0;
}
