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

        private LinkedList<GameObject> _objectsList = new LinkedList<GameObject>();
        private LinkedList<IRenderer> _spriteRenderer = new LinkedList<IRenderer>();
        private LinkedList<IRenderer> _uiRenderer = new LinkedList<IRenderer>();

        private Mutex _objectMutex = new Mutex(false, "objectMutex");
        private Mutex _rendererMutex = new Mutex(false, "rendererMutex");
        private Mutex _uiRendererMutex = new Mutex(false, "uiRendererMutex");
    }

    partial class ObjectManager : IDisposable
    {
        public GameObject RegisterObject()
        {
            GameObject gameObject = new GameObject();

            _objectMutex.WaitOne();
            _objectsList.AddLast(gameObject);
            _objectMutex.ReleaseMutex();

            gameObject.Init();

            return gameObject;
        }
        public void UnRegisterObject(GameObject gameObject)
        {
            gameObject.Release();

            _objectMutex.WaitOne();
            _objectsList.Remove(gameObject);
            _objectMutex.ReleaseMutex();
        }

        public void RegisterSpriteRenderer(IRenderer spriteRenderer)
        {
            _rendererMutex.WaitOne();
            _spriteRenderer.AddLast(spriteRenderer);
            _rendererMutex.ReleaseMutex();
        }
        public void RegisterUIRenderer(IRenderer inUIRenderer)
        {
            _uiRendererMutex.WaitOne();
            _uiRenderer.AddLast(inUIRenderer);
            _uiRendererMutex.ReleaseMutex();
        }

        public void UnRegisterSpriteRenderer(IRenderer inSpriteRenderer)
        {
            _rendererMutex.WaitOne();
            _spriteRenderer.Remove(inSpriteRenderer);
            _rendererMutex.ReleaseMutex();
        }
        public void UnRegisterUIRenderer(IRenderer inUIRenderer)
        {
            _uiRendererMutex.WaitOne();
            _uiRenderer.Remove(inUIRenderer);
            _uiRendererMutex.ReleaseMutex();
        }

        public void ClearObjects()
        {
            _objectMutex.WaitOne();
            foreach (GameObject obj in _objectsList)
                obj.Release();
            _objectsList.Clear();
            _objectMutex.ReleaseMutex();

        }

        public void Update()
        {
            _objectMutex.WaitOne();
            for (LinkedListNode<GameObject> node = _objectsList.First; node != null; )
            {
                if (node.Value.Destroy)
                {
                    node.Value.Release();
                    LinkedListNode<GameObject> oldNode = node;
                    node = node.Next;
                    _objectsList.Remove(oldNode);
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
            _objectMutex.ReleaseMutex();
        }

        public void Render()
        {
            DXManager.Instance.d3dSprite.Begin(SharpDX.Direct3D9.SpriteFlags.AlphaBlend | SharpDX.Direct3D9.SpriteFlags.ObjectSpace);

            _rendererMutex.WaitOne();
            foreach(IRenderer obj in _spriteRenderer)
            {
                Component com = obj as Component;

                if (com.Enable && com.gameObject.Active)
                {
                    DXManager.Instance.d3dSprite.Transform = com.gameObject.transform.matWorld;
                    obj.Render();
                }
            }
            _rendererMutex.ReleaseMutex();

            DXManager.Instance.d3dSprite.End();

            DXManager.Instance.d3dSprite.Begin(SharpDX.Direct3D9.SpriteFlags.AlphaBlend);

            _uiRendererMutex.WaitOne();
            foreach (IRenderer obj in _uiRenderer)
            {
                Component com = obj as Component;

                if (com.Enable && com.gameObject.Active)
                {
                    DXManager.Instance.d3dSprite.Transform = com.gameObject.transform.matWorld;
                    obj.Render();
                }
            }
            _uiRendererMutex.ReleaseMutex();

            DXManager.Instance.d3dSprite.End();

        }

        public void Dispose()
        {
            foreach (GameObject obj in _objectsList)
                obj.Release();
            _objectsList.Clear();

            GC.SuppressFinalize(this);
        }

    }
}
