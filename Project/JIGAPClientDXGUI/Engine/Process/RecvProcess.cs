using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace JIGAPClientDXGUI
{
    class RecvProcess : BaseProcess
    {
        public void OnRecvProcess()
        {
            PacketHeader header = new PacketHeader();
            handler.ParsingHeader(ref header);

            switch (header.Type)
            {
                case JIGAPPacket.Type.ELoginAnswer:
                    OnLoginAnswer(handler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.ESingUpAnswer:
                    OnSingUpAnswer(handler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.ECreateRoomAnswer:
                    OnCreateRoomAnswer(handler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.EChatData:
                    OnChatArrive(handler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.EJoinRoomAnswer:
                    OnJoinRoomAnswer(handler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.ERoomListAnswer:
                    OnRoomListAnswer(handler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.EExitRoomAnswer:
                    break;
            }
        }

        private void OnSingUpAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.SingUpAnswer singUpAnswer = new JIGAPPacket.SingUpAnswer();
            inHandler.ParsingPacket(ref singUpAnswer, inSize);

            if (singUpAnswer.Success)
                NetworkManager.Instance.InvokeSingUpSuccess();
            else
                NetworkManager.Instance.InvokeSingUpFailed(singUpAnswer.SingUpReason);
        }

        private void OnLoginAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.LoginAnswer loginAnswer = new JIGAPPacket.LoginAnswer();
            inHandler.ParsingPacket(ref loginAnswer, inSize);

            if (loginAnswer.Success)
            {
                NetworkManager.Instance.UserId = loginAnswer.UserData.Id;
                NetworkManager.Instance.UserName = loginAnswer.UserData.Name;

                NetworkManager.Instance.InvokeLoginSuccess();
            }
            else
                NetworkManager.Instance.InvokeLoginFailed(loginAnswer.LoginReason);
        }

        private void OnJoinRoomAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.JoinRoomAnswer joinAnswer = new JIGAPPacket.JoinRoomAnswer();
            inHandler.ParsingPacket(ref joinAnswer, inSize);

            if (joinAnswer.Success)
            {
                NetworkManager.Instance.RoomName = joinAnswer.RoomInfo.Roomname;
                NetworkManager.Instance.InvokeJoinRoomSuccess(NetworkManager.Instance.RoomName);
            }
            else
                NetworkManager.Instance.InvokeJoinRoomFailed();
        }

        private void OnRoomListAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.RoomListAnswer answer = new JIGAPPacket.RoomListAnswer();
            inHandler.ParsingPacket(ref answer, inSize);

            List<string> strList = new List<string>();

            for (int i = 0; i < answer.RoomCount; ++i)
            {
                PacketHeader header = new PacketHeader();
                inHandler.ParsingHeader(ref header);

                if (header.Type == JIGAPPacket.Type.ERoomListElement)
                {
                    JIGAPPacket.RoomListElement element = new JIGAPPacket.RoomListElement();
                    inHandler.ParsingPacket(ref element, header.PacketSize);
                    strList.Add(element.RoomInfo.Roomname);
                }
            }

            NetworkManager.Instance.InvokeRoomListSuccess(ref strList);

        }

        private void OnCreateRoomAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.CreateRoomAnswer answer = new JIGAPPacket.CreateRoomAnswer();
            inHandler.ParsingPacket(ref answer, inSize);

            if (answer.Success)
                NetworkManager.Instance.InvokeSuccessCreateRoom(answer.RoomInfo.Roomname);
            else
                NetworkManager.Instance.InvokeFailedCreateRoom();

        }
        private void OnChatArrive(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.ChatData answer = new JIGAPPacket.ChatData();
            inHandler.ParsingPacket(ref answer, inSize);

            NetworkManager.Instance.InvokeChatArrive(answer.UserData.Id, answer.UserData.Name, answer.StrMessage);
        }
    }
}
