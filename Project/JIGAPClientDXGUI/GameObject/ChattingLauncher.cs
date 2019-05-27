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
        private texture LobbyBackGround = null;
        private StandardInputField RoomField = null;
        private RoomListView ListView = null;

        public override void Init()
        {
            base.Init();
            LobbyBackGround = ImageManager.GetInst().LoadTexture("LobbyBackGround", "./Image/LobbyBackGround.png");

            ListView = ObjectManager.GetInst().AddObject<RoomListView>();

            RoomField = ObjectManager.GetInst().AddObject<StandardInputField>();
            RoomField.OnEnterCallBack = Send;

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("Send", "./Image/Send.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 90, 55),
                ButtonPos = new SharpDX.Vector3(816, 620f, 0f),
                ButtonEventCallBack = Send
            });
        }

        public override void Render()
        {
            LobbyBackGround.Draw();
        }

        public void Send()
        {

        }

        public override void OnExitRoom(object sender, EventArgs e)
        {
            
        }
    }
}
