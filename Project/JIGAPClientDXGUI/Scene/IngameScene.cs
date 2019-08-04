using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class IngameScene : Scene
    {
        CreateRoomPanel createRoomPanel = null;
        IngameChattingPanel chattingPanel = null;
        RoomListPanel roomListPanel = null;

        public override void Init()
        {
            GameObject BackGround = ObjectManager.Instance.RegisterObject();
            BackGround.AddComponent<UIRenderer>().Texture = ResourceManager.Instance.LoadTexture("IngameBackGround");

            ObjectManager.Instance.RegisterObject().AddComponent<ChatLauncher>();

            createRoomPanel = ObjectManager.Instance.RegisterObject().AddComponent<CreateRoomPanel>();
            roomListPanel = ObjectManager.Instance.RegisterObject().AddComponent<RoomListPanel>();
            chattingPanel = ObjectManager.Instance.RegisterObject().AddComponent<IngameChattingPanel>();

        }

        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }
    }
}
