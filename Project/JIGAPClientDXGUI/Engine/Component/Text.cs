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
        public StringBuilder text { get => _text; set => _text = value; }
        private StringBuilder _text = new StringBuilder();

        public FontDrawFlags drawFlag { get => _drawFlag; set => _drawFlag = value; }
        private FontDrawFlags _drawFlag = FontDrawFlags.Left | FontDrawFlags.Top;

        public Rectangle range { get => _range;  set => _range = value; }
        private Rectangle _range = new Rectangle();

        public Color color { get => _color; set => _color = value; }
        private Color _color = Color.Black;

        public Font font { get => _font; set => _font = value; }
        private Font _font = ResourceManager.Instance.LoadFont("Default");

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
                font.DrawText(DXManager.Instance.d3dSprite, _text.ToString(), _range, _drawFlag, _color);
        }
    }
}
