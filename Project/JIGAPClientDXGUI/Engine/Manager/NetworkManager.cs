using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI.Engine.Manager
{
    public partial class NetworkManager
    {
        private event EventHandler OnLogin;
        private event EventHandler OnLoginFailed;
        private event EventHandler OnRoomList;
        private event EventHandler OnCreateRoom;
        private event EventHandler OnCreateRoomFailed;
        private event EventHandler OnJoinedRoom;
        private event EventHandler OnJoinedRoomFailed;
        private event EventHandler OnExitRoom;

    }

    public partial class NetworkManager
    {
        private static NetworkManager Instance = null;
        public static NetworkManager GetInst()
        {
            if (Instance == null)
                Instance = new NetworkManager();

            return Instance;
        }

        JIGAPClientCLR.JIGAPClientWrap jigapClientWrap = new JIGAPClientCLR.JIGAPClientWrap();

        public NetworkManager()
        {
              jigapClientWrap.
        }

        public void OnLoginCallBack()
        {
            OnLogin?.Invoke(this, default);
        }
        public void OnLoginFailedCallBack()
        {
            OnLoginFailed?.Invoke(this, default);
        }
        public void OnCreateRoomCallBack()
        {
            OnCreateRoom?.Invoke(this, default);
        }
        public void OnCreateRoomFailedCallBack()
        {
            OnCreateRoomFailed?.Invoke(this, default);
        }
        public void OnJoinedRoomCallBack()
        {
            OnJoinedRoom?.Invoke(this, default);
        }
        public void OnJoinedRoomFailedCallBack()
        {
            OnJoinedRoomFailed?.Invoke(this, default);
        }
        public void OnExitRoomCallBack()
        {
            OnExitRoom?.Invoke(this, default);
        }
    }
}
