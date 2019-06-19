using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class ConnectScene : Scene
    {
        public override void Init()
        {
            BackGround backGround = ObjectManager.GetInst().AddObject<BackGround>();
            backGround.BGTexture = ImageManager.GetInst().LoadTexture("NConnectBackGround", " ");

        }

        public override void Release()
        { 
        }

        public override void Update()
        {
        }

        public override void Render()
        {
        }

    }
}
