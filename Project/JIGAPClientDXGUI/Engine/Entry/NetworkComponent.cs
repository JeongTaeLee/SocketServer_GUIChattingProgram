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
            NetworkManager.Instance.singUpSuccess += OnSingUpScuccess;
            NetworkManager.Instance.singUpFailed += OnSingUpFailed;
            NetworkManager.Instance.LoginSuccess += OnLoginSuccess;
            NetworkManager.Instance.LoginFailed += OnLoginFailed;
            NetworkManager.Instance.JoinRoomSuccess += OnJoinRoomSuccess;
            NetworkManager.Instance.JoinRoomFailed += OnJoinRoomFailed;
            NetworkManager.Instance.roomListSuccess += OnRoomListSuccess;
        }

        public override void Release()
        {
            base.Release();

            NetworkManager.Instance.singUpSuccess -= OnSingUpScuccess;
            NetworkManager.Instance.singUpFailed -= OnSingUpFailed;
            NetworkManager.Instance.LoginSuccess -= OnLoginSuccess;
            NetworkManager.Instance.LoginFailed -= OnLoginFailed;
            NetworkManager.Instance.JoinRoomSuccess -= OnJoinRoomSuccess;
            NetworkManager.Instance.JoinRoomFailed -= OnJoinRoomFailed;
            NetworkManager.Instance.roomListSuccess -= OnRoomListSuccess;
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
    }
}
