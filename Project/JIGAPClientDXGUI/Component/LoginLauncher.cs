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
                    System.Windows.Forms.MessageBox.Show("아이디를 잘못 입력하였습니다.", "로그인");
                    break;
                case JIGAPPacket.LoginFailedReason.EDontMatchPw:
                    System.Windows.Forms.MessageBox.Show("패스워드를 잘못 입력하였습니다.", "로그인");
                    break;
                case JIGAPPacket.LoginFailedReason.EOverlapConnect:
                    System.Windows.Forms.MessageBox.Show("이미 접속되어 있는 아이디입니다.", "로그인");
                    break;
            }
        }
    }
}
