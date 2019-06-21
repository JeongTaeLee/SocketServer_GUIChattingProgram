using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI.Engine
{
    partial class SceneManager : IDisposable
    {
        private static SceneManager instance = null;
        public static SceneManager Instance
        {
            get
            {
                if (instance == null)
                    instance = new SceneManager();

                return instance;
            }
        }

        Dictionary<string, Scene> scenes = new Dictionary<string, Scene>();

        private Scene nowScene = null;
        private Scene nextScene = null;
    }


    partial class SceneManager : IDisposable
    {
        public Scene AddScene(string inName, Scene inScene)
        {
            Scene scene = null;

            if (!scenes.TryGetValue(inName, out scene))
            {
                scenes.Add(inName, inScene);
                return inScene;
            }
            return null;
        }

        public Scene ChanageScene(string inName)
        {
            Scene scene = null;

            if (scenes.TryGetValue(inName, out scene))
            {
                nextScene = scene;
                return nextScene;
            }

            return null;
        }

        public void Update()
        {
            if (nextScene != null)
            {
                if (nowScene != null)
                    nowScene.Release();
                nowScene = nextScene;
                nextScene = null;
                nowScene.Init();
            }
        }

        public void Dispose()
        {
            if(nowScene != null)
                nowScene.Release();
            nowScene = null;
            nextScene = null;

            scenes.Clear();

            GC.SuppressFinalize(true);
        }

    }
}
