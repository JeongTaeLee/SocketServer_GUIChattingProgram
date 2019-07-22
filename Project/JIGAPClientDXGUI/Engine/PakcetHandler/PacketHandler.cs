using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPPacket;
using Google.Protobuf;
using Google.Protobuf.Collections;

namespace JIGAPClientDXGUI
{
     struct PacketHeader
    {
        public JIGAPPacket.Type Type;
        public int PacketSize;
    }


    partial class PacketHandler
    {
        public const int bufferSize = 2048;

        public byte[] parsingBuffer { get; private set; } = new byte[bufferSize];
        public byte[] serializeBuffer { get; private set; } = new byte[bufferSize];

        public int parsingPosition { get; private set; } = 0;
        public int serializePosition { get; private set; } = 0;

    }

    partial class PacketHandler
    {
        public void ClearParsingBuffer(byte[] inParsingBuffer, int inISize)
        {
            Array.Clear(parsingBuffer, 0, bufferSize);
            parsingPosition = sizeof(int);
            Buffer.BlockCopy(inParsingBuffer, 0, parsingBuffer, 0, inISize);
        }

        public void ClearSerializeBuffer()
        {
            Array.Clear(serializeBuffer, 0, bufferSize);
            serializePosition = sizeof(int);
            SerializeBufferSize(serializePosition);
        }

        public void SerializeBufferSize(int inISize)
        {
            Buffer.BlockCopy(BitConverter.GetBytes(inISize), 0, serializeBuffer, 0, sizeof(int));
        }

        public void SerializePacket<Packet>(JIGAPPacket.Type inType, Packet inPacket) where Packet : Google.Protobuf.IMessage
        {
            Buffer.BlockCopy(BitConverter.GetBytes((int)inType), 0, serializeBuffer, serializePosition, sizeof(JIGAPPacket.Type));
            serializePosition += sizeof(JIGAPPacket.Type);
            Buffer.BlockCopy(BitConverter.GetBytes(inPacket.CalculateSize()), 0, serializeBuffer, serializePosition, sizeof(int));
            serializePosition += sizeof(int);

            Buffer.BlockCopy(inPacket.ToByteArray(), 0, serializeBuffer, serializePosition, inPacket.CalculateSize());
            serializePosition += inPacket.CalculateSize();

            SerializeBufferSize(serializePosition);
        }

        public int ParsingTotalPacketSize(byte[] packetArray)
        {
            int totalSize = BitConverter.ToInt32(packetArray, 0);
            return totalSize;
        }

        public void ParsingHeader(ref PacketHeader header)
        {
            header.Type = (JIGAPPacket.Type)BitConverter.ToInt32(parsingBuffer, parsingPosition);
            parsingPosition += sizeof(JIGAPPacket.Type);
            header.PacketSize = BitConverter.ToInt32(parsingBuffer, parsingPosition);
            parsingPosition += sizeof(int);
        }
        public void ParsingPacket<Packet>(ref Packet inPacket, int inPacketSize) where Packet : Google.Protobuf.IMessage
        {
            inPacket = (Packet)inPacket.Descriptor.Parser.ParseFrom(parsingBuffer, parsingPosition, inPacketSize);
            parsingPosition += inPacketSize;
        }
    }

}
