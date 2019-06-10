using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class ChattingLauncher : NetworkObject
    {
        private InputLine ChattingLine = null;
        private ChattingView ChatView = null;

        public override void Init()
        {
            base.Init();
            BackGround backGround = ObjectManager.GetInst().AddObject<BackGround>(); 
            backGround.BGTexture = ImageManager.GetInst().LoadTexture("ChattingBackGround", "./Image/ChattingBackGround.png");

            ChattingLine = ObjectManager.GetInst().AddObject<InputLine>();
            ChattingLine.transform.position = new SharpDX.Vector3(360f, 640f, 0f);
            ChattingLine.FontColor = SharpDX.Color.Black;
            ChattingLine.FontSize = 30;
            ChattingLine.OnEnterCallBack = Send;
            ChattingLine.MaxTextLength = 50;

            ChatView = ObjectManager.GetInst().AddObject<ChattingView>();

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("Send", "./Image/Send.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 75, 75),
                ButtonPos = new SharpDX.Vector3(863f, 624f, 0f),
                ButtonEventCallBack = Send
            });

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("ExitButton", "./Image/ExitButton.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 114, 23),
                ButtonPos = new SharpDX.Vector3(821f, 6f, 0f),
                ButtonEventCallBack = NetworkManager.GetInst().RequestExitRoom
            });




            NetworkManager.GetInst().RequestChatting("Hello! Joined Room");
        }

        public void Send()
        {
            if (ChattingLine.InputText.Length > 0)
            {
                NetworkManager.GetInst().RequestChatting(ChattingLine.InputText.ToString());
                ChattingLine.InputText.Clear();
            }
        }
        public override void OnExitRoom(object sender, EventArgs e)
        {
            SceneManager.GetInst().ChanageScene("LobbyScene");
        }
    }
}
