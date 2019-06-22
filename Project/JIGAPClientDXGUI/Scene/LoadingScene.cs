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
            ImageManager.Instance.LoadTexture("ConnectSceneBackGround", "./Image/ConnectBackGround.png");
            ImageManager.Instance.LoadTexture("ConnectButton", "./Image/ConnectButton.png");

            SceneManager.Instance.ChanageScene("ConnectScene");
        }

        public override void Release()
        {
        }

    }
}
