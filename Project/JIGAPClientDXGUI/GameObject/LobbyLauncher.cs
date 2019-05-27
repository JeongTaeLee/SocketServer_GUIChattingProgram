using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class LobbyLauncher : NetworkObject
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
            RoomField.OnEnterCallBack = CreateRoom;

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("CreateRoomButton", "./Image/CreateRoomButton.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 90, 55),
                ButtonPos = new SharpDX.Vector3(816, 620f, 0f),
                ButtonEventCallBack = CreateRoom
            });

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("Refresh", "./Image/Refresh.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 48, 48),
                ButtonPos = new SharpDX.Vector3(876, 67, 0f),
                ButtonEventCallBack = RefreshRoomList
            });

            RefreshRoomList();
        }

        public override void Render()
        {
            LobbyBackGround.Draw();
        }

        public void RefreshRoomList()
        {
            NetworkManager.GetInst().RequestRoomList();
        }

        public void CreateRoom()
        {
            if (RoomField.TextBoxInputLine.InputText.Length > 0)
                NetworkManager.GetInst().RequestCreateRoom(RoomField.TextBoxInputLine.InputText.ToString());
        }

        public override void OnCreateRoom(object send, EventArgs e)
        {
            SceneManager.GetInst().ChanageScene("ChattingScene");
        }
        public override void OnCreateRoomFailed(object send, EventArgs e)
        {
        }
        public override void OnRoomList(object sender, EventArgs e)
        {
            ListView.UpdateRoomList();
        }
        public override void OnJoinedRoom(object sender, EventArgs e)
        {
            SceneManager.GetInst().ChanageScene("ChattingScene");
        }
        public override void OnJoinedRoomFailed(object sender, EventArgs e)
        {
            
        }
    }
}
