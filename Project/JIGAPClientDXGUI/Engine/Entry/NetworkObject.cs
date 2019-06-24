using System;

using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine.Entry
{
    partial class NetworkObject : GameObject
    {
        public virtual void OnLoginSuccess() { }
        public virtual void OnLoginFailed() { }
    }
}
