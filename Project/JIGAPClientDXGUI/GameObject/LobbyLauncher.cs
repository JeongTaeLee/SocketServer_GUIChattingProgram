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
        private InputLine inputRoomName = null;
        private RoomListView ListView = null;

        public override void Init()
        {
            base.Init();

            BackGround backGround = ObjectManager.GetInst().AddObject<BackGround>();
            backGround.BGTexture = ImageManager.GetInst().LoadTexture("LobbyBackGround", "./Image/LobbyBackGround.png");

            inputRoomName = ObjectManager.GetInst().AddObject<InputLine>();
            inputRoomName.transform.position = new SharpDX.Vector3(935f, 60f, 0f);
            inputRoomName.FontColor = SharpDX.Color.Black;
            inputRoomName.FontSize = 70;
            inputRoomName.SetCenterText();
            inputRoomName.OnEnterCallBack = CreateRoom;

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("CreateRoomButton", "./Image/CreateRoomButton.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 624, 76),
                ButtonPos = new SharpDX.Vector3(629f, 147f, 0f),
                ButtonEventCallBack = CreateRoom
            });

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("Refresh", "./Image/Refresh.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 139, 33),
                ButtonPos = new SharpDX.Vector3(457f, 14f, 0f),
                ButtonEventCallBack = NetworkManager.GetInst().RequestRoomList
            });


            ListView = ObjectManager.GetInst().AddObject<RoomListView>();
            
            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("NextPage", "./Image/NextPage.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 281, 43),
                ButtonPos = new SharpDX.Vector3(319f, 656f, 0f),
                ButtonEventCallBack = ListView.NextPage
            });
            
            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("BackPage", "./Image/BackPage.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 281, 43),
                ButtonPos = new SharpDX.Vector3(29f, 656f, 0f),
                ButtonEventCallBack = ListView.BackPage
            });


            NetworkManager.GetInst().RequestRoomList();
        }

        public void CreateRoom()
        {
            if (inputRoomName.InputText.Length > 0)
            {
                NetworkManager.GetInst().RequestCreateRoom(inputRoomName.InputText.ToString());
                inputRoomName.InputText.Clear();
            }
        }

        public override void OnCreateRoom(object send, EventArgs e)
        {
            SceneManager.GetInst().ChanageScene("ChattingScene");
            NetworkManager.GetInst().RequestChatting("방에 입장하였습니다");
        }
        public override void OnCreateRoomFailed(object send, EventArgs e)
        {
            System.Windows.Forms.MessageBox.Show("방 생성에 실패했습니다. 이름이 같은 방이 있는지 확인해 주세요. 방 목록을 갱신합니다.");
            NetworkManager.GetInst().RequestRoomList();
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
            NetworkManager.GetInst().RequestRoomList();
        }
    }
}
