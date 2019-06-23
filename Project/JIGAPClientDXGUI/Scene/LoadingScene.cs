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
            ImageManager.Instance.LoadTexture("LoginBackGround", "./Image/LoginScene/LoginBackGround.png");
            ImageManager.Instance.LoadTexture("LoginTextBox", "./Image/LoginScene/LoginTextBox.png");
            ImageManager.Instance.LoadTexture("LoginButton", "./Image/LoginScene/LoginButton.png");

            SceneManager.Instance.ChanageScene("LoginScene");
        }

        public override void Release()
        {
        }

    }
}
