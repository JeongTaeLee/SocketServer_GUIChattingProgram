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
            NetworkManager.GetInst().OnLogin += OnLoginSuccess;
            NetworkManager.GetInst().OnLoginFailed += OnLoginFailed;
            NetworkManager.GetInst().OnCreateRoom += OnCreateRoom;
            NetworkManager.GetInst().OnCreateRoomFailed += OnCreateRoomFailed;
            NetworkManager.GetInst().OnJoinedRoom += OnJoinedRoom;
            NetworkManager.GetInst().OnJoinedRoomFailed += OnJoinedRoomFailed;
            NetworkManager.GetInst().OnExitRoom += OnExitRoom;
        }

        public override void Render()
        {
        }

        public override void Update()
        {
        }

        public new void Dispose()
        {
            NetworkManager.GetInst().OnLogin -= OnLoginSuccess;
            NetworkManager.GetInst().OnLoginFailed -= OnLoginFailed;
            NetworkManager.GetInst().OnCreateRoom -= OnCreateRoom;
            NetworkManager.GetInst().OnCreateRoomFailed += OnCreateRoomFailed;
            NetworkManager.GetInst().OnJoinedRoom -= OnJoinedRoom;
            NetworkManager.GetInst().OnJoinedRoomFailed -= OnJoinedRoomFailed;
            NetworkManager.GetInst().OnExitRoom -= OnExitRoom;
            base.Dispose();
        }
        
        public virtual void OnLoginSuccess(object sender, EventArgs e)
        {
        }
        public virtual void OnLoginFailed(object sender, EventArgs e)
        {
        }
        public virtual void OnCreateRoom(object send, EventArgs e)
        {
        }
        public virtual void OnCreateRoomFailed(object send, EventArgs e)
        {
        }

        public virtual void OnJoinedRoom(object sender, EventArgs e)
        {
        }
        public virtual void OnJoinedRoomFailed(object sender, EventArgs e)
        {
        }
        public virtual void OnExitRoom(object sender, EventArgs e)
        {
        }
    }
}
