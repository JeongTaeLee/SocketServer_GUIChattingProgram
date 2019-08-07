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
            // SingUPScene
            ResourceManager.Instance.LoadTexture("SingUpBackGround", "./Image/SingUpScene/SingUpBackGround.png");
            ResourceManager.Instance.LoadTexture("SingUpTextBox", "./Image/SingUpScene/SingUpTextBox.png");
            ResourceManager.Instance.LoadTexture("SingUpButton", "./Image/SingUpScene/SingUpButton.png");
            ResourceManager.Instance.LoadTexture("SingUpLoginButton", "./Image/SingUpScene/Login.png");
            
            // LoginScene
            ResourceManager.Instance.LoadTexture("LoginBackGround", "./Image/LoginScene/LoginBackGround.png");
            ResourceManager.Instance.LoadTexture("LoginTextBox", "./Image/LoginScene/LoginTextBox.png");
            ResourceManager.Instance.LoadTexture("LoginButton", "./Image/LoginScene/LoginButton.png");
            ResourceManager.Instance.LoadTexture("LoginSingUpButton", "./Image/LoginScene/SingUp.png");

            // IngameScene
            ResourceManager.Instance.LoadTexture("IngameBackGround", "./Image/Ingame/IngameBackGround.png");
            ResourceManager.Instance.LoadTexture("CreateRoomTextBox", "./Image/Ingame/CreateRoomTextBox.png");
            ResourceManager.Instance.LoadTexture("CreateRoomButton", "./Image/Ingame/CreateRoomButton.png");
            ResourceManager.Instance.LoadTexture("RoomListElement", "./Image/Ingame/RoomListElement.png");
            ResourceManager.Instance.LoadTexture("ChattingTextBox", "./Image/Ingame/ChattingTextBox.png");
            ResourceManager.Instance.LoadTexture("SendButton", "./Image/Ingame/SendButton.png");
            ResourceManager.Instance.LoadTexture("RoomListRevertButton", "./Image/Ingame/RoomListRevertButton.png");
            ResourceManager.Instance.LoadTexture("RoomListNextButton", "./Image/Ingame/RoomListNextButton.png");


            //ResourceManager 
            ResourceManager.Instance.LoadFont("Default", 50);
            ResourceManager.Instance.LoadFont("RoomListElement", 40);
            ResourceManager.Instance.LoadFont("Chatting", 30);

            SceneManager.Instance.ChanageScene("LoginScene");
        }

        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

    }
}
