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
            System.Windows.Forms.MessageBox.Show($"{roomName} 방에 입장하셨습니다.", "Test");
        }

        public override void OnJoinRoomFailed()
        {
            base.OnJoinRoomFailed();
            System.Windows.Forms.MessageBox.Show($"방 입장 실패.", "Test");
        }
    }
}
