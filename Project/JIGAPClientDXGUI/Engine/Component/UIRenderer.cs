using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using System.Runtime.InteropServices;
using SharpDX.Mathematics.Interop;

namespace JIGAPClientDXGUI
{
   
    class UIRenderer : Component, IRenderer
    {
        private texture texture         = null;
        private RawRectangle srcRect    = new RawRectangle(0, 0, 0, 0);
        private Color color             = Color.White;
        private TexturePivot pivot = TexturePivot.LeftTop;
        
        public texture Texture
        {
            get
            {
                return texture;
            }
            set
            {
                texture = value;
                srcRect = new RawRectangle(0, 0, texture.d3dInfo.Width, texture.d3dInfo.Height);
            }

        }
        public Color Color { get => color; set => color = value; }
        public RawRectangle SrcRect { get => srcRect; set => srcRect = value; }
        public TexturePivot Pivot { get => pivot; set => pivot = value; }

        public override void Init()
        {
            ObjectManager.Instance.RegisterUIRenderer(this);
        }
        public override void Release()
        {
            ObjectManager.Instance.UnRegisterUIRenderer(this);
        }

        public virtual void Render()
        {
            if (texture == null)
                return;

            Vector3 vPivot = new Vector3() ;

            switch (pivot)
            {
                case TexturePivot.LeftTop:
                    vPivot = new Vector3(0, 0, 0f);
                    break;
                case TexturePivot.Center:
                    vPivot = new Vector3(texture.d3dInfo.Width / 2f, texture.d3dInfo.Height / 2f, 0f);
                    break;
            }


            DXManager.Instance.d3dSprite.Draw(Texture.d3dTex, Color, SrcRect, vPivot);
        }
    }
}
