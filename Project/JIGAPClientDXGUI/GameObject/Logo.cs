using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class Logo : GameObject
    {
        private texture LogoTexture;

        public override void Init()
        {
            LogoTexture = ImageManager.GetInst().LoadTexture("Logo", "./Image/Logo.png");
            transform.position = new Vector3(((float)DXManager.GetInst().Width / 2f - (float)LogoTexture.d3dInfo.Width / 2f),
                   ((float)DXManager.GetInst().Height / 2f - (float)LogoTexture.d3dInfo.Height / 2f) - 100, 0f);
        }
        public override void Update()
        {
        }
        public override void Render()
        {
            LogoTexture?.Draw();
        }

    }

}
