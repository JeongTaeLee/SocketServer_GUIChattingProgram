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

    partial class InputManager : IDisposable
    {
        private static InputManager instance = null;
        public static InputManager Instance
        {
            get
            {
                if (instance == null)
                    instance = new InputManager();

                return instance;
            }
        }
        

        public Point MousePoint { get => new Point(Cursor.Position.X, Cursor.Position.Y); }
        public event EventHandler LeftMouseDown;
        public event EventHandler LeftMouseUp;
        public event EventHandler RightMouseDown;
        public event EventHandler RightMouseUp;
    }


    partial class InputManager : IDisposable
    {
        public InputManager()
        {
            DXManager.Instance.RegisterWndProc(WndProc);
        }

        public void Dispose()
        {
            DXManager.Instance.UnRegisterwndProc(WndProc);
            
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
