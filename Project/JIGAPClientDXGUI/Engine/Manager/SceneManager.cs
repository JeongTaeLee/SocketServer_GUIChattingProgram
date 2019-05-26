using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI.Engine
{
    class SceneManager : IDisposable
    {
        private static SceneManager Instance = null;
        public static SceneManager GetInst()
        {
            if (Instance == null)
                Instance = new SceneManager();

            return Instance;
        }

        Dictionary<string, Scene> scenes = new Dictionary<string, Scene>();

        private Scene nowScene = null;
        private Scene nextScene = null;

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
            if (nowScene != null)
                nowScene.Update();
        }
        public void Render()
        {
            if (nowScene != null)
                nowScene.Render();
        }

        public void Dispose()
        {
            nowScene = null;
            nextScene = null;
            scenes.Clear();

            GC.SuppressFinalize(true);
        }

    }
}
