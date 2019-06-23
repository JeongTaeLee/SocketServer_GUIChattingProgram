using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using System.Runtime.InteropServices;
using SharpDX.Mathematics.Interop;

namespace JIGAPClientDXGUI.Engine
{
    partial class SpriteRenderer : Component
    {
        private texture texture;
        public texture Texture
        {
            get
            {
                return texture;
            }
            set
            {
                texture = value;
                SrcRect = new RawRectangle(0, 0, texture.d3dInfo.Width, texture.d3dInfo.Height);
            }

        }
        public Color Color { get; set; } = Color.White;
        public RawRectangle SrcRect { get; set; } = new RawRectangle(0, 0, 0, 0);
        public RawVector3 CenterPos { get; set; } = new RawVector3(0f, 0f, 0f);
    }


    partial class SpriteRenderer : Component
    {
        public override void Init()
        {
            ObjectManager.Instance.RegisterSpriteRenderer(this);
        }
        public override void Release()
        {
            ObjectManager.Instance.UnRegisterSpriteRenderer(this);
        }

        public override void Render()
        {
            DXManager.Instance.d3dSprite.Draw(Texture.d3dTex, Color, SrcRect, CenterPos);
        }

        public void SetTextureCenter()
        {
            CenterPos = new RawVector3((float)Texture?.d3dInfo.Width / 2f, (float)Texture?.d3dInfo.Height / 2f, 0f);
        }
    }
}
