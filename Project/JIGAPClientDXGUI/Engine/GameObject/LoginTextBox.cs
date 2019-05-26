using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace JIGAPClientDXGUI.Engine
{
    class LoginTextBox : GameObject
    {
        texture tex = null;

        public override void Init()
        {
            tex = ImageManager.GetInst().LoadTexture("LoginTexBox", "./Image/LoginTextBox.png");

            transform.position = new Vector3(((float)DXManager.GetInst().Width / 2f - (float)tex.d3dInfo.Width / 2f),
                ((float)DXManager.GetInst().Height / 2f - (float)tex.d3dInfo.Height / 2f), 0f);
        }
        public override void Update()
        {
        }
        public override void Render()
        {
            DXManager.GetInst().Draw(tex);
        }


    }
}
