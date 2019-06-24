using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;

using SharpDX.DirectInput;


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

        public Point MousePoint
        {
            get
            {
                Point point = new Point(Cursor.Position.X, Cursor.Position.Y);

                Point ToClient = new Point();
                ToClient = DXManager.Instance.renderForm.PointToClient(point);
                return ToClient;
            }
        }
        public event EventHandler LeftMouseDown;
        public event EventHandler LeftMouseUp;
        public event EventHandler RightMouseDown;
        public event EventHandler RightMouseUp;

        public enum MouseButtonType
        {
            Left = 0,
            Right = 1,
            Wheel = 2,
        }

        // DirectInput
        private DirectInput directInput     = null;
        private Keyboard keyboard           = null;
        private Mouse mouse                 = null;

        // Buffer
        bool[] currentKeyBuffer = new bool[(int)Key.MediaSelect + 1];
        bool[] oldKeyBuffer = new bool[(int)Key.MediaSelect + 1];

        bool[] oldMouseBuffer = null;
        bool[] currentMouseBuffer = null;
    }


    partial class InputManager : IDisposable
    {
        public InputManager()
        {
            DXManager.Instance.RegisterWndProc(WndProc);

            directInput = new DirectInput();
            keyboard = new Keyboard(directInput);
            mouse = new Mouse(directInput);

            keyboard.Properties.BufferSize = 0;
            mouse.Properties.BufferSize = 0;

            keyboard.Acquire();
            mouse.Acquire();

            var ms = mouse.GetCurrentState();
            currentMouseBuffer = new bool[ms.Buttons.Length];
            oldMouseBuffer = new bool[ms.Buttons.Length];
        }

        public void Dispose()
        {
            keyboard.Dispose();
            mouse.Dispose();
            directInput.Dispose();
            DXManager.Instance.UnRegisterwndProc(WndProc);
            GC.SuppressFinalize(this);
        }

        public void Update()
        {
            Array.Copy(currentKeyBuffer, 0, oldKeyBuffer, 0, currentKeyBuffer.Length);
            Array.Clear(currentKeyBuffer, 0, currentKeyBuffer.Length);

            var kb = keyboard.GetCurrentState();
            foreach (Key i in Enum.GetValues(typeof(Key)))
            {
                if (kb.IsPressed(i))
                    currentKeyBuffer[(int)i] = true;
            }

            var ms = mouse.GetCurrentState();
            Array.Copy(currentMouseBuffer, oldMouseBuffer, currentMouseBuffer.Length);
            Array.Copy(ms.Buttons, currentMouseBuffer, ms.Buttons.Length);
        }

        public bool GetKeyDown(Key key)
        {
            return (currentKeyBuffer[(int)key] && !oldKeyBuffer[(int)key]);
        }

        public bool GetKeyUp(Key key)
        {
            return (!currentKeyBuffer[(int)key] && oldKeyBuffer[(int)key]);
        }

        public bool GetKey(Key key)
        {
            return (currentKeyBuffer[(int)key] && oldKeyBuffer[(int)key]);
        }

        public bool GetMouseDown(MouseButtonType type)
        {
            return (currentMouseBuffer[(int)type] && !oldMouseBuffer[(int)type]);
        }

        public bool GetMouseUp(MouseButtonType type)
        {
            return (!currentMouseBuffer[(int)type] && oldMouseBuffer[(int)type]);
        }

        public bool GetMouse(MouseButtonType type)
        {
            return (currentMouseBuffer[(int)type] && oldMouseBuffer[(int)type]);
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
