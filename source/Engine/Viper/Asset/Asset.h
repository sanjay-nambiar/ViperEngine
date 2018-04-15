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

			const StringID& AssetId() const;
			AssetType Type() const;

			virtual bool operator==(const Asset& rhs) const = 0;
			virtual bool operator!=(const Asset& rhs) const = 0;

			void Load();
			void Save();
		protected:
			virtual void LoadFrom(InputStreamHelper& inputHelper) = 0;
			virtual void SaveTo(OutputStreamHelper& outputHelper) const = 0;

			StringID assetId;
			AssetType type;

			AssetManager& assetManager;
			static void RegisterAssetType(AssetType type, const AssetConstructor& constructor);

			friend class AssetManager;
		};

#define ASSET_DECLARATION(AssetClass)					\
		private:										\
		static const bool IsAssetTypeRegistered;		\
		static bool RegisterAssetType();				\
		public:											\
		AssetClass(const StringID& name);

#define ASSET_DEFINITION(AssetClass, ParentClass, AssetTypeEnum)								\
		const bool AssetClass::IsAssetTypeRegistered = RegisterAssetType();						\
		bool AssetClass::RegisterAssetType()													\
		{																						\
			Asset::RegisterAssetType(AssetTypeEnum, [](const StringID& name) -> Asset* {		\
				return new AssetClass(name);													\
			});																					\
			return true;																		\
		}																						\
		AssetClass::AssetClass(const StringID& name) : ParentClass(name, AssetTypeEnum) {}
	}

	OutputStreamHelper& operator<<(OutputStreamHelper& helper, const Assets::AssetType& value);
	InputStreamHelper& operator>>(InputStreamHelper& helper, Assets::AssetType& value);
}
