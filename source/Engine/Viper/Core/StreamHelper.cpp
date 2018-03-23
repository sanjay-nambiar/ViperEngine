#include "Viper.h"

using namespace std;
using namespace glm;

namespace Viper
{

#pragma region OutputStreamHelper

	OutputStreamHelper::OutputStreamHelper(ostream& stream) :
		mStream(stream)
	{}

	ostream& OutputStreamHelper::Stream()
	{
		return mStream;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(int8_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(int16_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(int32_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(int64_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(uint8_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(uint16_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(uint32_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(uint64_t value)
	{
		WriteObject(mStream, value);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(float32_t value)
	{
		mStream.write((char*)&value, sizeof(float32_t));
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const string& value)
	{
		uint32_t size = static_cast<uint32_t>(value.size());
		WriteObject(mStream, size);
		mStream.write(&value[0], size);
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const vec2& value)
	{
		*this << value.x;
		*this << value.y;
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const vec3& value)
	{
		*this << value.x;
		*this << value.y;
		*this << value.z;
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const vec4& value)
	{
		*this << value.x;
		*this << value.y;
		*this << value.z;
		*this << value.w;
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const mat4& value)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				*this << value[i][j];
			}
		}
		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(bool value)
	{
		WriteObject(mStream, static_cast<byte>(value ? 1 : 0));
		return *this;
	}

	template <typename T>
	void OutputStreamHelper::WriteObject(ostream& stream, T value)
	{
		stream.write(reinterpret_cast<char*>(&value), sizeof(T));
	}

#pragma endregion OutputStreamHelper

#pragma region InputStreamHelper

	InputStreamHelper::InputStreamHelper(istream& stream) :
		mStream(stream)
	{}

	istream& InputStreamHelper::Stream()
	{
		return mStream;
	}

	InputStreamHelper& InputStreamHelper::operator>>(int8_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(int16_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(int32_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(int64_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(uint8_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(uint16_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(uint32_t& value)
	{
		ReadObject(mStream, value);
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(uint64_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(float32_t& value)
	{
		mStream.read(reinterpret_cast<char*>(&value), sizeof(float32_t));

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(string& value)
	{
		uint32_t stringLength;
		ReadObject(mStream, stringLength);

		value.resize(stringLength);
		mStream.read(&value[0], stringLength);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(vec2& value)
	{
		*this >> value.x;
		*this >> value.y;
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(vec3& value)
	{
		*this >> value.x;
		*this >> value.y;
		*this >> value.z;
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(vec4& value)
	{
		*this >> value.x;
		*this >> value.y;
		*this >> value.z;
		*this >> value.w;
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(mat4& value)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				*this >> value[i][j];
			}
		}
		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(bool& value)
	{
		byte boolValue;
		ReadObject(mStream, boolValue);

		value = (boolValue == 1 ? true : false);

		return *this;
	}

	template <typename T>
	void InputStreamHelper::ReadObject(istream& stream, T& value)
	{
		stream.read(reinterpret_cast<char*>(&value), sizeof(T));
	}

#pragma endregion InputStreamHelper

}
