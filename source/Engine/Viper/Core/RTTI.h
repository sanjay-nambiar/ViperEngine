#pragma once

#include <string>
#include <cstdint>

namespace Viper
{
	class RTTI
	{
	public:
		virtual ~RTTI() = default;

		virtual std::uintmax_t TypeIdInstance() const = 0;

		virtual bool Is(std::uintmax_t) const
		{
			return false;
		}

		template <typename T>
		T* As() const
		{
			if (Is(T::TypeIdClass()))
			{
				return reinterpret_cast<T*>(const_cast<RTTI*>(this));
			}
			return nullptr;
		}
	};


#define RTTI_DECLARATIONS(Type, ParentType)																	 \
		public:                                                                                              \
			typedef ParentType Parent;                                                                       \
			static std::string TypeName() { return std::string(#Type); }                                     \
			static std::uintmax_t TypeIdClass() { return RunTimeTypeId; }                                    \
			virtual std::uintmax_t TypeIdInstance() const override { return Type::TypeIdClass(); }           \
			virtual bool Is(std::uintmax_t id) const override                                                \
			{                                                                                                \
				if (id == RunTimeTypeId)                                                                     \
					{ return true; }                                                                         \
				else                                                                                         \
					{ return Parent::Is(id); }                                                               \
			}                                                                                                \
			private:                                                                                         \
				static std::uintmax_t RunTimeTypeId;

#define RTTI_DEFINITIONS(Type) std::uintmax_t Type::RunTimeTypeId = reinterpret_cast<std::uintmax_t>(&Type::RunTimeTypeId);
}
