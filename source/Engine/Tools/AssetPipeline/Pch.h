#pragma once

// Standard
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//stbi
#include "stb_image.h"

// gason
#include "gason.h"

// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

// Assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

// Engine
#include "Asset/AssetManager.h"
#include "Asset/AssetRegistry.h"
#include "Asset/ModelAsset.h"
#include "Asset/MeshAsset.h"
#include "Asset/MaterialAsset.h"
#include "Asset/NonPbrMaterialAsset.h"
#include "Asset/PbrMaterialAsset.h"
#include "Asset/TextureAsset.h"
#include "Core/GameException.h"
#include "Core/ServiceLocator.h"
#include "Core/StringID.h"
#include "Core/Utility.h"
#include "Memory/MemoryAllocator.h"

 // Local
#include "AssetProcessor.h"
#include "ModelProcessor.h"
#include "MeshProcessor.h"
#include "MaterialProcessor.h"
#include "TextureProcessor.h"
