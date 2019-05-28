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
            TextBoxInputLine.MaxTextLength = 18;

            transform.position = new Vector3(((float)DXManager.GetInst().Width / 2f - (float)TextBoxTexture.d3dInfo.Width / 2f),
                ((float)DXManager.GetInst().Height / 2f - (float)TextBoxTexture.d3dInfo.Height / 2f), 0f);
        }

    }
}
