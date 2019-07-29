using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class SingUpLauncher : NetworkComponent
    {
        public override void OnSingUpFailed()
        {
            base.OnSingUpFailed();
            System.Windows.Forms.MessageBox.Show("회원 가입 실패!", "실패", System.Windows.Forms.MessageBoxButtons.OK);
        }

        public override void OnSingUpScuccess()
        {
            System.Windows.Forms.MessageBox.Show("회원 가입 성공!", "실패", System.Windows.Forms.MessageBoxButtons.OK);
            base.OnSingUpScuccess();
        }
    }
}
