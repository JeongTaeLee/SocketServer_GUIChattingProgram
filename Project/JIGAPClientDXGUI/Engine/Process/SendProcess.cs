using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace JIGAPClientDXGUI
{
    class SendProcess : BaseProcess
    {
        public void SendSingUpRequest(string strName, string strId, string strPassword)
        {
            handler.ClearSerializeBuffer();

            JIGAPPacket.SingUpRequest singUpRequest = new JIGAPPacket.SingUpRequest();
            singUpRequest.UserData = new JIGAPPacket.UserData();
            singUpRequest.UserData.Id = strId;
            singUpRequest.UserData.Name = strName;
            singUpRequest.Passward = strPassword;

            handler.SerializePacket(JIGAPPacket.Type.ESingUpRequest, singUpRequest);

            OnSendPacket();
        }
        public void SendLoginRequest(string strId, string strPassward)
        {
            handler.ClearSerializeBuffer();

            JIGAPPacket.LoginRequest loginRequest = new JIGAPPacket.LoginRequest();
            loginRequest.Id = strId;
            loginRequest.Passward = strPassward;

            handler.SerializePacket(JIGAPPacket.Type.ELoginRequest, loginRequest);

            OnSendPacket();
        }

        private void OnSendPacket()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();

            args.SetBuffer(handler.serializeBuffer, 0, handler.serializePosition);

            socket.SendAsync(args);
        }

    }
}
