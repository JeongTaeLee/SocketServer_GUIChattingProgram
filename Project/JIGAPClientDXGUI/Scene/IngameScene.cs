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
        private RoomUIPanel _createRoomPanel = null;
        private IngameChattingPanel _chattingPanel = null;
        private RoomListPanel _roomListPanel = null;

        public override void Init()
        {
            GameObject BackGround = ObjectManager.Instance.RegisterObject();
            BackGround.AddComponent<UIRenderer>().Texture = ResourceManager.Instance.LoadTexture("IngameBackGround");

            _createRoomPanel = ObjectManager.Instance.RegisterObject().AddComponent<RoomUIPanel>();
            _roomListPanel = ObjectManager.Instance.RegisterObject().AddComponent<RoomListPanel>();
            _chattingPanel = ObjectManager.Instance.RegisterObject().AddComponent<IngameChattingPanel>();
        }

        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }
    }
}
