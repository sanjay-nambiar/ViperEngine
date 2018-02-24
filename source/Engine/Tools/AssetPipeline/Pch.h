#pragma once

// Standard
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

#include "stb_image.h"	// stbi

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
#include "ModelProcessor.h"
#include "MeshProcessor.h"
#include "MaterialProcessor.h"
#include "TextureProcessor.h"
