using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;

namespace JIGAPClientDXGUI.Engine 
{
    partial class NetworkManager
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
    }

    partial class NetworkManager
    {
        public bool ConnectServer(string inIp, string inPort)
        {
            if (bServerOn)
                return false;

            IpAddress = inIp;
            PortAddress = inPort;

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
}
