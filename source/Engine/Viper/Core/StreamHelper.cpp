#include "Pch.h"
#include "StreamHelper.h"

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

	OutputStreamHelper& OutputStreamHelper::operator<<(float value)
	{
		mStream.write((char*)&value, sizeof(float));

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const string& value)
	{
		uint32_t size = static_cast<uint32_t>(value.size());
		WriteObject(mStream, size);
		mStream.write(&value[0], size);

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
		for (uint32_t size = sizeof(T); size > 0; --size, value >>= 8)
		{
			stream.put(static_cast<char>(value & 0xFF));
		}
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

	InputStreamHelper& InputStreamHelper::operator >> (int32_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator >> (int64_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator >> (uint32_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator >> (uint64_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator >> (float& value)
	{
		mStream.read((char*)&value, sizeof(float));

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator >> (string& value)
	{
		uint32_t stringLength;
		ReadObject(mStream, stringLength);

		value.resize(stringLength);
		mStream.read(&value[0], stringLength);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator >> (mat4& value)
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

	InputStreamHelper& InputStreamHelper::operator >> (bool& value)
	{
		byte boolValue;
		ReadObject(mStream, boolValue);

		value = (boolValue == 1 ? true : false);

		return *this;
	}

	template <typename T>
	void InputStreamHelper::ReadObject(istream& stream, T& value)
	{
		value = 0;

		for (uint32_t size = 0; size < sizeof(T); ++size)
		{
			value |= stream.get() << (8 * size);
		}
	}

#pragma endregion InputStreamHelper

}
