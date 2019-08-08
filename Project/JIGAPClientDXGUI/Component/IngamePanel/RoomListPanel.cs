using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class RoomListPanel : NetworkComponent
    {
        private ListView _roomListView;
        private Button _nextButton;
        private Button _revertButton;

        public override void Init()
        {
            base.Init();

            _roomListView = ObjectManager.Instance.RegisterObject().AddComponent<ListView>();
            _roomListView.transform.Parent = transform;
            _roomListView.transform.position = new SharpDX.Vector3(1006f, 270f, 0f);
            _roomListView.PageMaxElementSize = 8;

            GameObject buttonObj = ObjectManager.Instance.RegisterObject();
            buttonObj.transform.Parent = transform;
            buttonObj.transform.position = new SharpDX.Vector3(1175f, 670f, 0f);
            buttonObj.AddComponent<UIRenderer>();
            _nextButton = buttonObj.AddComponent<Button>();
            _nextButton.SetButton(ResourceManager.Instance.LoadTexture("RoomListNextButton"), _roomListView.NextPage);

            buttonObj = ObjectManager.Instance.RegisterObject();
            buttonObj.transform.Parent = transform;
            buttonObj.transform.position = new SharpDX.Vector3(1024f, 670f, 0f);
            buttonObj.AddComponent<UIRenderer>();
            _revertButton = buttonObj.AddComponent<Button>();
            _revertButton.SetButton(ResourceManager.Instance.LoadTexture("RoomListRevertButton"), _roomListView.RevertPage);

           NetworkManager.Instance.SendProcess.SendRoomListRequest();
        }

        public override void OnRoomListSuccess(ref List<string> roomNameList)
        {
            base.OnRoomListSuccess(ref roomNameList);

            _roomListView.ClearListObject();

            foreach (string roomname in roomNameList)
            {
                GameObject obj = ObjectManager.Instance.RegisterObject();
                RoomListElement element = obj.AddComponent<RoomListElement>();
                element.SetRoomTitle(roomname);
                _roomListView.AddListObject(obj);
            }
            _roomListView.SettingList();
        }

        public override void OnJoinRoomSuccess(string roomName)
        {
            base.OnJoinRoomSuccess(roomName);
            NetworkManager.Instance.SendProcess.SendRoomListRequest();
        }
    }
}
