using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    public class LoginScene : Scene
    {

        public override void Init()
        {
            BackGround back = ObjectManager.GetInst().AddObject<BackGround>();
            back.BGTexture = ImageManager.GetInst().LoadTexture("LoginBackGround", "./Image/LoginBackGround.png");

            ObjectManager.GetInst().AddObject<LoginLauncher>();
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
