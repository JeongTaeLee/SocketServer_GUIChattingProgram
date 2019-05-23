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
    public partial class Form1 : Form
    {
        Thread systemLogThread = null;
        bool systemLogThreadExit = false;

        public Form1()
        {
            InitializeComponent();

            serverCloseButton.Enabled = false;

            this.Shown += new EventHandler(ServerFormShown);
            this.FormClosing += new FormClosingEventHandler(ServerFormClosing);
        }

        #region FormEvent
        public void ServerFormShown(object obj, EventArgs args)
        {
            /*로그를 띄우기 위한 Thread 입니다. 0.1초마다 Log 큐에서 메시지를 꺼내옵니다.*/
            systemLogThread = new Thread(new ThreadStart(delegate ()
            {
                while (true)
                {
                    if (systemLogThreadExit)
                        break;

                    /*SystemLog가 있다면 Log를 띄웁니다.*/
                    if (ServerManager.GetInst().CheckSystemMsg())
                    {
                        string log = "System : " + ServerManager.GetInst().GetSystemMsg() + "\r\n";
                        this.Invoke(new Action(delegate ()
                        {
                            systemLogTextBox.AppendText(log);
                        }));
                    }

                    Thread.Sleep(100);
                }
            }
            ));
            systemLogThread.Start();
        }

        public void ServerFormClosing(object obj, EventArgs args)
        {
            systemLogThreadExit = true;
            systemLogThread.Join();
        }

        /*서버 열기 버튼을 클릭했을때 함수입니다*/
        #endregion

        private void ServerOpenButton_Click(object sender, EventArgs e)
        {
            if (!ServerManager.GetInst().bOnServer)
            {
                ServerManager.GetInst().ServerOpen(IPTextBox.Text, PortTextBox.Text);
                ServerOnBehavior();
            }
        }

        private void ServerCloseButton_Click(object sender, EventArgs e)
        {
            if (ServerManager.GetInst().bOnServer)
            {
                ServerManager.GetInst().ServerClose();
                SeverOffBehavior();
            }
        }

        private void ServerOnBehavior()
        {
            ServerOpenButton.Enabled = false;
            serverCloseButton.Enabled = true;
            IPTextBox.Enabled = false;
            PortTextBox.Enabled = false;
        }

        private void SeverOffBehavior()
        {
            ServerOpenButton.Enabled = true;
            serverCloseButton.Enabled = false;
            IPTextBox.Enabled = true;
            PortTextBox.Enabled = true;
        }
    }
}
