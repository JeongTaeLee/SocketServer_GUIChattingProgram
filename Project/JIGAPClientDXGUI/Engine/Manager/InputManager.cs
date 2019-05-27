using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;


namespace JIGAPClientDXGUI.Engine
{

    class InputManager : IDisposable
    {
        private static InputManager Instance = null;
        public static InputManager GetInst()
        {
            if (Instance == null)
                Instance = new InputManager();
            return Instance;
        }

        public Point MousePoint{ get => new Point(Cursor.Position.X, Cursor.Position.Y); }
        public event EventHandler LeftMouseDown;
        public event EventHandler LeftMouseUp;
        public event EventHandler RightMouseDown;
        public event EventHandler RightMouseUp;

        public InputManager()
        {
            DXManager.GetInst().RegisterWndProc(WndProc);
        }

        public void Dispose()
        {
            DXManager.GetInst().UnRegisterwndProc(WndProc);
            
        }

        protected void WndProc(object sender, Message message)
        {
            switch (message.Msg)
            {
                case MsgType.WM_LUP:
                    LeftMouseUp?.Invoke(this, default);
                    break;
                case MsgType.WM_LDOWN:
                    LeftMouseDown?.Invoke(this, default);
                    break;
                case MsgType.WM_RUP:
                    RightMouseUp?.Invoke(this, default);
                    break;
                case MsgType.WM_RDOWN:
                    RightMouseDown?.Invoke(this, default);
                    break;
            }

        }
    }
}
