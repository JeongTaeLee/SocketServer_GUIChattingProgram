using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class ChattingScene : Scene
    {
        public override void Init()
        {
            ObjectManager.GetInst().AddObject<ChattingLauncher>();
        }

        public override void Release()
        {
            ObjectManager.GetInst().ClearObjects();
        }

        public override void Update()
        {
            ObjectManager.GetInst().Update();
        }

        public override void Render()
        {
            ObjectManager.GetInst().Render();
        }

    }
}
