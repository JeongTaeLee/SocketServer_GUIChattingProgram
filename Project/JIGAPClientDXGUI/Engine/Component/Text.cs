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
    class Text : Component, IRenderer
    {
        public StringBuilder text { get; set; } = new StringBuilder();
        public int FontSize { get; set; } = 50;
        public FontWeight FontWeight { get; set; } = FontWeight.Bold;
        public FontDrawFlags DrawFlag { get; set; } = FontDrawFlags.Left | FontDrawFlags.Top;
        public Rectangle Range { get; set; } = new Rectangle();
        public Color Color { get; set; } = Color.Black;
        public Font Font { get; set; } = ResourceManager.Instance.LoadFont("Default");

        public override void Init()
        {
            ObjectManager.Instance.RegisterUIRenderer(this);
        }
        public override void Release()
        {
            ObjectManager.Instance.UnRegisterUIRenderer(this);
        }

        public void Render()
        {
            if (text.Length > 0)
                Font.DrawText(DXManager.Instance.d3dSprite, text.ToString(), Range, DrawFlag, Color);
        }
    }
}
