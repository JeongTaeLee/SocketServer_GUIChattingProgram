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
        struct Chatting
        {
            public string Str;
            public bool MyChatting;
        }

        private Queue<Chatting> ChattingQueue = new Queue<Chatting>();

        private int FontSize = 30;
        private int MaxHeight = 11;


        private float PivotRight = 925;
        private float PivotLeft = 350;
        private float PivotBottom = 590;

        public override void Init()
        {
            transform.position = new SharpDX.Vector3(200, 79f, 0f);
            base.Init();    
        }

        public override void Render()
        {
            if (ChattingQueue.Count <= 0) return;

            Font font = new Font(DXManager.GetInst().d3dDevice, FontSize, 0, FontWeight.Bold, 0, false,
                FontCharacterSet.Default, FontPrecision.Default, FontQuality.Default, FontPitchAndFamily.Default, "맑은 고딕");

            int heightIndex = 0;
            for (int i = ChattingQueue.Count; i > 0; --i, ++heightIndex)
            {
                Vector3 printPos = new Vector3();
                printPos.Y = PivotBottom - heightIndex * 70;

                FontDrawFlags drawFlags = FontDrawFlags.NoClip;

                if (ChattingQueue.ToList()[i - 1].MyChatting)
                {
                    drawFlags = FontDrawFlags.NoClip | FontDrawFlags.Right;
                    printPos.X = PivotRight;
                }
                else
                {
                    drawFlags = FontDrawFlags.NoClip | FontDrawFlags.Left;
                    printPos.X = PivotLeft;
                }

                DXManager.GetInst().d3dSprite.Transform = Matrix.Translation(printPos);
                font.DrawText(DXManager.GetInst().d3dSprite, ChattingQueue.ToList()[i - 1].Str, new Rectangle(), drawFlags, Color.Black);
            }

            font.Dispose();
        }

        public override void OnRecvChatting(string sender, string message)
        {
            Chatting chat = new Chatting();
            
            if(sender == NetworkManager.GetInst().MyName)
            {
                chat.Str = message;// + " : [" + NetworkManager.GetInst().MyName + "]";
                chat.MyChatting = true;
            }
            else
                chat.Str = "[" + NetworkManager.GetInst().MyName + "] : " + message;

            ChattingQueue.Enqueue(chat);
            if (ChattingQueue.ToList().Count == MaxHeight)
                ChattingQueue.Dequeue();
        }
    }
}
