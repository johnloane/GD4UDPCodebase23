#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0


class OutputMemoryStream
{
public:
	OutputMemoryStream()
	{
		ReallocBuffer(32);
	}

	~OutputMemoryStream() { std::free(mBuffer); }

	//get a pointer to the data in the stream
	const 	char*		GetBufferPtr()	const { return mBuffer; }
	uint32_t	GetLength()		const { return mHead; }

	void		Write(const void* inData,
		size_t inByteCount);

	template< typename T > void Write(T inData)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Write only supports primitive data types");

		if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
		{
			Write(&inData, sizeof(inData));
		}
		else
		{
			T swappedData = ByteSwap(inData);
			Write(&swappedData, sizeof(swappedData));
		}

	}

	void Write(const std::vector< int >& inIntVector)
	{
		size_t elementCount = inIntVector.size();
		Write(elementCount);
		Write(inIntVector.data(), elementCount * sizeof(int));
	}

	template< typename T >
	void Write(const std::vector< T >& inVector)
	{
		uint32_t elementCount = inVector.size();
		Write(elementCount);
		for (const T& element : inVector)
		{
			Write(element);
		}
	}

	void Write(const std::string& inString)
	{
		size_t elementCount = inString.size();
		Write(elementCount);
		Write(inString.data(), elementCount * sizeof(char));
	}

	void Write(Vector3 vec)
	{
		Write(vec.mX);
		Write(vec.mY);
		Write(vec.mZ);
	}

	void Write(Quaternion quat)
	{
		Write(quat.mX);
		Write(quat.mY);
		Write(quat.mZ);
		Write(quat.mW);
	}

	/*void Write(const GameObject* inGameObject)
	{
	uint32_t networkId = mLinkingContext->GetNetworkId(const_cast< GameObject* >(inGameObject), false);
	Write(networkId);
	}*/


private:
	void		ReallocBuffer(uint32_t inNewLength);

	char*		mBuffer;
	uint32_t	mHead;
	uint32_t	mCapacity;

	/*LinkingContext* mLinkingContext;*/
};

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount) :
		mBuffer(inBuffer), mCapacity(inByteCount), mHead(0) {}

	~InputMemoryStream() { std::free(mBuffer); }

	uint32_t		GetRemainingDataSize() const
	{
		return mCapacity - mHead;
	}

	void		Read(void* outData, uint32_t inByteCount);


	template< typename T > void Read(T& outData)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Read only supports primitive data types");
		Read(&outData, sizeof(outData));
	}

	template< typename T >
	void Read(std::vector< T >& outVector)
	{
		size_t elementCount;
		Read(elementCount);
		outVector.resize(elementCount);
		for (T& element : outVector)
		{
			Read(element);
		}
	}

	void Read(Vector3 vec)
	{
		Read(vec.mX);
		Read(vec.mY);
		Read(vec.mZ);
	}

	void Read(Quaternion quat)
	{
		Read(quat.mX);
		Read(quat.mY);
		Read(quat.mZ);
		Read(quat.mW);
	}

	/*void Read(GameObject*& outGameObject)
	{
	uint32_t networkId;
	Read(networkId);
	outGameObject = mLinkingContext->GetGameObject(networkId);
	}*/

private:
	char*		mBuffer;
	uint32_t	mHead;
	uint32_t	mCapacity;

	/*LinkingContext* mLinkingContext;*/
};




