using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class StandardInputField : InputField
    {
        public override void Init()
        {
            base.Init();

            TextBoxTexture = ImageManager.GetInst().LoadTexture("RoomNameTextBox", "./Image/RoomNameTextBox.png");
            TextBoxInputLine.FontSize = 50;
            TextBoxInputLine.MaxTextLength = 18;

            transform.position = new Vector3(358f, 620f, 0f);
        }
    }
}
