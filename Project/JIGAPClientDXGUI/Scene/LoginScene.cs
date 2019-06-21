using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using ProtoBuf;

using JIGAPClientDXGUI.Engine;
using JIGAPPacket;
using Google.Protobuf;

namespace JIGAPClientDXGUI
{
    public class LoginScene : Scene
    {

        public override void Init()
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

        }
        public override void Release()
        {
        }


    }
}
