﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI
{
    class RoomUIPanel : NetworkComponent
    { 
        private Text _roomTitleText = null;
        private TextField _createRoomTitleField = null;
        //private Button button = 
        public override void Init()
        {
            base.Init();

            Button button = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            
            button.transform.position = new SharpDX.Vector3(1062f, 143f, 0f);
            button.transform.Parent = transform;
            button.SetButton(ResourceManager.Instance.LoadTexture("CreateRoomButton"), SendCreateRoomPacket);

            #region CreateRoomTitleField
            _createRoomTitleField = ObjectManager.Instance.RegisterObject().AddComponent<TextField>();
            _createRoomTitleField.transform.Parent = this.transform;
            _createRoomTitleField.transform.position = new Vector3(1026f, 67f, 0f);
            _createRoomTitleField.Texture = ResourceManager.Instance.LoadTexture("CreateRoomTextBox");
            _createRoomTitleField.EnterBehavior = SendCreateRoomPacket;
            #endregion

            #region RoomTitle 
            _roomTitleText = ObjectManager.Instance.RegisterObject().AddComponent<Text>();
            _roomTitleText.transform.Parent = this.transform;
            _roomTitleText.transform.position = new Vector3(510f, 25f, 0f); 
            _roomTitleText.drawFlag = FontDrawFlags.Center | FontDrawFlags.VerticalCenter | FontDrawFlags.NoClip;
            _roomTitleText.text.Clear();
            _roomTitleText.text.Append(NetworkManager.Instance.RoomName);
            #endregion


        }

        public void SendCreateRoomPacket()
        {
            if (_createRoomTitleField.String.Length > 0)
                NetworkManager.Instance.SendProcess.SendCreateRoomRequest(_createRoomTitleField.String);
        }
        public void SendCreateRoomPacket(string str)
        {
            if (_createRoomTitleField.String.Length > 0)
                NetworkManager.Instance.SendProcess.SendCreateRoomRequest(_createRoomTitleField.String);
        }

        public override void OnJoinRoomSuccess(string roomName)
        {
            base.OnJoinRoomSuccess(roomName);
            _roomTitleText.text.Clear();
            _roomTitleText.text.Append(roomName);

        }

        public override void OnJoinRoomFailed()
        {
            base.OnJoinRoomFailed();
            System.Windows.Forms.MessageBox.Show($"방 입장 실패. 방 목록을 갱신합니다.", "Test");
            NetworkManager.Instance.SendProcess.SendRoomListRequest();
        }

        public override void OnCreateRoomSuccess(string inRoomName)
        {
            base.OnCreateRoomSuccess(inRoomName);
            NetworkManager.Instance.SendProcess.SendJoinRoom(inRoomName);
        }
        public override void OnCreateRoomFailed()
        {
            base.OnCreateRoomFailed();
            System.Windows.Forms.MessageBox.Show("방 생성 실패", "방 생성");
        }
    }
}
