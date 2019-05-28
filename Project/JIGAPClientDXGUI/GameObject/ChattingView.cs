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
        private Queue<string> ChattingStringQueue = new Queue<string>();
        private int FontSize = 20;
        public override void Init()
        {
            transform.position = new SharpDX.Vector3(379f, 79f, 0f);
            base.Init();    
        }

        public override void Render()
        {
            if (ChattingStringQueue.Count == 0) return;

            Font font = new Font(DXManager.GetInst().d3dDevice, FontSize, 0, FontWeight.Bold, 0, false, FontCharacterSet.Hangul, FontPrecision.Default,
                FontQuality.Default, FontPitchAndFamily.Default, "돋움");

            for(int i = 0; i < ChattingStringQueue.Count(); ++i)
            {
                Vector3 DrawPos = new Vector3(transform.position.X, (transform.position.Y + 390) - (FontSize * i), 0f);
                font.DrawText(DXManager.GetInst().d3dSprite, ChattingStringQueue.ToList()[i], (int)DrawPos.X, (int)DrawPos.Y, Color.Black);
            }

            font.Dispose();
        }

        public override void OnRecvChatting(string sender, string message)
        {
            base.OnRecvChatting(sender, message);
            string str = "[" + sender + "]" + " : " + message;
            ChattingStringQueue.Enqueue(str);

            if (ChattingStringQueue.Count == 6)
                ChattingStringQueue.Dequeue();
               
        }
    }
}
