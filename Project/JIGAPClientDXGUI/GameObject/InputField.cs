
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class InputField : GameObject
    {
        protected texture TextBoxTexture { get; set; }
        public InputLine TextBoxInputLine { get; private set; }
        public InputLine.OnEnter OnEnterCallBack
        {
            get { return TextBoxInputLine.OnEnterCallBack; }
            set { TextBoxInputLine.OnEnterCallBack = value; }
        }
        public override void Init()
        {
            TextBoxInputLine = ObjectManager.GetInst().AddObject<InputLine>();
            TextBoxInputLine.transform.SetParent(this);
            TextBoxInputLine.transform.position = new Vector3(13f, 5f, 0f);
        }

        public override void Release()
        {
            TextBoxInputLine.transform.SetParent(null);
        }

        public override void Render()
        {
            TextBoxTexture.Draw();
        }


    }
}
