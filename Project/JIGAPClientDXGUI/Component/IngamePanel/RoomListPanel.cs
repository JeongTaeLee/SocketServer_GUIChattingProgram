using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class RoomListPanel : NetworkComponent
    {
        private ListView roomListView;
        private Button nextButton;
        private Button revertButton;

        public override void Init()
        {
            base.Init();

            roomListView = ObjectManager.Instance.RegisterObject().AddComponent<ListView>();
            roomListView.transform.Parent = transform;
            roomListView.transform.position = new SharpDX.Vector3(1006f, 270f, 0f);
            roomListView.PageMaxElementSize = 8;

            GameObject buttonObj = ObjectManager.Instance.RegisterObject();
            buttonObj.transform.Parent = transform;
            buttonObj.transform.position = new SharpDX.Vector3(1175f, 670f, 0f);
            buttonObj.AddComponent<UIRenderer>();
            nextButton = buttonObj.AddComponent<Button>();
            nextButton.SetButton(ResourceManager.Instance.LoadTexture("RoomListNextButton"), roomListView.NextPage);

            buttonObj = ObjectManager.Instance.RegisterObject();
            buttonObj.transform.Parent = transform;
            buttonObj.transform.position = new SharpDX.Vector3(1024f, 670f, 0f);
            buttonObj.AddComponent<UIRenderer>();
            revertButton = buttonObj.AddComponent<Button>();
            revertButton.SetButton(ResourceManager.Instance.LoadTexture("RoomListRevertButton"), roomListView.RevertPage);

           NetworkManager.Instance.SendProcess.SendRoomListRequest();
        }

        public override void OnRoomListSuccess(ref List<string> roomNameList)
        {
            base.OnRoomListSuccess(ref roomNameList);

            foreach (string roomname in roomNameList)
            {
                GameObject obj = ObjectManager.Instance.RegisterObject();
                RoomListElement element = obj.AddComponent<RoomListElement>();
                element.SetRoomTitle(roomname);
                roomListView.AddListObject(obj);
            }
            roomListView.SettingList();
        }

    }
}
