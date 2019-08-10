using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class SingUpLauncher : NetworkComponent
    {
        public override void OnSingUpFailed(JIGAPPacket.SingUpFailedReason reason)
        {
            base.OnSingUpFailed(reason);

            switch (reason)
            {
                case JIGAPPacket.SingUpFailedReason.EDuplicateId:
                    MyMessageBox.Show("회원가입", "이미 가입된 아이디입니다.");
                    break;
                case JIGAPPacket.SingUpFailedReason.EDuplicateName:
                    MyMessageBox.Show("회원가입", "이미 가입된 이름입니다.");
                    break;
            }

        }

        public override void OnSingUpScuccess()
        {
            base.OnSingUpScuccess();
            MyMessageBox.Show("회원가입", "회원 가입 성공!");
            SceneManager.Instance.ChanageScene("LoginScene");
        }
    }
}
