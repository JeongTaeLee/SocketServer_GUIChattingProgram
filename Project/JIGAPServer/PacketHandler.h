#pragma once

struct PacketHeader
{
	int iSize;
	JIGAPPacket::Type ePacketType;
	google::protobuf::Message* lpPacket;
	
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
	int ParsingBufferSize(const char * inSzBuffer);
	void SerializeBufferSize(unsigned int iInSerializeSize);

	void ClearParsingBuffer(const char* inSzBuffer, unsigned int iInTotalBuffeRSize);
	void ClearSerializeBuffer();

public:
	template<class PacketType>
	void SerializePacket(JIGAPPacket::Type eInType, PacketType& inPacket)
	{
		int serializeSize = inPacket.ByteSize();
		memcpy(&szSerializeBuffer[iSerializePosition], &serializeSize, sizeof(int));
		iSerializePosition += sizeof(int);

		memcpy(&szSerializeBuffer[iSerializePosition], &eInType, sizeof(JIGAPPacket::Type));
		iSerializePosition += sizeof(JIGAPPacket::Type);

		inPacket.SerializeToArray(&szSerializeBuffer[iSerializePosition], inPacket.ByteSize());
		iSerializePosition += inPacket.ByteSize();

		SerializeBufferSize(iSerializePosition);
	}

};

