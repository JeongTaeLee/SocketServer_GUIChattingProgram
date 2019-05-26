using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace JIGAPClientDXGUI.Engine
{
    public abstract class GameObject : IDisposable
    {
        public Transform transform { get; private set; } = null;

        public GameObject()
        {
            transform = new Transform();
        }
        public abstract void Init();
        public abstract void Update();
        public abstract void Render();

        public void Dispose()
        {
            ObjectManager.GetInst().DeleteObject(this);
            GC.SuppressFinalize(true);
        }
    }
}
