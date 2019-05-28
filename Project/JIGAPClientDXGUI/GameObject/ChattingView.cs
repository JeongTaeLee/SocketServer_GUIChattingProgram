using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JIGAPClientDXGUI.Engine;

using SharpDX;
using SharpDX.Direct3D9;
namespace JIGAPClientDXGUI
{
    class ChattingView : NetworkObject 
    {
        struct ChatBox
        {
            public string Str;
            public bool MyMessage;
        }
        

        private Queue<ChatBox> ChattingStringQueue = new Queue<ChatBox>();
        private int FontSize = 25;
        public override void Init()
        {
            transform.position = new SharpDX.Vector3(200, 79f, 0f);
            base.Init();    
        }

        public override void Render()
        {
            if (ChattingStringQueue.Count == 0) return;

            Font font = new Font(DXManager.GetInst().d3dDevice, FontSize, 0, FontWeight.Bold, 0, false, FontCharacterSet.Hangul, FontPrecision.Default,
                FontQuality.Default, FontPitchAndFamily.Default, "돋움");

            for(int i = 0; i < ChattingStringQueue.Count(); ++i)
            {
                Vector3 DrawPos = new Vector3(DXManager.GetInst().Width / 2f, 0f, 0f);
                DrawPos.X = 360f;
                DrawPos.Y = (transform.position.Y + 500) - (FontSize * (ChattingStringQueue.Count - i));

                DXManager.GetInst().d3dSprite.Transform = Matrix.Translation(DrawPos);
                font.DrawText(DXManager.GetInst().d3dSprite, ChattingStringQueue.ToArray()[i].Str, 0,0, Color.Black);
            }

            font.Dispose();
        }

        public override void OnRecvChatting(string sender, string message)
        {
            base.OnRecvChatting(sender, message);

            ChatBox Chatbox = new ChatBox();
            Chatbox.Str = "[" + sender + "]" + " : " + message;
            Chatbox.MyMessage = (sender == NetworkManager.GetInst().MyName);
            Console.Write(NetworkManager.GetInst().MyName);

            ChattingStringQueue.Enqueue(Chatbox);

            if (ChattingStringQueue.Count == 10)
                ChattingStringQueue.Dequeue();
               
        }
    }
}
