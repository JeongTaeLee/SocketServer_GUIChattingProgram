using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JIGAPClientDXGUI.Engine;
using System.Threading;

using SharpDX;
using SharpDX.Direct3D9;
namespace JIGAPClientDXGUI
{
    class ChattingView : NetworkObject 
    {
        private Boolean mtxSuccess;
        private Mutex mtx;

        struct Chatting
        {
            public string Str;
            public bool MyChatting;
        }

        private Queue<Chatting> ChattingQueue = new Queue<Chatting>();

        private int FontSize = 30;
        private int MaxHeight = 8;


        private float PivotRight = 925;
        private float PivotLeft = 350;
        private float PivotBottom = 500;

        public override void Init()
        {
            mtx = new Mutex(true, "ChattingQueueMutex", out mtxSuccess);

            base.Init();    
        }

        public override void Render()
        {

            if (ChattingQueue.Count <= 0) return;

            Font font = new Font(DXManager.GetInst().d3dDevice, FontSize, 0, FontWeight.Bold, 0, false,
                FontCharacterSet.Default, FontPrecision.Default, FontQuality.Default, FontPitchAndFamily.Default, "맑은 고딕");

            int heightIndex = 0;
            for (int i = (ChattingQueue.Count - 1); i >= 0; --i, ++heightIndex)
            {
                Vector3 printPos = new Vector3();
                printPos.Y = PivotBottom - heightIndex * 70;

                FontDrawFlags drawFlags = FontDrawFlags.NoClip;
                bool MyChatting = ChattingQueue.ToList()[i].MyChatting;

                if (MyChatting)
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
                string qeueMessage = ChattingQueue.ToList()[i].Str;
                font.DrawText(DXManager.GetInst().d3dSprite, qeueMessage, new Rectangle(), drawFlags, Color.Black);
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
                chat.Str = "[" + sender + "] : " + message;

            ChattingQueue.Enqueue(chat);
            if (ChattingQueue.ToList().Count >= MaxHeight)
                ChattingQueue.Dequeue();
        }
    }
}
