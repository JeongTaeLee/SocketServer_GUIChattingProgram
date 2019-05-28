using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class LoginField : InputField
    {
        public override void Init()
        {
            base.Init();

            TextBoxTexture = ImageManager.GetInst().LoadTexture("LoginTextBox", "./Image/LoginTextBox.png");
            TextBoxInputLine.FontSize = 50;
            TextBoxInputLine.MaxTextLength = 5;
            TextBoxInputLine.transform.position = new Vector3(transform.position.X + (float)TextBoxTexture.d3dInfo.Width / 2f,
                5f, 0f);
            TextBoxInputLine.FontFlag = SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.NoClip;

            transform.position = new Vector3(((float)DXManager.GetInst().Width / 2f - (float)TextBoxTexture.d3dInfo.Width / 2f),
                ((float)DXManager.GetInst().Height / 2f - (float)TextBoxTexture.d3dInfo.Height / 2f), 0f);
        }

    }
}
