using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Direct3D9;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class InputLine : GameObject
    {
        public delegate void OnEnter();

        public StringBuilder InputText { get; private set; } = new StringBuilder();
        public int FontSize { get; set; } = 11;
        public Color FontColor { get; set; } = Color.White;
        public int MaxTextLength { get; set; } = 11;
        public FontDrawFlags FontFlag { get; set; } = FontDrawFlags.NoClip;
        public OnEnter OnEnterCallBack { get; set; } = null;
        
        public InputLine()
        {
            DXManager.GetInst().RegisterWndProc(WndProc);
        }

        public override void Release()
        {
            base.Release();
            DXManager.GetInst().UnRegisterwndProc(WndProc);
        }
        public override void Render()
        {
            if (InputText.Length == 0)
                return;

            Font font = new Font(DXManager.GetInst().d3dDevice, FontSize, 0, FontWeight.Bold, 0, false, FontCharacterSet.Hangul, FontPrecision.Default
                , FontQuality.Default, FontPitchAndFamily.Default, "맑은 고딕");

            font.DrawText(DXManager.GetInst().d3dSprite, InputText.ToString(), new Rectangle(), FontFlag, FontColor);

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
                    if (InputText.Length > 0)
                        InputText.Remove(InputText.Length - 1, 1);
                    break;
                default:
                    if (InputText.Length < MaxTextLength)
                    InputText.Append(ch);
                    break;
            }
        }

        public void InputEnter()
        {
            if (OnEnterCallBack != null)
                OnEnterCallBack();
        }

        public void SetCenterText()
        {
            FontFlag = SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.NoClip;
        }

    }
}
