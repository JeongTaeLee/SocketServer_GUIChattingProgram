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
        private JIGAPServerCLR.JIGAPServerWrap jigapServerWrap = null;

        public bool bOnServer { get; private set; } = false;


        private ServerManager()
        {
            jigapServerWrap = new JIGAPServerCLR.JIGAPServerWrap();
            bOnServer = false;
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
            if (jigapServerWrap.JIGAPWrapServerOpen(strIpAddr, strPortAddr))
            {
                bOnServer = true;
                return true;
            }

            return false;
        }

        public void ServerClose()
        {
            /*Server Close합니다*/
            if (bOnServer)
            {
                jigapServerWrap.JIGAPWrapServerClose();
                bOnServer = false;
            }
        }
        public bool CheckSystemMsg() { return jigapServerWrap.JIGAPCheckSystemMsg(); }
        public string GetSystemMsg() { return jigapServerWrap.JIGAPGetSystemMsg(); }
    }
}
