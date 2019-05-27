using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;


namespace JIGAPClientDXGUI.Engine
{
    class Button : GameObject
    {
        private texture tex = null;
        private Rectangle range = new Rectangle();
        private bool onRange = false;
        private int balck = 0;

        public override void Init()
        {
           
        }
        public override void Update()
        {
            if (range.Contains(InputManager.GetInst().mousePos.X, InputManager.GetInst().mousePos.Y))
            {

            }
        }

        public override void Render()
        {
            DXManager.GetInst().Draw(tex);
        }

        public void MouseDown(object sender, EventArgs args)
        {

        }

        public new void Dispose()
        {
            InputManager.GetInst().LeftMouseDown += MouseDown;
            base.Dispose();
        }

    }
}
