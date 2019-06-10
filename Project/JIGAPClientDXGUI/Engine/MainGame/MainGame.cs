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
            DXManager.GetInst().Initialize("JIGAPChattingGame", 1280, 720);

            if (!NetworkManager.GetInst().StartClient())
            {
                MessageBox.Show("서버에 연결 할 수 없습니다.");
                    return false;
            }

            SceneManager.GetInst().AddScene("LoginScene", new LoginScene());
            SceneManager.GetInst().AddScene("LobbyScene", new LobbyScene());
            SceneManager.GetInst().AddScene("ChattingScene", new ChattingScene());
            SceneManager.GetInst().ChanageScene("LoginScene");

            return true;
        }
        public void Update()
        {
            SceneManager.GetInst().Update();
        }

        public void Run()
        {
            RenderLoop.Run(DXManager.GetInst().renderForm, delegate ()
            {
                Update();

                DXManager.GetInst().d3dDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, new Color(172, 103, 32, 255), 1f, 0);
                DXManager.GetInst().d3dDevice.BeginScene();
                DXManager.GetInst().d3dSprite.Begin(SpriteFlags.AlphaBlend);

                SceneManager.GetInst().Render();

                DXManager.GetInst().d3dSprite.End();
                DXManager.GetInst().d3dDevice.EndScene();
                DXManager.GetInst().d3dDevice.Present();
            });
        }

        public void Dispose()
        {
            ObjectManager.GetInst().Dispose();
            ImageManager.GetInst().Dispose();
            SceneManager.GetInst().Dispose();
            InputManager.GetInst().Dispose();
            DXManager.GetInst()?.Dispose();
        }
    }
}
