using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace JIGAPClientDXGUI.Engine
{
    public class GameObject : IDisposable
    {
        public Transform transform { get; private set; } = null;
        public bool Active { get; set; } = true;

        public GameObject()
        {
            transform = new Transform();
        }
        public virtual void Init() { }
        public virtual void Update() { }
        public virtual void Render() { }

        public void Dispose()
        {
            ObjectManager.GetInst().DeleteObject(this);
            GC.SuppressFinalize(true);
        }
    }
}
