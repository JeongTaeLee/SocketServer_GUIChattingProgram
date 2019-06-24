using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPPacket;
using Google.Protobuf;
using Google.Protobuf.Collections;

namespace JIGAPClientDXGUI.Engine
{
    partial class PacketHandler
    {
        //const int message_header_size = sizeof(int) * 2;
        //LoginRequestPacket packet = new LoginRequestPacket { Id = "Test" };

        //byte[] readBuffer = new byte[1024];
        //byte[] buffer = new byte[1024];
        //Buffer.BlockCopy(buffer, 0, readBuffer, packetheadersize, packetsize);

        //LoginRequestPacket.Parser.ParseFrom(buffer);


        //byte[] buffer = new byte[message_header_size + packet.CalculateSize()];
        //
        //
        //packet.ToByteArray().CopyTo(buffer, message_header_size);

        public const int bufferSize = 2048;

        public byte[] recvBuffer { get; set; } = new byte[bufferSize];
        public byte[] sendBuffer { get; set; } = new byte[bufferSize];

        public int recvPosition { get; private set; } = 0;
        public int sendPosition { get; private set; } = 0;

        private int recvTotalSize = 0;

    }

    partial class PacketHandler
    {
        public void SetRecvBuffer(byte[] inRecvBuffer, int inTotalSize)
        {
            Array.Clear(recvBuffer, 0, bufferSize);

            Buffer.BlockCopy(inRecvBuffer, 0, recvBuffer, 0, inTotalSize);
            recvTotalSize = inTotalSize;
            recvPosition = 4;
        }
        public void ClearSendBuffer()
        {
            Array.Clear(sendBuffer, 0, bufferSize);
            sendPosition = 4;
        }

        public int ParsingPacketSize(byte[] inRecvBuffer)
        {
            return BitConverter.ToInt32(inRecvBuffer, sizeof(int) * 0);
        }

        public MessageHeader ParsingPacketHeader()
        {
            PacketType type = (PacketType)BitConverter.ToInt32(recvBuffer, recvPosition);
            recvPosition += sizeof(int);
            int byteSizes = BitConverter.ToInt32(recvBuffer, recvPosition);
            recvPosition += sizeof(int);

            MessageHeader header = new MessageHeader();
            header.Type = type;
            header.Size = byteSizes;

            return header;
        }

        public Packet PrasingPacket<Packet>(int inSize) where Packet : Google.Protobuf.IMessage, new ()
        {
            byte[] buffer = new byte[inSize];

            Packet packet = new Packet();
            packet = (Packet)packet.Descriptor.Parser.ParseFrom(recvBuffer, recvPosition, inSize);

            recvPosition += inSize;

            return packet;
        }

        private void SerializePacketSize(int size)
        {
            Buffer.BlockCopy(BitConverter.GetBytes(size), 0, sendBuffer, 0, sizeof(int));
        }

        public void SerializePacket<Packet>(PacketType inType, Packet inPacket) where Packet : Google.Protobuf.IMessage
        {
            Buffer.BlockCopy(BitConverter.GetBytes((int)inType), 0, sendBuffer, sendPosition, sizeof(int));
            sendPosition += sizeof(int);
            Buffer.BlockCopy(BitConverter.GetBytes(inPacket.CalculateSize()), 0, sendBuffer, sendPosition, sizeof(int));
            sendPosition += sizeof(int);

            Buffer.BlockCopy(inPacket.ToByteArray(), 0, sendBuffer, sendPosition, inPacket.CalculateSize());
            sendPosition += inPacket.CalculateSize();

            SerializePacketSize(sendPosition);
        }
    }

}
