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
        public override void Init()
        {
            GameObject BackGround = ObjectManager.Instance.RegisterObject();
            BackGround.AddComponent<UIRenderer>().Texture = ImageManager.Instance.LoadTexture("IngameBackGround");

            GameObject CreateRoomTextBox = ObjectManager.Instance.RegisterObject();
            CreateRoomTextBox.transform.position = new SharpDX.Vector3(1026f, 67f, 0f);
            TextField CreateRoomField = CreateRoomTextBox.AddComponent<TextField>();
            CreateRoomField.Texture = ImageManager.Instance.LoadTexture("CreateRoomTextBox");
            CreateRoomField.FontSize = 40;

            GameObject CreateRoomButton = ObjectManager.Instance.RegisterObject();
            CreateRoomButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("CreateRoomButton"),
                1062, 143, 154, 40, null);

            GameObject ChattingTextBox = ObjectManager.Instance.RegisterObject();
            ChattingTextBox.transform.position = new SharpDX.Vector3(10f, 672f, 0f);
            TextField CattingField = ChattingTextBox.AddComponent<TextField>();
            CattingField.Texture = ImageManager.Instance.LoadTexture("ChattingTextBox");
            CattingField.FontSize = 35;
            
            GameObject SendButton = ObjectManager.Instance.RegisterObject();
            SendButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("SendButton"),
                916, 672, 77, 38, null);

            ObjectManager.Instance.RegisterObject().AddComponent<ChatLauncher>();
        }

        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }
    }
}
