using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace JIGAPClientDXGUI
{
    class ChatView : NetworkComponent
    {
        private LinkedList<ChatText> _chatTexts = new LinkedList<ChatText>();
        private Mutex _chatTextsMutex = new Mutex(false, "chatTextsMutex");
         
        public float textX{ get => _textX; set => _textX = value; }
        private float _textX = 0f;

        public float textMinY { get => _textMinY; set => _textMinY = value; }
        private float _textMinY = 0f;

        public float textMaxY { get => _textMaxY; set => _textMaxY = value; }
        private float _textMaxY = 0f;

        public int chatViewCount { get => _chatViewCount; set => _chatViewCount = value; }
        private int _chatViewCount = 0;

        public float textYDistance {  get => _textYDistance; set => _textYDistance = value;  }
        private float _textYDistance = 0f;

        public int textRangeW { get => _textRangeW; set => _textRangeW = value; }
        private int _textRangeW = 0;

        public int textRangeH { get => _textRangeH; set => _textRangeH = value; }
        private int _textRangeH = 0;

        public void Initialize(float inTextX, float inTextMaxHeight, float inTextMinHeight, int inChatViewCount, float inTextYDistance,
            int inTextRangeW, int inTextRangeH)
        {
            _textX              = inTextX;
            _textMaxY           = inTextMaxHeight;
            _textMinY           = inTextMinHeight;
            _chatViewCount      = inChatViewCount;
            _textYDistance      = inTextYDistance;
            _textRangeW         = inTextRangeW;
            _textRangeH         = inTextRangeH;
        }

        public void RenewChatView()
        {
            _chatTextsMutex.WaitOne();

            float firstY = _textMinY;

            List<ChatText> deleteObjs = new List<ChatText>();

            foreach (var chatText in _chatTexts)
            {
                chatText.transform.position = new SharpDX.Vector3(_textX, firstY, 0f);
                firstY -= _textYDistance;

                if (firstY <= _textMaxY)
                    deleteObjs.Add(chatText);
            }

            _chatTextsMutex.ReleaseMutex();

            foreach(var chatText in deleteObjs)
                chatText.gameObject.Destroy = true;
            

        }
        public override void OnJoinRoomSuccess(string roomName)
        {
            base.OnJoinRoomSuccess(roomName);
      
            base.OnJoinRoomFailed();

            foreach (var chatText in _chatTexts)
                chatText.gameObject.Destroy = true;
            _chatTexts.Clear();
        }

        public override void OnChatArrive(string inId, string inName, string inMessage)
        {
            //FirstY 620;
            // 10 Owner 980

            base.OnChatArrive(inId, inName, inMessage);

            ChatText chatText = ObjectManager.Instance.RegisterObject().AddComponent<ChatText>();
            
            chatText.Initialize((inId == NetworkManager.Instance.UserId), _textRangeW, _textRangeH, inId, inName, inMessage);

            _chatTextsMutex.WaitOne();

            _chatTexts.AddFirst(chatText);

            _chatTextsMutex.ReleaseMutex();

            RenewChatView();
        }

    }
}
