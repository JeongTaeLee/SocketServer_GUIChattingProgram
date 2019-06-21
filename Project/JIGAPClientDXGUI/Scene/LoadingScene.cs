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
            SceneManager.Instance.ChanageScene("LoginScene");
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
