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
            NetworkManager.Instance._singUpSuccess += OnSingUpScuccess;
            NetworkManager.Instance._singUpFailed += OnSingUpFailed;
            NetworkManager.Instance._LoginSuccess += OnLoginSuccess;
            NetworkManager.Instance._LoginFailed += OnLoginFailed;
            NetworkManager.Instance._JoinRoomSuccess += OnJoinRoomSuccess;
            NetworkManager.Instance._JoinRoomFailed += OnJoinRoomFailed;
            NetworkManager.Instance._roomListSuccess += OnRoomListSuccess;
            NetworkManager.Instance._chatArriveSuccess += OnChatArrive;
            NetworkManager.Instance._createRoomSuccess += OnCreateRoomSuccess;
            NetworkManager.Instance._createRoomFailed += OnCreateRoomFailed;
        }

        public override void Release()
        {
            base.Release();

            NetworkManager.Instance._singUpSuccess -= OnSingUpScuccess;
            NetworkManager.Instance._singUpFailed -= OnSingUpFailed;
            NetworkManager.Instance._LoginSuccess -= OnLoginSuccess;
            NetworkManager.Instance._LoginFailed -= OnLoginFailed;
            NetworkManager.Instance._JoinRoomSuccess -= OnJoinRoomSuccess;
            NetworkManager.Instance._JoinRoomFailed -= OnJoinRoomFailed;
            NetworkManager.Instance._roomListSuccess -= OnRoomListSuccess;
            NetworkManager.Instance._chatArriveSuccess -= OnChatArrive;
            NetworkManager.Instance._createRoomSuccess -= OnCreateRoomSuccess;
            NetworkManager.Instance._createRoomFailed -= OnCreateRoomFailed;
        }

        public virtual void OnSingUpScuccess()
        {
        }
        public virtual void OnSingUpFailed(JIGAPPacket.SingUpFailedReason reason)
        {
        }

        public virtual void OnLoginSuccess()
        {
        }
        public virtual void OnLoginFailed(JIGAPPacket.LoginFailedReason reason)
        {
        }

        public virtual void OnJoinRoomSuccess(string roomName)
        {
        }

        public virtual void OnJoinRoomFailed()
        {
        }

        public virtual void OnRoomListSuccess(ref List<string> roomNameList)
        {
        }

        public virtual void OnChatArrive(string inId, string inName, string inMessage)
        {

        }

        public virtual void OnCreateRoomSuccess(string inRoomName)
        {

        }
        
        public virtual void OnCreateRoomFailed()
        {

        }
    }
}
