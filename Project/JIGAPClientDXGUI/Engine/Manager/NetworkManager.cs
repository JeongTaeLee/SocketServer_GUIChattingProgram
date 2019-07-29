using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;
using System.IO;

namespace JIGAPClientDXGUI
{
    public struct UserData
    {
        public string userName { get; set; }
        public string userId { get; set; }
    }

    //Delegate & Event
    partial class NetworkManager
    {
        public delegate void SingUpSuccessCallBack();
        public delegate void SingUpFailedCallBack(JIGAPPacket.SingUpFailedReason reason);

        public delegate void LoginSuccessCallBack();
        public delegate void LoginFailedCallBack(JIGAPPacket.LoginFailedReason reason);

        public void InvokeSingUpSuccess() { singUpSuccess?.Invoke(); }
        public event SingUpSuccessCallBack singUpSuccess;
        public void InvokeSingUpFailed(JIGAPPacket.SingUpFailedReason reason) { singUpFailed?.Invoke(reason); }
        public event SingUpFailedCallBack singUpFailed;

        public void InvokeLoginSuccess() { LoginSuccess?.Invoke(); }
        public event LoginSuccessCallBack LoginSuccess;

        public void InvokeLoginFailed(JIGAPPacket.LoginFailedReason reason) { LoginFailed?.Invoke(reason); }
        public event LoginFailedCallBack LoginFailed;        
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
        public Socket ServerSock { get; private set; } = null;
        public bool bServerOn { get; private set; } = false;

        public PacketHandler PacketHandler { get; private set; } = new PacketHandler();
        public ConnectDataInfo connectDataInfo { get; private set; } = new ConnectDataInfo();
        public PacketProcess PacketProcess { get; private set; } = new PacketProcess();
        public UserData UserData { get => PacketProcess.UserData; }
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

            ServerSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse(connectDataInfo.strIpAddress), Convert.ToInt32(connectDataInfo.strPortAddress));

            try
            {
                ServerSock.Connect(localEndPoint);
            }
            catch (Exception e)
            {
                MessageBox.Show("서버에 접속 할 수 없습니다 " + e.Message,"Error!");
                return false;
            }

            bServerOn = true;

            OnRecvPacket();
            return bServerOn;
        }

        public void DisconnectServer()
        {
            if (!bServerOn)
                return;

            ServerSock.Close();
            ServerSock.Dispose();
            bServerOn = false;
        }
    }

    partial class NetworkManager
    {
        private void OnSendPacket()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();

            args.SetBuffer(PacketHandler.serializeBuffer, 0, PacketHandler.serializePosition);

            ServerSock.SendAsync(args);
        }

        private void OnRecvPacket()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            
            args.Completed += OnRecvComplete;
            
            byte[] recvBuffer = new byte[PacketHandler.bufferSize];
            args.SetBuffer(recvBuffer, 0, PacketHandler.bufferSize);
            ServerSock.ReceiveAsync(args);
        }

        private void OnRecvComplete(object sender, SocketAsyncEventArgs e)
        {

            if (ServerSock.Connected && e.BytesTransferred > 0)
            {
                int paketSize = PacketHandler.ParsingTotalPacketSize(e.Buffer);
                PacketHandler.ClearParsingBuffer(e.Buffer, paketSize);
                PacketProcess.OnRecvProcess(PacketHandler);

                OnRecvPacket();
            }
            else
            {
                if (!ServerSock.Connected)
                    ServerSock.Close();
            }
        } 
    }

    partial class NetworkManager
    { 
        public void SendSingUpRequest(string strName, string strId, string strPassword)
        {
            PacketHandler.ClearSerializeBuffer();

            JIGAPPacket.SingUpRequest singUpRequest = new JIGAPPacket.SingUpRequest();
            singUpRequest.UserData = new JIGAPPacket.UserData();
            singUpRequest.UserData.Id = strId;
            singUpRequest.UserData.Name = strName;
            singUpRequest.Passward = strPassword;

            PacketHandler.SerializePacket(JIGAPPacket.Type.ESingUpRequest, singUpRequest);

            OnSendPacket();
        }
        public void SendLoginRequest(string strId, string strPassward)
        {
            PacketHandler.ClearSerializeBuffer();

            JIGAPPacket.LoginRequest loginRequest = new JIGAPPacket.LoginRequest();
            loginRequest.Id         = strId;
            loginRequest.Passward   = strPassward;

            PacketHandler.SerializePacket(JIGAPPacket.Type.ELoginRequest, loginRequest);

            OnSendPacket();
        }
    }
}
