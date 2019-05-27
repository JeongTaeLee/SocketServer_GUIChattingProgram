using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine
{
    class NetworkObject : GameObject
    {

        public NetworkObject()
        {
            NetworkManager.GetInst().OnLoginSuccessEvent += OnLoginSuccess;
            NetworkManager.GetInst().OnLoginFailedEvent += OnLoginFailed;
            NetworkManager.GetInst().OnRoomListEvent += OnRoomList;
            NetworkManager.GetInst().OnCreateRoomSuccessEvent += OnCreateRoom;
            NetworkManager.GetInst().OnCreateRoomFailedEvent += OnCreateRoomFailed;
            NetworkManager.GetInst().OnJoinedRoomSuccessEvent += OnJoinedRoom;
            NetworkManager.GetInst().OnJoinedRoomFailedEvent += OnJoinedRoomFailed;
            NetworkManager.GetInst().OnExitRoomEvent += OnExitRoom;
        }

        public new void Dispose()
        {
            NetworkManager.GetInst().OnLoginSuccessEvent -= OnLoginSuccess;
            NetworkManager.GetInst().OnLoginFailedEvent -= OnLoginFailed;
            NetworkManager.GetInst().OnRoomListEvent -= OnRoomList;
            NetworkManager.GetInst().OnCreateRoomSuccessEvent -= OnCreateRoom;
            NetworkManager.GetInst().OnCreateRoomFailedEvent += OnCreateRoomFailed;
            NetworkManager.GetInst().OnJoinedRoomSuccessEvent -= OnJoinedRoom;
            NetworkManager.GetInst().OnJoinedRoomFailedEvent -= OnJoinedRoomFailed;
            NetworkManager.GetInst().OnExitRoomEvent -= OnExitRoom;
            base.Dispose();
        }
        
        public virtual void OnLoginSuccess(object sender, EventArgs e)
        {
        }
        public virtual void OnLoginFailed(object sender, EventArgs e)
        {
        }
        public virtual void OnRoomList(object sender, EventArgs e)
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
