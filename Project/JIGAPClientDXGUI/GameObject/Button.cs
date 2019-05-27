using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

using JIGAPClientDXGUI.Engine;



namespace JIGAPClientDXGUI
{
    class Button : GameObject
    {
        public delegate void ButtonEvent();

        public texture ButtonTexture { get; set; } = null;
        public ButtonEvent ButtonEventCallBack { get; set; } = null;
        public SharpDX.Vector3 ButtonPos {
            get { return transform.position; }
            set { transform.position = value; }
        }

        public Rectangle ButtonRange = new Rectangle();

        private float Fade = 0;
        private bool OnButtonRange = false;

        public Button()
        {
            InputManager.GetInst().LeftMouseUp += MouseUp;
        }

        public override void Update()
        {
            Point MousePoint = DXManager.GetInst().renderForm.PointToClient(InputManager.GetInst().MousePoint);
            ButtonRange.X = (int)transform.worldPos.X;
            ButtonRange.Y = (int)transform.worldPos.Y;

            if (ButtonRange.Contains(MousePoint.X, MousePoint.Y))
            {
                Fade = SharpDX.MathUtil.Lerp(Fade, 0.5f, 0.1f);
                OnButtonRange = true;
            }
            else
            {
                Fade = SharpDX.MathUtil.Lerp(Fade, 0, 0.1f);
                OnButtonRange = false;
            }
        }

        public override void Render()
        {
            ButtonTexture?.Draw(new SharpDX.Color(1f - Fade, 1f - Fade, 1f - Fade, 1f));
        }

        public void MouseUp(object sender, EventArgs args)
        {
            if (ButtonEventCallBack != null)
            {
                if (OnButtonRange)
                    ButtonEventCallBack();
            }
        }

        public new void Dispose()
        {
            InputManager.GetInst().LeftMouseUp -= MouseUp;
            base.Dispose();
        }

    }
}
