using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine
{ 
    public abstract class Scene
    {

        public abstract void Init();
        public abstract void Release();
        public abstract void Update();
        public abstract void Render();
    }
}
