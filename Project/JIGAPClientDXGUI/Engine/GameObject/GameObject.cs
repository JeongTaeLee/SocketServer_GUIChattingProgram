using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace JIGAPClientDXGUI.Engine
{
    public class GameObject
    {
        public Transform transform { get; private set; } = null;
        public bool Active { get; set; } = true;
        public bool Destroy { get; set; } = false;

        public GameObject()
        {
            transform = new Transform();
        }
        public virtual void Init() { }
        public virtual void Release()
        {}
        public virtual void Update() { }
        public virtual void Render() { }

     
    }
}
