using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace JIGAPServerGUI
{
    public partial class LogForm : Form
    {
        private Thread printLogThread;
        bool bPrintLogThreadExit = false;
        public LogForm()
        {
            InitializeComponent();

            /*Form 이벤트를 추가합니다*/
            this.Shown += new EventHandler(LogFormShown);
        }

        /*Form 생성시 가장 마지막에 호출되는 이벤트입니다(폼이 생성되고 호출됩니다.)*/
        public void LogFormShown(object obj, EventArgs args )
        {
            /*로그를 띄우기 위한 Thread 입니다. 0.1초마다 Log 큐에서 메시지를 꺼내옵니다.*/
            printLogThread = new Thread(new ThreadStart(delegate ()
            {
                while (true)
                {
                    if (bPrintLogThreadExit)
                        break;

                    // /r 커서 위치를 맨앞으로 옮깁니다(현재라인)
                    string log = "system : LogText \r\n";

                    this.Invoke(new Action(delegate ()
                    {
                        LogTextBox.AppendText(log);
                    }));

                    Thread.Sleep(100);
                }
            }
            ));
            printLogThread.Start();
        }

        public void PrintSystemLog(string str)
        {
            LogTextBox.AppendText("System : " + str + "\r\n");
        }

        private void ServerCloseButton_Click(object sender, EventArgs e)
        {
            ServerManager.GetInst().ServerClose();
            bPrintLogThreadExit = true;

            printLogThread.Join();
            this.Close();
        }
    }
}
