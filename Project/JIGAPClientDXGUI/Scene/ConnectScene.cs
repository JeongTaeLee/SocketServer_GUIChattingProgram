using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class ConnectScene : Scene
    {
        public override void Init()
        {
            GameObject obj = ObjectManager.Instance.RegisterObject();
            obj.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("ConnectButton"), 100, 100, 223, 58);
            
        }

        public override void Release()
        { 
        }


    }
}
