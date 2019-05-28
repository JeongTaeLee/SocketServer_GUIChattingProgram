using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Mathematics;


namespace JIGAPClientDXGUI.Engine
{
    class ObjectManager : IDisposable
    {
        private static ObjectManager Instance = null;
        public static ObjectManager GetInst()
        {
            if (Instance == null)
                Instance = new ObjectManager();

            return Instance;
        }

        private List<GameObject> objects = new List<GameObject>();

        public T AddObject<T>() where T : GameObject, new()
        {
            T createObject = new T();
            GameObject obj = createObject as GameObject;
            objects.Add(obj);
            obj.Init();
            return createObject;
        }
        public GameObject AddObject(GameObject gameObject)
        {
            objects.Add(gameObject);
            gameObject.Init();
            return gameObject;
        }
        public void DeleteObject(GameObject inObject)
        {
            objects.Remove(inObject);
        }

        public void ClearObjects()
        {
            for(int i = 0; i < objects.Count; ++i)
                objects[i].Release();
        }

        public void Update()
        {
            for (int i = 0; i < objects.Count;)
            {
                if (objects[i].Destroy)
                {
                    objects[i].Release();
                    objects.RemoveAt(i);
                }
                else
                {
                    if(objects[i].Active)
                    { 
                        objects[i].Update();
                        objects[i].transform.TransformUpdate();
                    }
                    ++i;
                }
            }
        }

        public void Render()
        {
            for (int i = 0; i < objects.Count; ++i)
            {
                if (objects[i].Active)
                {
                    DXManager.GetInst().d3dSprite.Transform = objects[i].transform.matWorld;
                    objects[i].Render();
                }
            }
        }

        public void Dispose()
        {
            for (int i = 0; i < objects.Count; ++i)
                objects[i].Release();

            objects.Clear();
            GC.SuppressFinalize(true);
        }

    }
}
