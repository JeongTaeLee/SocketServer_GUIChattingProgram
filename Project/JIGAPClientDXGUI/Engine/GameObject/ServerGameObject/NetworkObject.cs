using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine
{
    class NetworkObject : GameObject
    {
        public override void Init()
        {
        }

        public override void Render()
        {
        }

        public override void Update()
        {
        }

        public virtual void OnLoginSuccess()
        {
        }
        public virtual void OnLoginFailed()
        {
        }
        public virtual void OnJoeindRoom()
        {
        }
        public virtual void OnJoinedRoomFailed()
        {
        }
        public virtual void OnExitRoomSuccesss()
        {
        }
        public virtual void OnExitRoomFailed()
        {
        }
    }
}
