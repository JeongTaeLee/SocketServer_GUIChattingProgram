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
    }
}
