using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine
{
    public class LoginScene : Scene
    {

        public override void Init()
        {
            LoginTextBox obj = ObjectManager.GetInst().AddObject<LoginTextBox>();
  
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
