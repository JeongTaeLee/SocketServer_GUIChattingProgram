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
                    System.Windows.Forms.MessageBox.Show("이미 가입된 아이디입니다.", "회원가입", System.Windows.Forms.MessageBoxButtons.OK);
                    break;
                case JIGAPPacket.SingUpFailedReason.EDuplicateName:
                    System.Windows.Forms.MessageBox.Show("이미 가입된 이름입니다.", "회원가입", System.Windows.Forms.MessageBoxButtons.OK);
                    break;
            }

        }

        public override void OnSingUpScuccess()
        {
            base.OnSingUpScuccess();
            System.Windows.Forms.MessageBox.Show("회원 가입 성공!", "회원가입", System.Windows.Forms.MessageBoxButtons.OK);
            SceneManager.Instance.ChanageScene("LoginScene");
        }
    }
}
