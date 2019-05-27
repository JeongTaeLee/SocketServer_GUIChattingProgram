using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JIGAPClientCLR;

namespace JIGAPClientDXGUI.Engine
{
    public partial class NetworkManager
    {
        public event EventHandler OnLogin;
        public event EventHandler OnLoginFailed;
        public event EventHandler OnRoomList;
        public event EventHandler OnCreateRoom;
        public event EventHandler OnCreateRoomFailed;
        public event EventHandler OnJoinedRoom;
        public event EventHandler OnJoinedRoomFailed;
        public event EventHandler OnExitRoom;

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

        private JIGAPClientWrap jigapClientWrap = null;

        public NetworkManager()
        {
            jigapClientWrap = new JIGAPClientWrap();
            jigapClientWrap.JIGAPWrapOnLoginCallBack(OnLoginCallBack);
            jigapClientWrap.JIGAPWrapOnLoginFailedCallBack(OnLoginFailedCallBack);
            jigapClientWrap.JIGAPWrapOnRoomListCallBack(OnRoomListCallBack);
            jigapClientWrap.JIGAPWrapOnCreateRoomCallBack(OnCreateRoomCallBack);
            jigapClientWrap.JIGAPWrapOnCreateRoomFailedCallBack(OnCreateRoomFailedCallBack);
            jigapClientWrap.JIGAPWrapOnJoinedRoomCallBack(OnJoinedRoomCallBack);
            jigapClientWrap.JIGAPWrapOnJoinedRoomFaileCallBack(OnJoinedRoomFailedCallBack);
            jigapClientWrap.JIGAPWrapOnExitRoomCallBack(OnExitRoomCallBack);
        }

        public void OnLoginCallBack()
        {
            OnLogin?.Invoke(this, default);
        }
        public void OnLoginFailedCallBack()
        {
            OnLoginFailed?.Invoke(this, default);
        }
        public void OnRoomListCallBack()
        {
            OnRoomList?.Invoke(this, default);
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
        public bool ConnecServer()
        {
            return jigapClientWrap.JIGAPWrapClientStart("127.0.0.1", "9199");
        }

        public void Login(string str)
        {
            jigapClientWrap.JIGAPWrapRequestLogin(str);
        }

    }
}
