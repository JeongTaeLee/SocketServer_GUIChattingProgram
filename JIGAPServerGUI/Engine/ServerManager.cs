using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPServerGUI
{
    class ServerManager
    { 
        private static ServerManager instance = null;

        /*C++로 만든 서버엔진을 C#에서 쓸 수 있게 랩핑한 객체입니다*/
        private JIGAPServerCLR.JIGAPServerWrap m_JigapServer;

        /*Server가 열려있는지를 판단하는 변수입니다.*/
        private bool m_bOnServer = false;
        public bool bOnServer
        {
            get
            {
                return m_bOnServer;
            }
        }

        private ServerManager()
        {
            m_JigapServer = new JIGAPServerCLR.JIGAPServerWrap();
            m_bOnServer = false;
        }
        public static ServerManager GetInst()
        {
            if (instance == null)
                instance = new ServerManager();

            return instance;
        }
        public bool ServerOpen(string strIpAddr, string strPortAddr)
        {
            /*Server Open합니다*/
            if (m_JigapServer.JIGAPWrapServerOpen(strIpAddr, strPortAddr))
            {
                m_bOnServer = true;
                return true;
            }

            return false;
        }

        public void ServerClose()
        {
            /*Server Close합니다*/
            m_JigapServer.JIGAPWrapServerClose();
            m_bOnServer = false;
        }
        public bool CheckSystemMsg() { return m_JigapServer.JIGAPCheckSystemMsg(); }
        public string GetSystemMsg() { return m_JigapServer.JIGAPGetSystemMsg(); }
    }
}
