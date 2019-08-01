using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class ChatLauncher : NetworkComponent
    {
        public Text roomTitle = null;


        public override void Init()
        {
            base.Init();

        }

        public override void OnJoinRoomSuccess(string roomName)
        {
            base.OnJoinRoomSuccess(roomName);
        }

        public override void OnJoinRoomFailed()
        {
            base.OnJoinRoomFailed();
        }
    }
}
