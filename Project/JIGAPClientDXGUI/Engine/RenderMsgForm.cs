using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Windows;

namespace JIGAPClientDXGUI
{
    public class MsgType
    {
        public const int WM_CHAR = 0x0102;
        public const int WM_LDOWN = 0x0201;
        public const int WM_LUP = 0x0202;
        public const int WM_RDOWN = 0x0204;
        public const int WM_RUP = 0x0205;

    }

    public class KeyType
    {
        public const int VK_BACKSPACE = 8;
        public const int VK_SPACE = 32;
    }


    class RenderMsgForm : RenderForm
    {
        public event EventHandler<Message> OnMessage;

        public RenderMsgForm(){}
        public RenderMsgForm(string text) : base(text){}

        protected override void WndProc(ref Message m)
        {
            OnMessage?.Invoke(this, m);
            base.WndProc(ref m);
        }
    }
}
