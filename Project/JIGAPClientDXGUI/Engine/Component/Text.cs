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
        private StringBuilder _stringBuilder = new StringBuilder();

        public string str { get => _stringBuilder.ToString();}

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

        public void SetString(string inText)
        {
            _stringBuilder.Clear();
            _stringBuilder.Append(inText);
        }
        public void SetAddString(char c)
        {
            _stringBuilder.Append(c);
        }   
        public void DeleteCharInString()
        {
            _stringBuilder.Remove(_stringBuilder.Length - 1, 1);
        }

        public void Render()
        {
            if (_stringBuilder.Length > 0)
                font.DrawText(DXManager.Instance.d3dSprite, _stringBuilder.ToString(), _range, _drawFlag, _color);
        }
    }
}
