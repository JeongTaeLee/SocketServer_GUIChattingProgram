using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class IngameChattingPanel : NetworkComponent
    {
        TextField ChattingTextField = null;
        
        public override void Init()
        {
            base.Init();

            ChattingTextField = ObjectManager.Instance.RegisterObject().AddComponent<TextField>();
            ChattingTextField.transform.Parent = transform;
            ChattingTextField.transform.position = new SharpDX.Vector3(10f, 672f, 0f);
            ChattingTextField.Texture = ResourceManager.Instance.LoadTexture("ChattingTextBox");
            ChattingTextField.FontSize = 35;

            Button button = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            button.transform.Parent = transform;
            button.transform.position = new SharpDX.Vector3(916f, 672f, 0f);
            button.SetButton(ResourceManager.Instance.LoadTexture("SendButton"), null);

        }
    }
}
