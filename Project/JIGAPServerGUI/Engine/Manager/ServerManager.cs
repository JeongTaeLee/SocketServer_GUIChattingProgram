using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPServerGUI
{
    partial class ServerManager
    {
        JIGAPServerCLR.JIGAPServerWrap jigapServer = new JIGAPServerCLR.JIGAPServerWrap();
        JIGAPServerCLR.JIGAPServerWrap.LogFunc logFunc = null;

        private static ServerManager instance = null;
        public static ServerManager Instance
        {
            get {
                if (instance == null)
                    instance = new ServerManager();
                return instance;
            }
        }

        public bool IsOnServer { get; private set; } = false;
    }

    partial class ServerManager
    {
        public bool OpenServer(string strInIp, string strInPort)
        {
            if (!IsOnServer)
                return (IsOnServer = jigapServer.OpenServer(strInIp, strInPort));

            return false;
        }

        public void CloseServer()
        {
            if (IsOnServer)
            {
                jigapServer.CloseServer();
                IsOnServer = false;
            }
        }

        public void RegisterLogFunc(JIGAPServerCLR.JIGAPServerWrap.LogFunc inLogFunc)
        {
            logFunc = inLogFunc;
            jigapServer.RegisterLogFunc(logFunc);
        }
    
        public void Dispose()
        {
            if (jigapServer != null)
            {
                jigapServer.Dispose();
                jigapServer = null;
            }
        }
    }
}
