using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class LoadingScene : Scene
    {
        public override void Init()
        {
            ImageManager.GetInst().LoadTexture("NConnectBackGround", "./Image/새로운UI/ConnectBackGround.png");
            ImageManager.GetInst().LoadTexture("NConnectInfoTextBox", "./Image/새로운UI/ConnectInfoTextBox.png");
            ImageManager.GetInst().LoadTexture("NConnectButton", "./Image/새로운UI/ConnectButton.png");


            ImageManager.GetInst().LoadTexture("NLoginBackGround", "./Image/새로운UI/LoginUI.png");
            ImageManager.GetInst().LoadTexture("NLoginTextBox", "./Image/새로운UI/LoginTextBox.png");

            //
            ImageManager.GetInst().LoadTexture("NInGamebackGround", "./Image/새로운UI/IngameBackGround.png");
            ImageManager.GetInst().LoadTexture("NCreateRoomTextBox", "./Image/새로운UI/CreateRoomTextBox.png");
            ImageManager.GetInst().LoadTexture("NCreateRoomButton", "./Image/새로운UI/CreateRoomButton.png");
            ImageManager.GetInst().LoadTexture("NRoomListElement", "./Image/새로운UI/RoomListElement.png");

            SceneManager.GetInst().ChanageScene("ConnectScene");
        }

        public override void Release()
        {
        }

        public override void Render()
        {
        }

        public override void Update()
        {
        }
    }
}
