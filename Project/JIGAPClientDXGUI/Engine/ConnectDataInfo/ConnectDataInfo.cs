using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace JIGAPClientDXGUI
{
    class ConnectDataInfo
    {
        public string strIpAddress { get; private set; } = "None";
        public string strPortAddress { get; private set; } = "None";

        public bool ParseConnectDataInfo()
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

            using (StreamReader stream = fi.OpenText())
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
                            strIpAddress = data;
                            break;

                        case "PORT":
                            IsParsingPort = true;
                            strPortAddress = data;
                            break;

                        default:
                            continue;
                    }
                }
            }



            if (!IsParsingIP)
            {
                MessageBox.Show($"{fi.DirectoryName} 파일에서 IP 주소 정보를 읽어올 수 없습니다.");
                return false;
            }
            if (!IsParsingPort)
            {
                MessageBox.Show($"{fi.DirectoryName} 파일에서 PORT 주소 정보를 읽어올 수 없습니다.");
                return false;
            }

            return true;
        }
    }
}
