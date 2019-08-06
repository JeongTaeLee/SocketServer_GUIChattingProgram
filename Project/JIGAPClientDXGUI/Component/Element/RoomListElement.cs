using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace JIGAPClientDXGUI
{
    class RoomListElement : Component
    {
        private UIRenderer uiRenderer = null;
        private Button button = null;
        private Text text = null;

        public override void Init()
        {
            base.Init();
            uiRenderer = gameObject.AddComponent<UIRenderer>();
            button = gameObject.AddComponent<Button>();
            button.SetButton( ResourceManager.Instance.LoadTexture("RoomListElement"), () => { ClickRoomButton(text.text.ToString()); });

            text = ObjectManager.Instance.RegisterObject().AddComponent<Text>();
            text.transform.Parent = transform;
            text.transform.position = new Vector3(uiRenderer.Texture.d3dInfo.Width / 2f, uiRenderer.Texture.d3dInfo.Height / 2f, 0f);
            text.DrawFlag = SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.VerticalCenter | SharpDX.Direct3D9.FontDrawFlags.NoClip;
            text.Font = ResourceManager.Instance.LoadFont("RoomListElement");

            text.text.Append("Test");
        }

        public void SetRoomTitle(string inRoomTitle)
        {
            text.text.Clear();
            text.text.Append(inRoomTitle);
            
        }

        private void ClickRoomButton(string roomName)
        {
            NetworkManager.Instance.SendProcess.SendJoinRoom(roomName);
        }

    }
}
