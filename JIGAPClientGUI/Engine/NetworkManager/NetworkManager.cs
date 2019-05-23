using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace JIGAPClientGUI
{
    class NetworkManager
    {
        private static NetworkManager instance = null;
        private JIGAPClientCLR.JIGAPClientWrap jigapClientWrap = null;

        public bool bOnConnect { get; private set; } = false;

        private NetworkManager()
        {
            jigapClientWrap = new JIGAPClientCLR.JIGAPClientWrap();
            bOnConnect = false;
        }

        public static NetworkManager GetInst()
        {
            if (instance == null)
                instance = new NetworkManager();

            return instance;
        }
        
        public bool ClientStart(string strInIpAddr, string strInPortAddr)
        {
            bOnConnect = jigapClientWrap.JIGAPWrapClientStart(strInIpAddr, strInPortAddr);
            return bOnConnect;
        }

        public void ClientEnd()
        {
            if(bOnConnect)
            {
                jigapClientWrap.JIGAPWrapClientEnd();
                bOnConnect = false;
            }
        }

        public bool CheckMessageLog()
        {
            return jigapClientWrap.JIGAPCheckMessage();
        }

        public string GetMessageLog()
        {
            return jigapClientWrap.JIGAPGetMessageLog();
        }
    }
}
