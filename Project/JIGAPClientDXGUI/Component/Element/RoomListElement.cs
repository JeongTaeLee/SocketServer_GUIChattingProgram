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
        private UIRenderer _uiRenderer = null;
        private Button _button = null;
        private Text _text = null;

        public override void Init()
        {
            base.Init();
            _uiRenderer = gameObject.AddComponent<UIRenderer>();
            _button = gameObject.AddComponent<Button>();
            _button.SetButton( ResourceManager.Instance.LoadTexture("RoomListElement"), () => { ClickRoomButton(_text.text.ToString()); });

            _text = ObjectManager.Instance.RegisterObject().AddComponent<Text>();
            _text.transform.Parent = transform;
            _text.transform.position = new Vector3(_uiRenderer.Texture.d3dInfo.Width / 2f, _uiRenderer.Texture.d3dInfo.Height / 2f, 0f);
            _text.drawFlag = SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.VerticalCenter | SharpDX.Direct3D9.FontDrawFlags.NoClip;
            _text.font = ResourceManager.Instance.LoadFont("RoomListElement");

            _text.text.Append("Test");
        }

        public void SetRoomTitle(string inRoomTitle)
        {
            _text.text.Clear();
            _text.text.Append(inRoomTitle);
            
        }

        private void ClickRoomButton(string roomName)
        {
            NetworkManager.Instance.SendProcess.SendJoinRoom(roomName);
        }

        

    }
}
