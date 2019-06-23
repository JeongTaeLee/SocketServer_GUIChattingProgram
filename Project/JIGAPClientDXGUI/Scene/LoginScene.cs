using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using ProtoBuf;

using JIGAPClientDXGUI.Engine;

using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI
{
    public class LoginScene : Scene
    {

        public override void Init()
        {
            NetworkManager.Instance.ConnectServer();

            GameObject obj = ObjectManager.Instance.RegisterObject();
            obj.AddComponent<UIRenderer>().Texture = ImageManager.Instance.LoadTexture("LoginBackGround");

            obj = ObjectManager.Instance.RegisterObject();
            TextField field= obj.AddComponent<TextField>();
            field.Texture = ImageManager.Instance.LoadTexture("LoginTextBox");
            field.String = "Login 정보를 입력해주세요";
            field.EnterBehavior = (string str) => { Console.WriteLine(str); };
            obj.transform.position = new SharpDX.Vector3(429f, 315f, 0f);


            obj = ObjectManager.Instance.RegisterObject();
            obj.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("LoginButton"), 560f, 418f, 223, 58, () => { Console.WriteLine(field.String); });
            


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
