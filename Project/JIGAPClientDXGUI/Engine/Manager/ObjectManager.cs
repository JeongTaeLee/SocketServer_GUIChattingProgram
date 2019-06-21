using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Mathematics;


namespace JIGAPClientDXGUI.Engine
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

        private List<GameObject> objects = new List<GameObject>();
    }

    partial class ObjectManager : IDisposable
    {
        public void ClearObjects()
        {
        }

        public void Update()
        {

        }

        public void Render()
        {

        }

        public void Dispose()
        {

            GC.SuppressFinalize(true);
        }

    }
}
