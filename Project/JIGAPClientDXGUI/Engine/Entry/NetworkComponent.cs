using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    public class NetworkComponent : Component
    {

        public NetworkComponent()
        {
            NetworkManager.Instance.LoginSuccess += OnLoginSuccess;
            NetworkManager.Instance.LoginFailed += OnLoginFailed;
        }

        public override void Release()
        {
            base.Release();

            NetworkManager.Instance.LoginSuccess -= OnLoginSuccess;
            NetworkManager.Instance.LoginFailed -= OnLoginFailed;
        }

        public virtual void OnLoginSuccess()
        {

        }
        public virtual void OnLoginFailed()
        {

        }
    }
}
