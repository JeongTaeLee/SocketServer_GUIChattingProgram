using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Direct3D9;
using SharpDX.Mathematics.Interop;
using SharpDX.Windows;

namespace JIGAPClientDXGUI.Engine
{
    delegate void RunFunc();

    class DXManager : IDisposable
    {
        private static DXManager Instance = null;
        public static DXManager GetInst()
        {
            if (Instance == null)
                Instance = new DXManager();

            return Instance;
        }

        public RenderMsgForm renderForm { get; private set; } = null;
        public Direct3D d3dObject { get; private set; } = null;
        public Device d3dDevice { get; private set; } = null;
        public Sprite d3dSprite { get; private set; } = null;

        public int Width { get => renderForm.ClientSize.Width; }
        public int Height { get => renderForm.ClientSize.Height; }

        public bool Initialize(string name, int width, int height)
        {
            renderForm = new RenderMsgForm(name);
            renderForm.ClientSize = new System.Drawing.Size(width, height);
 
            d3dObject = new Direct3D();
            if (d3dObject == null)
                return false;

            d3dDevice = new Device(d3dObject, 0, DeviceType.Hardware, renderForm.Handle, CreateFlags.HardwareVertexProcessing,
                new PresentParameters(renderForm.ClientSize.Width, renderForm.ClientSize.Height));

            if (d3dDevice == null)
                return false;

            d3dSprite = new Sprite(d3dDevice);

            return true;
        }

        public void Dispose()
        {
            d3dSprite?.Dispose();
            d3dDevice?.Dispose();
            d3dObject?.Dispose();

            GC.SuppressFinalize(true);
        }

        public void RegisterWndProc(EventHandler<Message> eventHandler)
        {
            renderForm.OnMessage += eventHandler;
        }
        public void UnRegisterwndProc(EventHandler<Message> eventHandler)
        {
            renderForm.OnMessage -= eventHandler;
        }

    }
}
