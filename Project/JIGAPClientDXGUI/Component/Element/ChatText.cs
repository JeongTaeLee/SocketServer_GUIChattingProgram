using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class ChatText : Component
    {
        private Text chatText;

        public override void Init()
        {
            base.Init();
            chatText = gameObject.AddComponent<Text>();
        }

        public void SetChatText(bool inMy, string inId, string inName, string inMessage)
        {
            if (inMy)
            {
                string text = $"{inName}({inId}) : {inMessage}";
                chatText.text.Append(text);
            }
            else
            {
                string text = $"{inMessage} : ({inId}){inName}";
                chatText.text.Append(text);
            }
        }
    }
}
