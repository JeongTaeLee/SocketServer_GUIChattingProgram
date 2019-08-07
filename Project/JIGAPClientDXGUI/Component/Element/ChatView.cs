using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class ChatView : NetworkComponent
    {
        LinkedList<ChatText> chatTexts = new LinkedList<ChatText>();

        public int iChatViewCount = 10;

        public void RenewChatView()
        {
            int count = 0;

        }
        public override void OnChatArrive(string inId, string inName, string inMessage)
        {
            base.OnChatArrive(inId, inName, inMessage);

            ChatText chatText = ObjectManager.Instance.RegisterObject().AddComponent<ChatText>();
            chatText.SetChatText((inId == NetworkManager.Instance.UserId), inId, inName, inMessage);

            chatTexts.AddFirst(chatText);
        }

    }
}
