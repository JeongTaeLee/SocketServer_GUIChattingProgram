using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;
using System.IO;

namespace JIGAPClientDXGUI.Engine 
{
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
        public string IpAddress { get; private set; }
        public string PortAddress { get; private set; }
        public bool bServerOn { get; private set; } = false;

        public PacketHandler PacketHandler { get; private set; } = new PacketHandler();
    }

    partial class NetworkManager : IDisposable
    {
        public void Dispose()
        {
            if (bServerOn)
                DisconnectServer();
        }

        /*
         * Data/ConnectData.txt 파일에서 서버 정보를 읽어옵니다.
         */
        private bool ParsingConnectData()
        {
            System.IO.DirectoryInfo di = new System.IO.DirectoryInfo("./Data");
            if (!di.Exists)
                di.Create();

            System.IO.FileInfo fi = new System.IO.FileInfo("./Data/ConnectData.txt");
            if (!fi.Exists)
            {
                fi.Create().Close();
                
                using (StreamWriter wr = new StreamWriter("./Data/ConnectData.txt"))
                {
                    wr.WriteLine("# 클라이언트에 필요한 연결 정보입니다.");
                    wr.WriteLine("# Tag 종류");
                    wr.WriteLine("# '#' 주석");
                    wr.WriteLine("# 'IP' ip 주소");
                    wr.WriteLine("# 'PORT' port 주소");
                    wr.WriteLine("# 입력 방식은 'Tag 입력' 입니다.");
                    wr.WriteLine("");
                    wr.WriteLine("# Connect 정보");
                    wr.WriteLine("");
                    wr.WriteLine("IP 127.0.0.1");
                    wr.WriteLine("");
                    wr.WriteLine("PORT 9199");
                }
            }

            bool IsParsingIP = false;
            bool IsParsingPort = false;
            using (StreamReader stream  = fi.OpenText())
            {
                string line;
                while ((line = stream.ReadLine()) != null)
                {
                    string tag = null;
                    string data = null;

                    int index = line.IndexOf(' ');
                    if (index > 0)
                    {
                        tag = line.Substring(0, index);
                        data = line.Substring(index + 1);
                    }
                    else
                        continue;

                    switch (tag)
                    {
                        case "#":
                            continue;
                            break;

                        case "IP":
                            IsParsingIP = true;
                            IpAddress = data;
                            break;

                        case "PORT":
                            IsParsingPort = true;
                            PortAddress = data;
                            break;

                        default:
                            continue;
                    }
                }
            }

            if (!IsParsingIP)
            {
                MessageBox.Show("./Data/ConnectData.txt 파일에서 IP 주소 정보를 읽어올 수 없습니다.");
                return false;
            }
            if (!IsParsingPort)
            {
                MessageBox.Show("./Data/ConnectData.txt 파일에서 PORT 주소 정보를 읽어올 수 없습니다.");
                return false;
            }

            return true;
        }


        public bool ConnectServer()
        {
            if (bServerOn)
                return false;

            if (!ParsingConnectData())
                return false;

            ServerSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse(IpAddress), Convert.ToInt32(PortAddress));

            try
            {
                ServerSock.Connect(localEndPoint);
            }
            catch
            {
                MessageBox.Show("서버에 접속 할 수 없습니다", "Error!");
                return false;
            }

            bServerOn = true;

            OnRecvMode();
            return bServerOn;
        }

        public void DisconnectServer()
        {
            if (!bServerOn)
                return;

            ServerSock.Close();
            bServerOn = false;
        }
    }

    partial class NetworkManager : IDisposable
    {
        public void SendLoginRequest(string id, string passward)
        {
            JIGAPPacket.LoginRequest loginRequest = new JIGAPPacket.LoginRequest();
            loginRequest.Id = id;
            loginRequest.Passward = passward;

            PacketHandler.SerializePacket(JIGAPPacket.Type.ELoginRequest, loginRequest);

            OnSendPacket();
        }
        private void RecvLoginAnswer(ref PacketHeader inHeader)
        {
        }

        private void OnSendPacket()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();

            args.Completed += OnSendComplate;
            args.SetBuffer(PacketHandler.sendBuffer, 0, PacketHandler.sendPosition);

            ServerSock.SendAsync(args);

        }

        private void OnSendComplate(object sender, SocketAsyncEventArgs e)
        {
            OnRecvMode();
        }

        private void OnRecvMode()
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
                PacketHandler.ClearSendBuffer();

                int paketSize = PacketHandler.ParsingPacketSize(e.Buffer);
                PacketHandler.SetRecvBuffer(e.Buffer, paketSize);

                OnRecvProcess();
            }
            else
            {

            }
        } 
        
        private void OnRecvProcess()
        {
            PacketHeader header = PacketHandler.ParsingPacketHeader();

            switch (header.Type)
            {
                case JIGAPPacket.Type.ELoginAnswer:
                    RecvLoginAnswer(ref header);
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

    }
}
