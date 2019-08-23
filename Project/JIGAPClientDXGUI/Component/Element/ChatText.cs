using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class ChatText : Component
    {
        private Text _chatText;

        private bool _mine;
        public bool mine { get => _mine; }

        public override void Init()
        {
            base.Init();
            _chatText = gameObject.AddComponent<Text>();
        }

        public void Initialize(bool inMy, int inTextRangeW, int inTextRangeH, string inId, string inName, string inMessage)
        {
            _mine = inMy;
            _chatText.font = ResourceManager.Instance.LoadFont("Chatting");
            _chatText.range = new SharpDX.Rectangle(0, 0, inTextRangeW, inTextRangeH);

            string text;

            if (mine)
            {
                text = $"{inName}({inId}) : {inMessage}";
                _chatText.drawFlag = SharpDX.Direct3D9.FontDrawFlags.Left;
    
            }
            else
            {
                text = $"{inMessage} : ({inId}){inName}";
                _chatText.drawFlag = SharpDX.Direct3D9.FontDrawFlags.Right;
            }

             _chatText.SetString(text);
        }
    }
}
