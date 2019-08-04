using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D9;
using SharpDX.Mathematics.Interop;
using SharpDX.Windows;
using System.Windows.Forms;

namespace JIGAPClientDXGUI.Engine
{
    class MainGame : IDisposable
    {
        public bool IsInitialize = false;

        public bool Initialize()
        {


            if (!NetworkManager.Instance.ConnectServer())
                return false;

            IsInitialize = true;

            DXManager.Instance.Initialize("JIGAPChattingGame", 1280, 720);

            SceneManager.Instance.AddScene("LoadingScene", new LoadingScene());
            SceneManager.Instance.AddScene("SingUpScene", new SingUpScene());
            SceneManager.Instance.AddScene("LoginScene", new LoginScene());
            SceneManager.Instance.AddScene("IngameScene", new IngameScene());
            SceneManager.Instance.ChanageScene("LoadingScene");

            return true;
        }
        public void Update()
        {
            InputManager.Instance.Update();
            SceneManager.Instance.Update();
            ObjectManager.Instance.Update();
        }

        public void Run()
        {
            RenderLoop.Run(DXManager.Instance.renderForm, delegate ()
            {
                Update();

                DXManager.Instance.d3dDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, new Color(100, 100, 255, 255), 1f, 0);
                DXManager.Instance.d3dDevice.BeginScene();

                ObjectManager.Instance.Render();

                DXManager.Instance.d3dDevice.EndScene();
                DXManager.Instance.d3dDevice.Present();
            });
        }

        public void Dispose()
        {
            NetworkManager.Instance?.Dispose();

            if (IsInitialize)
            {
                ObjectManager.Instance?.Dispose();
                ResourceManager.Instance?.Dispose();
                SceneManager.Instance?.Dispose();
                InputManager.Instance?.Dispose();
                DXManager.Instance?.Dispose();
            }
        }
    }
}
