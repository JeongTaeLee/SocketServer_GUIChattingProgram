
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
        LoginInputLine inputLine = null;
        public override void Init()
        {
            tex = ImageManager.GetInst().LoadTexture("LoginTexBox", "./Image/LoginTextBox.png");

            transform.position = new Vector3(((float)DXManager.GetInst().Width / 2f - (float)tex.d3dInfo.Width / 2f),
                ((float)DXManager.GetInst().Height / 2f - (float)tex.d3dInfo.Height / 2f), 0f);

            LoginInputLine input = ObjectManager.GetInst().AddObject<LoginInputLine>();
          
            input.transform.SetParent(this);
            input.transform.position = new Vector3(10f, 0f, 0f);
            input.maxTextLength = 15;
            input.fontSize = 60;  
        }
        public override void Update()
        {

        }
        public override void Render()
        {
            DXManager.GetInst().Draw(tex);
        }

        public new void Dispose()
        {
            inputLine.transform.SetParent(null);
            inputLine.Dispose();
            base.Dispose();
        }

    }
}
