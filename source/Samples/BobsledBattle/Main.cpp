#include "Pch.h"
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/StringID.h"
#include "Core/ModuleLoader.h"
#include "Core/Platform.h"
#include "Core/ServiceLocator.h"
#include "Gameplay/Actor.h"
#include "Graphics/Mesh.h"

#ifndef UNICODE
#define UNICODE
#endif

#define WIDTH		800
#define HEIGHT		600
#define TITLE		"Bobsled Battle"
#define CONFIG_FILE "Config.json"

using namespace std;
using namespace glm;
using namespace Viper;
using namespace Viper::Core;
using namespace Viper::Graphics;
using namespace Viper::Gameplay;
using namespace Viper::Memory;
using namespace Viper::Audio;
using namespace Viper::Input;
using namespace Viper::Window;


void Initialize()
{
	MemoryManager* allocator = new MemoryManager();
	assert(allocator != nullptr);
	ModuleLoader::CreateInstance(*allocator);
	ModuleLoader::GetInstance().LoadModules(CONFIG_FILE);
}

void ShutDown()
{
	MemoryAllocator& allocator = ServiceLocator::GetInstance().GetMemoryAllocator();
	ModuleLoader::GetInstance().UnloadModules();
	ModuleLoader::Destroy();
	delete &allocator;
}

void DefineCubeActors(vector<Gameplay::Actor*>& actors)
{
	TextureLoader& loader = ServiceLocator::GetInstance().GetTextureLoader();
	Texture textureObj1 = loader.LoadTexture("Content/Textures/wall.jpg");
	Texture textureObj2 = loader.LoadTexture("Content/Textures/decal.jpg");

	Renderer& renderer = ServiceLocator::GetInstance().GetRendererSystem();

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
	uint32_t elements[36];
	for (uint32_t index = 0; index < 36; ++index)
	{
		elements[index] = index;
	}

	// load mesh
	Mesh mesh(36, vertices, 12, elements);
	renderer.LoadMesh(mesh);

	// Create cube actors
	vec3 cubePositions[10] = {
		vec3(1.0f,  0.0f,  0.0f),
		vec3(3.0f,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(3.4f, -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(2.3f, -2.0f, -2.5f),
		vec3(2.5f,  2.0f, -2.5f),
		vec3(3.5f,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};
	for (unsigned int i = 0; i < 10; i++)
	{
		Actor& actor = *(new Actor(&mesh, {&textureObj1, &textureObj2}));
		actor.Model() = translate(actor.Model(), cubePositions[i]);
		float angle = 20.0f * i;
		actor.Model() = rotate(actor.Model(), radians(angle), vec3(1.0f, 0.3f, 0.5f));
		actor.View() = translate(actor.View(), vec3(-1.0f, 0.0f, -3.0f));
		actor.Projection() = perspective(radians(45.0f), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 100.0f);
		renderer.AddActorToScene(actor);
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

	Renderer& renderer = ServiceLocator::GetInstance().GetRendererSystem();
	renderer.Initialize();
	renderer.SetViewport(windowContext);

	Shader vertexShader = renderer.LoadShaderFile("Content/Shaders/default.vert", ShaderType::VERTEX_SHADER);
	Shader fragmentShader = renderer.LoadShaderFile("Content/Shaders/default.frag", ShaderType::FRAGMENT_SHADER);
	renderer.UseShaders({vertexShader, fragmentShader});

	vector<Gameplay::Actor*> actors;
	DefineCubeActors(actors);

	// Test Audio
	AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
	audioManager.SetListener3dAttributes(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 1, 0));
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.bank");
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.strings.bank");
	audioManager.LoadSoundBankEvents("Content/Sounds/ZombieWars.bank");

	vec3 position(0, 0, 0);
	audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, vec3(0, 0, 0));
	audioManager.PlayEvent("event:/GattlingGun-Fire");

	mat4 model;
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
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, vec3(0, 0, 0));
		}
		if (inputManager.GetButtonState(Button::Key_Down) == ButtonState::Pressed)
		{
			position.z -= 0.01f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, vec3(0, 0, 0));
		}
		if (inputManager.GetButtonState(Button::Key_Space) == ButtonState::Pressed && inputManager.IsModifierActive(ModifierKey::Control))
		{
			position.z = 0.0f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, vec3(0, 0, 0));
		}

		for (auto actor : actors)
		{
			actor->Model() = rotate(actor->Model(), radians(0.05f), vec3(0.5f, 1.0f, 0.0f));
		}
		renderer.Update();
		audioManager.Update();

		windowManager.EndUpdate(windowContext);
	}

	TestFrameGraph();
	renderer.Shutdown();
	windowManager.Shutdown();
	ShutDown();



	return 0;
}
