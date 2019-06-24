using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using JIGAPClientDXGUI.Engine;
using Google.Protobuf.Collections;
using Google.Protobuf.Reflection;
using Google.Protobuf.WellKnownTypes;

namespace JIGAPClientDXGUI
{
    class LoadingScene : Scene
    {
        public override void Init()
        {
            // LoginScene
            ImageManager.Instance.LoadTexture("LoginBackGround", "./Image/LoginScene/LoginBackGround.png");
            ImageManager.Instance.LoadTexture("LoginTextBox", "./Image/LoginScene/LoginTextBox.png");
            ImageManager.Instance.LoadTexture("LoginButton", "./Image/LoginScene/LoginButton.png");

            // IngameScene
            ImageManager.Instance.LoadTexture("IngameBackGround", "./Image/Ingame/IngameBackGround.png");
            ImageManager.Instance.LoadTexture("CreateRoomTextBox", "./Image/Ingame/CreateRoomTextBox.png");
            ImageManager.Instance.LoadTexture("CreateRoomButton", "./Image/Ingame/CreateRoomButton.png");
            ImageManager.Instance.LoadTexture("RoomListElement", "./Image/Ingame/RoomListElement.png");
            ImageManager.Instance.LoadTexture("ChattingTextBox", "./Image/Ingame/ChattingTextBox.png");
            ImageManager.Instance.LoadTexture("SendButton", "./Image/Ingame/SendButton.png");

            SceneManager.Instance.ChanageScene("LoginScene");
        }

        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

    }
}
