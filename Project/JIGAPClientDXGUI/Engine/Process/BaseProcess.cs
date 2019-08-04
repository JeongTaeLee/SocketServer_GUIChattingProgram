using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace JIGAPClientDXGUI
{
    class BaseProcess
    {
        protected Socket socket = null;
        protected PacketHandler handler = null;

        public virtual void InitProcess(Socket _socket, PacketHandler _handler)
        {
            socket  = _socket;
            handler = _handler;
        }
    }
}
