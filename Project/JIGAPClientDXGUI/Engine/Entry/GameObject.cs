using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace JIGAPClientDXGUI.Engine
{
    partial class GameObject
    {
        private LinkedList<Component> Components = new LinkedList<Component>();

        public Transform transform { get; private set; } = null;
        public bool Active { get; set; } = true;
        public bool Destroy { get; set; } = false;

       
    }

    public partial class GameObject
    {

        public GameObject()
        {
            transform = new Transform();
        }
        public virtual void Init()
        {
            transform = AddComponent<Transform>();
            transform.SettingComponent(this, transform);
        }
        public virtual void Release()
        {
            foreach (Component Com in Components)
                Com.Release();

            Components.Clear();
        }
        public void ComponentUpdate()
        {
            foreach (Component Com in Components)
                Com.Update();
        }
        public void ComponentRender()
        {
            foreach (Component Com in Components)
                Com.Render();
        }

        public T AddComponent<T>()
            where T : Component, new()
        {
            foreach (Component obj in Components)
            {
                if ((obj as T) != null)
                    return null;
            }

            T Com = new T();
            Components.AddLast(Com);

            Com.SettingComponent(this, transform);
            Com.Init();
            return Com;
        }

        public T GetComponent<T>()
            where T : Component
        {
            foreach (Component obj in Components)
            {
                T Com = obj as T;

                if (Com != null)
                    return Com;
            }
            return null;
        }
    }

    
}
