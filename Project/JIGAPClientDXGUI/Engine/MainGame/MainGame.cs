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
        public bool Initialize()
        {
            DXManager.Instance.Initialize("JIGAPChattingGame", 1280, 720);

            SceneManager.Instance.AddScene("LoadingScene", new LoadingScene());
            SceneManager.Instance.AddScene("LoginScene", new LoginScene());
            SceneManager.Instance.AddScene("ConnectScene", new ConnectScene());
            SceneManager.Instance.AddScene("ChattingScene", new ChattingScene());
            SceneManager.Instance.ChanageScene("LoadingScene");

            return true;
        }
        public void Update()
        {
            SceneManager.Instance.Update();
        }

        public void Run()
        {
            RenderLoop.Run(DXManager.Instance.renderForm, delegate ()
            {
                Update();

                DXManager.Instance.d3dDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, new Color(172, 103, 32, 255), 1f, 0);
                DXManager.Instance.d3dDevice.BeginScene();
                DXManager.Instance.d3dSprite.Begin(SpriteFlags.AlphaBlend);

                SceneManager.Instance.Render();

                DXManager.Instance.d3dSprite.End();
                DXManager.Instance.d3dDevice.EndScene();
                DXManager.Instance.d3dDevice.Present();
            });
        }

        public void Dispose()
        {
            ObjectManager.Instance.Dispose();
            ImageManager.Instance.Dispose();
            SceneManager.Instance.Dispose();
            InputManager.Instance.Dispose();
            DXManager.Instance?.Dispose();
        }
    }
}
