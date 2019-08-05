using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

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

        private Mutex objectMutex = new Mutex(false, "objectMutex");
        private Mutex rendererMutex = new Mutex(false, "rendererMutex");
        private Mutex uiRendererMutex = new Mutex(false, "uiRendererMutex");
    }

    partial class ObjectManager : IDisposable
    {
        public GameObject RegisterObject()
        {
            GameObject gameObject = new GameObject();

            objectMutex.WaitOne();
            Objects.AddLast(gameObject);
            objectMutex.ReleaseMutex();

            gameObject.Init();

            return gameObject;
        }
        public void UnRegisterObject(GameObject gameObject)
        {
            gameObject.Release();

            objectMutex.WaitOne();
            Objects.Remove(gameObject);
            objectMutex.ReleaseMutex();
        }

        public void RegisterSpriteRenderer(IRenderer spriteRenderer)
        {
            rendererMutex.WaitOne();
            SpriteRenderers.AddLast(spriteRenderer);
            rendererMutex.ReleaseMutex();
        }
        public void RegisterUIRenderer(IRenderer inUIRenderer)
        {
            uiRendererMutex.WaitOne();
            UIRenderers.AddLast(inUIRenderer);
            uiRendererMutex.ReleaseMutex();
        }

        public void UnRegisterSpriteRenderer(IRenderer inSpriteRenderer)
        {
            rendererMutex.WaitOne();
            SpriteRenderers.Remove(inSpriteRenderer);
            rendererMutex.ReleaseMutex();
        }
        public void UnRegisterUIRenderer(IRenderer inUIRenderer)
        {
            uiRendererMutex.WaitOne();
            UIRenderers.Remove(inUIRenderer);
            uiRendererMutex.ReleaseMutex();
        }

        public void ClearObjects()
        {
            objectMutex.WaitOne();
            foreach (GameObject obj in Objects)
                obj.Release();
            Objects.Clear();
            objectMutex.ReleaseMutex();

        }

        public void Update()
        {
            objectMutex.WaitOne();
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
            objectMutex.ReleaseMutex();
        }

        public void Render()
        {
            DXManager.Instance.d3dSprite.Begin(SharpDX.Direct3D9.SpriteFlags.AlphaBlend | SharpDX.Direct3D9.SpriteFlags.ObjectSpace);

            rendererMutex.WaitOne();
            foreach(IRenderer obj in SpriteRenderers)
            {
                Component com = obj as Component;

                if (com.Enable && com.gameObject.Active)
                {
                    DXManager.Instance.d3dSprite.Transform = com.gameObject.transform.matWorld;
                    obj.Render();
                }
            }
            rendererMutex.ReleaseMutex();

            DXManager.Instance.d3dSprite.End();

            DXManager.Instance.d3dSprite.Begin(SharpDX.Direct3D9.SpriteFlags.AlphaBlend);

            uiRendererMutex.WaitOne();
            foreach (IRenderer obj in UIRenderers)
            {
                Component com = obj as Component;

                if (com.Enable && com.gameObject.Active)
                {
                    DXManager.Instance.d3dSprite.Transform = com.gameObject.transform.matWorld;
                    obj.Render();
                }
            }
            uiRendererMutex.ReleaseMutex();

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
