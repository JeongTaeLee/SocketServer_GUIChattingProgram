using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Mathematics;


namespace JIGAPClientDXGUI
{

    partial class ObjectManager : IDisposable
    {
        private static ObjectManager instance = null;
        public static ObjectManager Instance
        {
            get
            {
                if (instance == null)
                    instance = new ObjectManager();

                return instance;
            }  
        }

        private LinkedList<GameObject> Objects = new LinkedList<GameObject>();
        private LinkedList<IRenderer> SpriteRenderers = new LinkedList<IRenderer>();
        private LinkedList<IRenderer> UIRenderers = new LinkedList<IRenderer>();
    }

    partial class ObjectManager : IDisposable
    {
        public GameObject RegisterObject()
        {
            GameObject gameObject = new GameObject();
            Objects.AddLast(gameObject);
            gameObject.Init();

            return gameObject;
        }
        public void UnRegisterObject(GameObject gameObject)
        {
            gameObject.Release();
            Objects.Remove(gameObject);
        }

        public void RegisterSpriteRenderer(IRenderer spriteRenderer)
        {
            SpriteRenderers.AddLast(spriteRenderer);
        }
        public void RegisterUIRenderer(IRenderer inUIRenderer)
        {
            UIRenderers.AddLast(inUIRenderer);
        }

        public void UnRegisterSpriteRenderer(IRenderer inSpriteRenderer)
        {
            SpriteRenderers.Remove(inSpriteRenderer);
        }
        public void UnRegisterUIRenderer(IRenderer inUIRenderer)
        {
            UIRenderers.Remove(inUIRenderer);
        }

        public void ClearObjects()
        {
            foreach (GameObject obj in Objects)
                obj.Release();

            Objects.Clear();
        }

        public void Update()
        {
            for (LinkedListNode<GameObject> node = Objects.First; node != null; )
            {
                if (node.Value.Destroy)
                {
                    node.Value.Release();
                    node = node.Next;
                    Objects.Remove(node);
                }
                else
                {
                    if (node.Value.Active)
                    {
                        node.Value.ComponentUpdate();
                        node.Value.transform.TransformUpdate();
                    }
                    node = node.Next;
                }
            }
        }

        public void Render()
        {
            DXManager.Instance.d3dSprite.Begin(SharpDX.Direct3D9.SpriteFlags.AlphaBlend | SharpDX.Direct3D9.SpriteFlags.ObjectSpace);

            foreach(IRenderer obj in SpriteRenderers)
            {
                Component com = obj as Component;

                if (com.Enable && com.gameObject.Active)
                {
                    DXManager.Instance.d3dSprite.Transform = com.gameObject.transform.matWorld;
                    obj.Render();
                }
            }

            DXManager.Instance.d3dSprite.End();

            DXManager.Instance.d3dSprite.Begin(SharpDX.Direct3D9.SpriteFlags.AlphaBlend);

            foreach (IRenderer obj in UIRenderers)
            {
                Component com = obj as Component;

                if (com.Enable && com.gameObject.Active)
                {
                    DXManager.Instance.d3dSprite.Transform = com.gameObject.transform.matWorld;
                    obj.Render();
                }
            }

            DXManager.Instance.d3dSprite.End();

        }

        public void Dispose()
        {
            foreach (GameObject obj in Objects)
                obj.Release();
            Objects.Clear();

            

            GC.SuppressFinalize(this);
        }

    }
}
