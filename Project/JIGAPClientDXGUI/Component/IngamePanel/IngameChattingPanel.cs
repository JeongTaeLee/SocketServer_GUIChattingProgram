using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class IngameChattingPanel : NetworkComponent
    {
        private TextField _chattingInputField = null;
        private ChatView _chatView = null;
        
        public override void Init()
        {
            //FirstY 620;
            // 10 Owner 980

            base.Init();

            _chattingInputField = ObjectManager.Instance.RegisterObject().AddComponent<TextField>();
            _chattingInputField.transform.Parent = transform;
            _chattingInputField.transform.position = new SharpDX.Vector3(10f, 672f, 0f);
            _chattingInputField.texture = ResourceManager.Instance.LoadTexture("ChattingTextBox");
            _chattingInputField.enterBehavior = (string str) => { SendChatting(str); };
            _chattingInputField.textComponent.font = ResourceManager.Instance.LoadFont("ChattingInput");

            //_chattingInputField.text.

            Button button = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            button.transform.Parent = transform;
            button.transform.position = new SharpDX.Vector3(916f, 672f, 0f);
            button.SetButton(ResourceManager.Instance.LoadTexture("SendButton"), 
                () => { SendChatting(_chattingInputField.textComponent.ToString()); });

            _chatView = ObjectManager.Instance.RegisterObject().AddComponent<ChatView>();
            _chatView.Initialize(10, 55, 620, 10, 35f, 980, 35);
        }

        private void SendChatting(string inMessage)
        {
            if (inMessage.Length <= 0)
                return;

            NetworkManager.Instance.SendProcess.SendChatRequest(
                NetworkManager.Instance.UserId,
                NetworkManager.Instance.UserName,
                inMessage);

            _chattingInputField.textComponent.SetString("");
        }
    }
}
