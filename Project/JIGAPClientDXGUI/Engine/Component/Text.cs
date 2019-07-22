using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D9;
using SharpDX.Mathematics.Interop;

namespace JIGAPClientDXGUI
{
    class Text : UIRenderer
    {
        public StringBuilder text { get; set; } = new StringBuilder();
        public Vector2 Offset { get; set; } = new SharpDX.Vector2();
        public int FontSize { get; set; } = 50;
        public FontWeight FontWeight { get; set; } = FontWeight.Bold;
        public FontDrawFlags DrawFlag { get; set; } = FontDrawFlags.Left | FontDrawFlags.Top;
        public Rectangle Range { get; set; } = new Rectangle();

        public Text()
        {
            Color = Color.Black;
        }

        public override void Render()
        {
            if (text.ToString().Length <= 0)
                return;

            Font font = 
                new Font(DXManager.Instance.d3dDevice,
                FontSize, 0, FontWeight, 0, false, FontCharacterSet.Hangul, FontPrecision.Default, FontQuality.Default,
                FontPitchAndFamily.Default, "맑은 고딕");

            font.DrawText(DXManager.Instance.d3dSprite, text.ToString(), Range, DrawFlag, Color);

            font.Dispose();
        }
    }
}
