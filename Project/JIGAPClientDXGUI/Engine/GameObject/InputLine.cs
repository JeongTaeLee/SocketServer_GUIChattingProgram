using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI.Engine
{
    abstract class InputLine : GameObject
    {
        private StringBuilder text = new StringBuilder();

        public int fontSize { get; set; } = 11;
        public Color fontColor { get; set; } = Color.White;
        public int maxTextLength { get; set; } = 11;

        public override void Init()
        {
            DXManager.GetInst().RegisterWndProc(WndProc);
        }
        public override void Update()
        {
            
        }
        public override void Render()
        {
            if (text.Length == 0)
                return;

            Font font = new Font(DXManager.GetInst().d3dDevice, fontSize, 0, FontWeight.Bold, 0, false, FontCharacterSet.Hangul, FontPrecision.Default
                , FontQuality.Default, FontPitchAndFamily.Default, "돋움");

            font.DrawText(DXManager.GetInst().d3dSprite, text.ToString(), 0, 0, fontColor);

            font.Dispose();
        }
        protected void WndProc(object sender, Message message)
        {
            switch (message.Msg)
            {
                case MsgType.WM_CHAR:
                    AddText((char)message.WParam);
                    break;
            }

        }

        public void AddText(char ch)
        {
            switch (ch)
            {
                case (char)13: // ENTER
                    InputEnter();
                    break;

                case (char)8: // BackSpace
                    if (text.Length > 0)
                        text.Remove(text.Length - 1, 1);
                    break;
                default:
                    if (text.Length < maxTextLength)
                    text.Append(ch);
                    break;
            }
        }

        public new void Dispose()
        {
            DXManager.GetInst().UnRegisterwndProc(WndProc);
            base.Dispose();
        }

        public abstract void InputEnter();
        
        
    }
}
