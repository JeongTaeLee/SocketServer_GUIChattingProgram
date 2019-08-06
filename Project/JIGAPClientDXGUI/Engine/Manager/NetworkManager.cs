using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;


using System.Net;
using System.Net.Sockets;
using System.IO;

namespace JIGAPClientDXGUI
{
    public struct UserData
    {
        public string UserName { get; set; }
        public string UserId { get; set; }
        public string RoomName { get; set; }
    }

    //Delegate & Event
    partial class NetworkManager
    {
        public delegate void SingUpSuccessCallBack();
        public delegate void SingUpFailedCallBack(JIGAPPacket.SingUpFailedReason reason);

        public delegate void LoginSuccessCallBack();
        public delegate void LoginFailedCallBack(JIGAPPacket.LoginFailedReason reason);

        public delegate void JoinRoomSuccessCallBack(string roomName);
        public delegate void JoinRoomFailedCallBack();

        public delegate void RoomListSuccessCallBack(ref List<string> roomNameList);

        public void InvokeSingUpSuccess() { singUpSuccess?.Invoke(); }
        public event SingUpSuccessCallBack singUpSuccess;

        public void InvokeSingUpFailed(JIGAPPacket.SingUpFailedReason reason) { singUpFailed?.Invoke(reason); }
        public event SingUpFailedCallBack singUpFailed;

        public void InvokeLoginSuccess() { LoginSuccess?.Invoke(); }
        public event LoginSuccessCallBack LoginSuccess;

        public void InvokeLoginFailed(JIGAPPacket.LoginFailedReason reason) { LoginFailed?.Invoke(reason); }
        public event LoginFailedCallBack LoginFailed;

        public void InvokeJoinRoomSuccess(string roomName) {JoinRoomSuccess?.Invoke(roomName);}
        public event JoinRoomSuccessCallBack JoinRoomSuccess;

        public void InvokeJoinRoomFailed() { JoinRoomFailed?.Invoke(); }
        public event JoinRoomFailedCallBack JoinRoomFailed;

        public void InvokeRoomListSuccess(ref List<string> roomNameList) { roomListSuccess?.Invoke(ref roomNameList); }
        public event RoomListSuccessCallBack roomListSuccess;
    }


    partial class NetworkManager : IDisposable
    {
        private static NetworkManager instance = null;
        public static NetworkManager Instance
        {
            get
            {

                if (instance == null)
                    instance = new NetworkManager();

                return instance;
            }
        }

        private Socket serverSock = null;
        private PacketHandler handler = new PacketHandler();
        private ConnectDataInfo connectDataInfo = new ConnectDataInfo();
        private Thread recvThread = null;

        public RecvProcess RecvProcess { get; private set; } = new RecvProcess();
        public SendProcess SendProcess { get; private set; } = new SendProcess();

        private UserData userData = new UserData();
        public string UserName { get => userData.UserName; set => userData.UserName = value; }
        public string RoomName { get => userData.RoomName; set => userData.RoomName = value; }
        public string UserId { get => userData.UserId; set => userData.UserId = value; }
        public bool bServerOn { get; private set; } = false;

    }

    partial class NetworkManager : IDisposable
    {
        public void Dispose()
        { 
            if (bServerOn)
                DisconnectServer();
        }

        public bool ConnectServer()
        {
            if (bServerOn)
                return false;

            connectDataInfo.ParseConnectDataInfo();
            serverSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse(connectDataInfo.strIpAddress), int.Parse(connectDataInfo.strPortAddress));

            try
            {
                serverSock.Connect(localEndPoint);
            }
            catch (Exception e)
            {
                MessageBox.Show($"서버에 접속 할 수 없습니다.\n{e.Message}", "Error!");
                return false;
            }

            bServerOn = true;

            RecvProcess.InitProcess(serverSock, handler);
            SendProcess.InitProcess(serverSock, handler);

            OnRecvPacket();

            return bServerOn;
        }

        public void DisconnectServer()
        {
            if (!bServerOn)
                return;

            serverSock.Dispose();

            bServerOn = false;
        }


        public void OnRecvPacket()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            
            args.Completed += OnRecvComplete;
            
            byte[] recvBuffer = new byte[PacketHandler.bufferSize];
            args.SetBuffer(recvBuffer, 0, PacketHandler.bufferSize);
            serverSock.ReceiveAsync(args);
        }

        private void OnRecvComplete(object sender, SocketAsyncEventArgs e)
        {
            if (serverSock.Connected && e.BytesTransferred > 0)
            {
                int paketSize = handler.ParsingTotalPacketSize(e.Buffer);
                handler.ClearParsingBuffer(e.Buffer, paketSize);

                RecvProcess.OnRecvProcess();

                OnRecvPacket();
            }
           
        }
    }
}
