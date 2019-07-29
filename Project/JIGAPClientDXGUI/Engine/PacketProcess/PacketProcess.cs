using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class PacketProcess
    {
        public void OnRecvProcess(PacketHandler inHandler)
        {
            PacketHeader header = new PacketHeader();
            inHandler.ParsingHeader(ref header);

            switch (header.Type)
            {
                case JIGAPPacket.Type.ELoginAnswer:
                    OnLoginAnswer(inHandler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.ESingUpAnswer:
                    OnSingUpAnswer(inHandler, header.PacketSize);
                    break;
                case JIGAPPacket.Type.ECreateRoomAnswer:
                    break;
                case JIGAPPacket.Type.EJoinRoomAnswer:
                    break;
                case JIGAPPacket.Type.ERoomListAnswer:
                    break;
                case JIGAPPacket.Type.EExitRoomAnswer:
                    break;
            }
        }
        
        public void OnSingUpAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.SingUpAnswer singUpAnswer = new JIGAPPacket.SingUpAnswer();
            inHandler.ParsingPacket(ref singUpAnswer, inSize);

            if (singUpAnswer.Success)
                NetworkManager.Instance.InvokeSingUpSuccess();
            else
                NetworkManager.Instance.InvokeSingUpFailed();
        }

        public void OnLoginAnswer(PacketHandler inHandler, int inSize)
        {
            JIGAPPacket.LoginAnswer loginAnswer = new JIGAPPacket.LoginAnswer();
            inHandler.ParsingPacket(ref loginAnswer, inSize);

            if (loginAnswer.Success)
                NetworkManager.Instance.InvokeLoginSuccess();
            else
                NetworkManager.Instance.InvokeLoginFailed();
        }

        
    }
}
