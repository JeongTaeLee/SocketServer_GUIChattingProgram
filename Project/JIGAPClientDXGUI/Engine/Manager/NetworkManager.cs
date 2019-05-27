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
        public event EventHandler OnLoginSuccessEvent;
        public event EventHandler OnLoginFailedEvent;
        public event EventHandler OnRoomListEvent;
        public event EventHandler OnCreateRoomSuccessEvent;
        public event EventHandler OnCreateRoomFailedEvent;
        public event EventHandler OnJoinedRoomSuccessEvent;
        public event EventHandler OnJoinedRoomFailedEvent;
        public event EventHandler OnExitRoomEvent;

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
            OnLoginSuccessEvent?.Invoke(this, default);
        }
        public void OnLoginFailedCallBack()
        {
            OnLoginFailedEvent?.Invoke(this, default);
        }
        public void OnRoomListCallBack()
        {
            OnRoomListEvent?.Invoke(this, default);
        }
        public void OnCreateRoomCallBack()
        {
            OnCreateRoomSuccessEvent?.Invoke(this, default);
        }
        public void OnCreateRoomFailedCallBack()
        {
            OnCreateRoomFailedEvent?.Invoke(this, default);
        }
        public void OnJoinedRoomCallBack()
        {
            OnJoinedRoomSuccessEvent?.Invoke(this, default);
        }
        public void OnJoinedRoomFailedCallBack()
        {
            OnJoinedRoomFailedEvent?.Invoke(this, default);
        }
        public void OnExitRoomCallBack()
        {
            OnExitRoomEvent?.Invoke(this, default);
        }
        public bool StartClient()
        {
            return jigapClientWrap.JIGAPWrapClientStart("127.0.0.1", "9199");
        }

        public void RequestLogin(string str)
        {
            jigapClientWrap.JIGAPWrapRequestLogin(str);
        }

        public void RequestCreateRoom(string roomName)
        {
            jigapClientWrap.JIGAPWrapRequsetCreateRoom(roomName);
        }
        public void RequestRoomList()
        {
            jigapClientWrap.JIGAPWrapRequestRoomList();
        }
        public void RequestJoinedRoom(string roomName)
        {
            jigapClientWrap.JIGAPWrapRequestJoeindRoom(roomName);
        }
        public void RequestExitRoom()
        {
            jigapClientWrap.JIGAPWrapRequestExitRoom();
        }
        public void GetRoomList(ref string[] strList)
        {
            jigapClientWrap.JIGAPGetRoomList(ref strList);
        }
    }
}
