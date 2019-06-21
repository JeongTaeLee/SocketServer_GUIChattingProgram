using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine
{
    public partial class Component
    {
        public GameObject gameObject { get; private set; } = null;

        public Transform transform { get; private set; } = null;

        bool Enable { get; set; } = true;

        ~Component()
        {
            gameObject = null;
            transform = null;
        }
    }

    public partial class Component
    {
        public virtual void Init() { }
        public virtual void Release() { }
        public virtual void Update() { }
        public virtual void Render() { }
        
        public void SettingComponent(GameObject inGameObject, Transform inTransform)
        {
            gameObject = inGameObject;
            transform = inTransform;
        }
    }

}
