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

        public void SendJoinRoom(string roomName)
        {
            handler.ClearSerializeBuffer();

            JIGAPPacket.JoinRoomRequest joinRoomrequest = new JIGAPPacket.JoinRoomRequest();
            joinRoomrequest.RoomInfo = new JIGAPPacket.RoomInfo();
            joinRoomrequest.RoomInfo.Roomname = roomName;

            handler.SerializePacket(JIGAPPacket.Type.EJoinRoomRequest, joinRoomrequest);
            OnSendPacket();
        }

        public void SendRoomListRequest()
        {
            handler.ClearSerializeBuffer();

            JIGAPPacket.EmptyPacket emptyPacket = new JIGAPPacket.EmptyPacket();
            emptyPacket.Type = JIGAPPacket.Type.ERoomListRequest;

            handler.SerializePacket(JIGAPPacket.Type.ERoomListRequest, emptyPacket);
            OnSendPacket();
        }
        public void SendChatRequest(string inUserId, string inUserName, string inMessage)
        {
            handler.ClearSerializeBuffer();

            JIGAPPacket.ChatRequest request = new JIGAPPacket.ChatRequest();
            request.StrMessage = inMessage;
            request.UserData = new JIGAPPacket.UserData();
            request.UserData.Id = inUserId;
            request.UserData.Name = inUserName;

            handler.SerializePacket(JIGAPPacket.Type.EChatRequest, request);
            OnSendPacket();
        }
        private void OnSendPacket()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();

            args.SetBuffer(handler.serializeBuffer, 0, handler.serializePosition + 1);

            socket.SendAsync(args);
        }


    }
}
