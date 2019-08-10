using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class LoginLauncher : NetworkComponent
    {
        public override void OnLoginSuccess()
        {
            base.OnLoginSuccess();
            SceneManager.Instance.ChanageScene("IngameScene");
        }
        public override void OnLoginFailed(JIGAPPacket.LoginFailedReason reason)
        {
            base.OnLoginFailed(reason);

            switch (reason)
            {
                case JIGAPPacket.LoginFailedReason.EDontMatchId:
                    MyMessageBox.Show("로그인", "아이디를 잘못 입력하였습니다.");
                    break;
                case JIGAPPacket.LoginFailedReason.EDontMatchPw:
                    MyMessageBox.Show("로그인", "패스워드를 잘못 입력하였습니다.");
                    break;
                case JIGAPPacket.LoginFailedReason.EOverlapConnect:
                    MyMessageBox.Show("로그인", "이미 접속되어 있는 아이디입니다.");
                    break;
            }
        }
    }
}
