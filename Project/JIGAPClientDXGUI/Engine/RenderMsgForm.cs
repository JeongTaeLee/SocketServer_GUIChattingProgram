using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Windows;

namespace JIGAPClientDXGUI.Engine
{
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
