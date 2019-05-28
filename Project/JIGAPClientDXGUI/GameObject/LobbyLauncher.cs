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
                ButtonPos = new SharpDX.Vector3(816, 650f, 0f),
                ButtonEventCallBack = CreateRoom
            });

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("Refresh", "./Image/Refresh.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 48, 48),
                ButtonPos = new SharpDX.Vector3(876, 67, 0f),
                ButtonEventCallBack = NetworkManager.GetInst().RequestRoomList
            });


            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("NextPage", "./Image/NextPage.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 38, 118),
                ButtonPos = new SharpDX.Vector3(884f, 301f, 0f),
                ButtonEventCallBack = ListView.NextPage
            });

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("BackPage", "./Image/BackPage.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 38, 118),
                ButtonPos = new SharpDX.Vector3(358f, 301f, 0f),
                ButtonEventCallBack = ListView.BackPage
            });

            NetworkManager.GetInst().RequestRoomList();
        }

        public override void Render()
        {
            LobbyBackGround.Draw();
        }

        public void CreateRoom()
        {
            if (RoomField.TextBoxInputLine.InputText.Length > 0)
            {
                NetworkManager.GetInst().RequestCreateRoom(RoomField.TextBoxInputLine.InputText.ToString());
                RoomField.TextBoxInputLine.InputText.Clear();
            }
        }

        public override void OnCreateRoom(object send, EventArgs e)
        {
            SceneManager.GetInst().ChanageScene("ChattingScene");
        }
        public override void OnCreateRoomFailed(object send, EventArgs e)
        {
            System.Windows.Forms.MessageBox.Show("방 생성에 실패했습니다. 이름이 같은 방이 있는지 확인해 주세요. 방 목록을 갱신합니다.");
        }
        public override void OnRoomList(object sender, EventArgs e)
        {
            ListView.UpdateRoomList();
        }
        public override void OnJoinedRoom(string rommName)
        {
            SceneManager.GetInst().ChanageScene("ChattingScene");
        }
        public override void OnJoinedRoomFailed(object sender, EventArgs e)
        {
            System.Windows.Forms.MessageBox.Show("방 참가에 실패했습니다. 방 목록을 갱신합니다!");
        }
    }
}
