#pragma once

struct PacketHeader
{
	int iSize;
	JIGAPPacket::Type ePacketType;
	
};

class PacketHandler
{
private:
	char szParsingBuffer[MAXBUFFERSIZE];
	char szSerializeBuffer[MAXBUFFERSIZE];

	unsigned int iParsingPosition = 0;
	unsigned int iSerializePosition = 0;

	unsigned int iTotalSerializeBufferSize = 0;
	unsigned int iTotalParsingBufferSize = 0;
public:
	PacketHandler();
	virtual ~PacketHandler();

	int ParsingBufferSize(const char* inSzBuffer);
	void SerializeBufferSize(unsigned int iInSerializeSize);

	void ClearParsingBuffer(const char* inSzBuffer, unsigned int iInTotalBuffeRSize);
	void ClearSerializeBuffer();

public:
	template<class PacketType>
	void SerializePacket(JIGAPPacket::Type eInType, PacketType& inPacket)
	{
		memcpy(&szSerializeBuffer[iSerializePosition], &eInType, sizeof(JIGAPPacket::Type));
		iSerializePosition += sizeof(JIGAPPacket::Type);

		int serializeSize = inPacket.ByteSize();
		memcpy(&szSerializeBuffer[iSerializePosition], &serializeSize, sizeof(int));
		iSerializePosition += sizeof(int);

		inPacket.SerializeToArray(&szSerializeBuffer[iSerializePosition], inPacket.ByteSize());
		iSerializePosition += inPacket.ByteSize();

		SerializeBufferSize(iSerializePosition);
	}

public:
	void NextParsingHeader(PacketHeader& inPacketHeader);

	template<class PacketType>
	void NextParsingPacket(PacketType& inPacket, int iInSize)
	{
		inPacket.ParseFromArray(&szParsingBuffer[iParsingPosition], iInSize);
		iParsingPosition += iInSize;
	}

public:
	const char* GetSerializeBufferData() { return szSerializeBuffer; }
	int GetSerializeBufferSize() { return sizeof(szSerializeBuffer); }

	const char* GetParsingBufferData() { return szParsingBuffer; }
	int GetParsingBufferSize() { return sizeof(szParsingBuffer); }

	int GetSerializeRealSize() { return iSerializePosition; }
};

