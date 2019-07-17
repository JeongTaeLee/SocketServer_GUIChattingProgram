#include "pch.h"
#include "PacketHandler.h"

int PacketHandler::ParsingBufferSize(const char* inSzBuffer)
{
	unsigned int bufferSize = 0;
	memcpy(&bufferSize, inSzBuffer, sizeof(unsigned int));
	return bufferSize;
}

void PacketHandler::SerializeBufferSize(unsigned int iInSerializeSize)
{
	iTotalSerializeBufferSize = iInSerializeSize;
	memcpy(szSerializeBuffer, &iTotalSerializeBufferSize, sizeof(iTotalSerializeBufferSize));
}


void PacketHandler::ClearParsingBuffer(const char* inSzBuffer, unsigned int iInTotalBufferSize)
{
	iParsingPosition = sizeof(unsigned int);
	iTotalParsingBufferSize = iInTotalBufferSize;
	memcpy(szParsingBuffer, inSzBuffer, sizeof(char) * iTotalParsingBufferSize);
}

void PacketHandler::ClearSerializeBuffer()
{
	memset(szSerializeBuffer, 0, sizeof(szSerializeBuffer));
	iSerializePosition			= sizeof(unsigned int);
	
	SerializeBufferSize(iSerializePosition);
}
