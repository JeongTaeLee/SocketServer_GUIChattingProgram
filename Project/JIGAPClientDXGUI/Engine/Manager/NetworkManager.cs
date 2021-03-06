﻿using System;
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

        public delegate void ChatArriveCallBack(string inId, string inName, string inMessage);

        public delegate void CreateRoomSuccess(string inRoomName);
        public delegate void CreateRoomFailed();

        public void InvokeSingUpSuccess() { _singUpSuccess?.Invoke(); }
        public event SingUpSuccessCallBack _singUpSuccess;

        public void InvokeSingUpFailed(JIGAPPacket.SingUpFailedReason reason) { _singUpFailed?.Invoke(reason); }
        public event SingUpFailedCallBack _singUpFailed;

        public void InvokeLoginSuccess() { _LoginSuccess?.Invoke(); }
        public event LoginSuccessCallBack _LoginSuccess;

        public void InvokeLoginFailed(JIGAPPacket.LoginFailedReason reason) { _LoginFailed?.Invoke(reason); }
        public event LoginFailedCallBack _LoginFailed;

        public void InvokeJoinRoomSuccess(string roomName) { _JoinRoomSuccess?.Invoke(roomName); }
        public event JoinRoomSuccessCallBack _JoinRoomSuccess;

        public void InvokeJoinRoomFailed() { _JoinRoomFailed?.Invoke(); }
        public event JoinRoomFailedCallBack _JoinRoomFailed;

        public void InvokeRoomListSuccess(ref List<string> roomNameList) { _roomListSuccess?.Invoke(ref roomNameList); }
        public event RoomListSuccessCallBack _roomListSuccess;

        public void InvokeChatArrive(string inId, string inName, string InMessage) { _chatArriveSuccess?.Invoke(inId, inName, InMessage); }
        public event ChatArriveCallBack _chatArriveSuccess;

        public void InvokeSuccessCreateRoom(string roomName) { _createRoomSuccess?.Invoke(roomName); }
        public event CreateRoomSuccess _createRoomSuccess;

        public void InvokeFailedCreateRoom() { _createRoomFailed?.Invoke(); }
        public event CreateRoomFailed _createRoomFailed;

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
            else
                MyMessageBox.Show("연결 종료", "서버가 종료되었습니다", ()=> { Application.Exit(); });

        }
    }
}
