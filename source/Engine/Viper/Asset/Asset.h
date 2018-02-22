#pragma once

#include <functional>
#include <iostream>
#include "Asset/AssetType.h"
#include "Core/StringID.h"
#include "Core/StreamHelper.h"

namespace Viper
{
	namespace Assets
	{
		class AssetManager;
		class Asset;

		typedef std::function<Asset*(const StringID&)> AssetConstructor;

		class Asset
		{
		public:
			Asset(const StringID& assetFullName, AssetType type = AssetType::Invalid);
			virtual ~Asset() = default;

			const StringID& AssetFullName() const;
			AssetType Type() const;

			void Load();
			void Save();

			StringID assetFullName;
			AssetType type;
		protected:
			virtual void LoadFrom(InputStreamHelper& inputHelper) = 0;
			virtual void SaveTo(OutputStreamHelper& outputHelper) const = 0;

			static void RegisterAssetType(AssetType type, const AssetConstructor& constructor);
		};

#define ASSET_DECLARATION(AssetClass)					\
		private:										\
		static const bool IsAssetTypeRegistered;		\
		static bool RegisterAssetType();				\
		public:											\
		AssetClass(const StringID& name);

#define ASSET_DEFINITION(AssetClass, ParentClass, AssetTypeEnum)												\
		const bool AssetClass::IsAssetTypeRegistered = RegisterAssetType();										\
		bool AssetClass::RegisterAssetType()																	\
		{																										\
			Asset::RegisterAssetType(AssetTypeEnum, [](const StringID& name) -> Asset* {				\
				return new AssetClass(name);															\
			});																									\
			return true;																						\
		}																										\
		AssetClass::AssetClass(const StringID& name) : ParentClass(name, AssetTypeEnum) {}
	}
}
