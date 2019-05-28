using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using JIGAPClientCLR;

namespace JIGAPClientDXGUI.Engine
{
    public partial class NetworkManager
    {
        public delegate void ChattingEventHandler(string sender, string message);
        public delegate void OnJoinedRoomHandler(string sender);

        public event EventHandler OnLoginSuccessEvent;
        public event EventHandler OnLoginFailedEvent;
        public event EventHandler OnRoomListEvent;
        public event EventHandler OnCreateRoomSuccessEvent;
        public event EventHandler OnCreateRoomFailedEvent;
        public event OnJoinedRoomHandler OnJoinedRoomSuccessEvent;
        public event EventHandler OnJoinedRoomFailedEvent;
        public event EventHandler OnExitRoomEvent;
        public event ChattingEventHandler OnChattingEvent;
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

        public string MyName { get; set; } = null;
        public string MyRoomName { get; set; } = null;
        private JIGAPClientWrap jigapClientWrap = null;

        public NetworkManager()
        {
            jigapClientWrap = new JIGAPClientWrap();
            jigapClientWrap.JIGAPWrapSetLoginCallBack(OnLoginCallBack);
            jigapClientWrap.JIGAPWrapSetLoginFailedCallBack(OnLoginFailedCallBack);
            jigapClientWrap.JIGAPWrapSetRoomListCallBack(OnRoomListCallBack);
            jigapClientWrap.JIGAPWrapSetCreateRoomCallBack(OnCreateRoomCallBack);
            jigapClientWrap.JIGAPWrapSetCreateRoomFailedCallBack(OnCreateRoomFailedCallBack);
            jigapClientWrap.JIGAPWrapSetJoinedRoomFaileCallBack(OnJoinedRoomFailedCallBack);
            jigapClientWrap.JIGAPWrapSetExitRoomCallBack(OnExitRoomCallBack);

            unsafe
            {
                jigapClientWrap.JIGAPWrapSetJoinedRoomCallBack(OnJoinedRoomCallBack);
                jigapClientWrap.JIGAPWrapSetChattingCallBack(OnChattingCallBack);
            }

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
        public unsafe void OnJoinedRoomCallBack(byte* roomName, int iLen)
        {
            byte[] newRoomName = new byte[iLen];
            for (int i = 0; i < iLen; ++i)
                newRoomName[i] = (byte)roomName[i];

            OnJoinedRoomSuccessEvent?.Invoke(Encoding.ASCII.GetString(newRoomName));
        }
        public void OnJoinedRoomFailedCallBack()
        {
            OnJoinedRoomFailedEvent?.Invoke(this, default);
        }
        public void OnExitRoomCallBack()
        {
            OnExitRoomEvent?.Invoke(this, default);
        }

        public unsafe void OnChattingCallBack(byte* sender, byte* message, int senderSize, int messageSize)
        {
            byte[] newSender = new byte[senderSize];
            byte[] newMessage = new byte[messageSize];

            for (int i = 0; i < senderSize; ++i)
                newSender[i] = (byte)sender[i];

            for (int i = 0; i < messageSize; ++i)
                newMessage[i] = message[i];

            OnChattingEvent?.Invoke(Encoding.ASCII.GetString(newSender), Encoding.ASCII.GetString(newMessage));
            
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
            System.Threading.Thread.Sleep(100);
        }
        public void RequestJoinedRoom(string roomName)
        {
            jigapClientWrap.JIGAPWrapRequestJoeindRoom(roomName);
        }
        public void RequestExitRoom()
        {
            jigapClientWrap.JIGAPWrapRequestExitRoom();
        }
        public void RequestChatting(string message)
        {
            jigapClientWrap.JIGAPWrapRequestChattingMessage(message);
        }
        public void GetRoomList(ref string[] strList)
        {
            jigapClientWrap.JIGAPGetRoomList(ref strList);
        }
    }
}
