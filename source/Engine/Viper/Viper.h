#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <sstream>
#include <string>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "gason.h"
#include "glm/glm.hpp"

#include "Asset/Asset.h"
#include "Asset/AssetManager.h"
#include "Asset/AssetRegistry.h"
#include "Asset/DataAsset.h"
#include "Asset/MaterialAsset.h"
#include "Asset/MeshAsset.h"
#include "Asset/ModelAsset.h"
#include "Asset/NonPbrMaterialAsset.h"
#include "Asset/PbrMaterialAsset.h"
#include "Asset/SceneAsset.h"
#include "Asset/TextureAsset.h"
#include "Audio/AudioManager.h"
#include "Containers/DataArray.h"
#include "Containers/Queue.h"
#include "Core/Checksum.h"
#include "Core/Core.h"
#include "Core/GameException.h"
#include "Core/MatrixHelper.h"
#include "Core/RTTI.h"
#include "Core/Service.h"
#include "Core/ServiceLocator.h"
#include "Core/Singleton.h"
#include "Core/StreamHelper.h"
#include "Core/StringID.h"
#include "Core/Types.h"
#include "Core/Utility.h"
#include "Core/VectorHelper.h"
#include "Gameplay/Actor.h"
#include "Gameplay/Game.h"
#include "Gameplay/GameClock.h"
#include "Gameplay/GameTime.h"
#include "Graphics/Blackboard.h"
#include "Graphics/FrameGraph.h"
#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"
#include "Graphics/RendererTypes.h"
#include "Graphics/RenderPass.h"
#include "Graphics/RenderPassBuilder.h"
#include "Graphics/RenderPrimitive.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Input/InputEnums.h"
#include "Input/InputManager.h"
#include "Logging/Logger.h"
#include "Memory/MemoryAllocator.h"
#include "Memory/MemoryManager.h"
#include "Memory/StackAllocator.h"
#include "Platform/DynamicLibrary.h"
#include "Platform/Platform.h"
#include "Window/WindowContext.h"
#include "Window/WindowManager.h"
