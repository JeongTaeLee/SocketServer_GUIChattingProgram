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
        private texture ChattingBackGround = null;
        private StandardInputField ChattingField = null;
        private ChattingView ChatView = null;

        public override void Init()
        {
            base.Init();
            ChattingBackGround = ImageManager.GetInst().LoadTexture("ChattingBackGround", "./Image/ChattingBackGround.png");

            ChatView = ObjectManager.GetInst().AddObject<ChattingView>();
  

            ChattingField = ObjectManager.GetInst().AddObject<StandardInputField>();
            ChattingField.OnEnterCallBack = Send;


            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("Send", "./Image/Send.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 90, 55),
                ButtonPos = new SharpDX.Vector3(816, 650f, 0f),
                ButtonEventCallBack = Send
            });

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("ExitButton", "./Image/ExitButton.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 109, 51),
                ButtonPos = new SharpDX.Vector3(368f, 5f, 0f),
                ButtonEventCallBack = NetworkManager.GetInst().RequestExitRoom
            });
        }

        public override void Render()
        {
            ChattingBackGround.Draw();
        }

        public void Send()
        {
            if (ChattingField.TextBoxInputLine.InputText.Length > 0)
            {
                NetworkManager.GetInst().RequestChatting(ChattingField.TextBoxInputLine.InputText.ToString());
                ChattingField.TextBoxInputLine.InputText.Clear();
            }
        }
        public override void OnExitRoom(object sender, EventArgs e)
        {
            SceneManager.GetInst().ChanageScene("LobbyScene");
        }
    }
}
